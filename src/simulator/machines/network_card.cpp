#include "simulator/machines/network_card.hpp"

namespace muse::simulator {

    network_card::network_card(const uint64_t &band_width):band_width_(band_width) {
        this->band_width_current_ms_ = band_width / 1000;
    }

    bool network_card::add_task(message *msg) {
        if (msg == nullptr){
            return false;
        }
        if (msg->get_rpc_phase() == message_rpc_phase::RPC_FINISH){
            return false;
        }
        //判断消息阶段
        if (msg->get_rpc_phase() == message_rpc_phase::RPC_REQUEST){
            std::unique_lock lock(this->mtx_waiting_tasks_);
            if (msg->request == nullptr){
                lock.unlock();
                throw std::runtime_error("request is nullptr");
            }
            network_card_task task(msg, msg->get_message_bytes(),this->real_tick_, 0);
            waiting_tasks.push_back(task);
        }
        else if (msg->get_rpc_phase() == message_rpc_phase::RPC_RESPONSE){
            std::unique_lock lock(this->mtx_waiting_tasks_);
            if (msg->response == nullptr){
                lock.unlock();
                throw std::runtime_error("response is nullptr");
            }
            network_card_task task(msg, msg->response->getSize(),this->real_tick_, 0);
            waiting_tasks.push_back(task);
        }
        //计算剩余可用带宽
        return true;
    }

    bool network_card::get_leftover_task() {
        if (sending_tasks.empty()){
            return this->band_width_;
        }
        uint64_t used_band_width = 0;
//        std::for_each(sending_tasks.begin(), sending_tasks.end(), [&used_band_width](const std::pair<network_card_task, message*>& kv){
//            used_band_width += kv.first.get_float_bytes();
//        });
        return this->band_width_ - used_band_width;
    }

    auto network_card::next_tick(const uint64_t& tick) -> void {
        this->real_tick_ = tick;
        //判断是否有任务等待发送
        uint64_t host_left_float_bytes = this->band_width_current_ms_;
        //检查是否发生完了，发生完毕需要触发事件
        {
            auto start = sending_tasks.begin();
            while (start != sending_tasks.end()){
                //数据已经发生完毕了，需要挂到延迟队列里面去
                if (start->get_left_bytes() == 0){
                    message* ptr = start->get_message();
                    //如果再请求阶段，需要服务器触发事件
                    if (ptr->get_rpc_phase() == message_rpc_phase::RPC_REQUEST){
                        auto ht = HOST_DELAY_MATRIX::get_ptr();
                        auto random_delay = ht->get_delay(ptr->sender_ip, ptr->acceptor_id);
                        auto delay = tick + random_delay;
                        start->set_end_ms(delay);
                        latency_tasks_set.emplace(delay, ptr);
                    }
                    //发送完毕了
                    start = sending_tasks.erase(start); //摘掉
                    continue;
                }
                //计算剩余可用带宽
                if (host_left_float_bytes > 0){
                    auto task_left = start->get_left_bytes();
                    if (task_left >= host_left_float_bytes){
                        start->append_send_bytes(host_left_float_bytes);
                        host_left_float_bytes = 0;
                    }else{
                        host_left_float_bytes -= task_left;
                        start->append_send_bytes(task_left);
                    }
                }
                if (host_left_float_bytes == 0) break;
            }
        }
        if (host_left_float_bytes > 0){
            //将等待队列的任务加入到发送队列中
            auto bgn = waiting_tasks.begin();
            while (bgn != waiting_tasks.end()){
                auto wt_left = bgn->get_left_bytes();
                if (wt_left >= host_left_float_bytes){
                    bgn->append_send_bytes(host_left_float_bytes);
                    host_left_float_bytes = 0;
                }else{
                    host_left_float_bytes -= wt_left;
                    bgn->append_send_bytes(wt_left);
                }
                //加入发送队列中
                sending_tasks.push_back(*bgn);
                //从等待队列中删除
                bgn = waiting_tasks.erase(bgn);

                if (host_left_float_bytes == 0) break;
            }
        }
        //处理 latency_tasks_set
        auto it = latency_tasks_set.begin();
        while (it != latency_tasks_set.end() && it->first <= tick){
            auto ptr = it->second;

            if (ptr->get_rpc_phase() == message_rpc_phase::RPC_REQUEST){
                ptr->rpc_client_is_finish_sending = true;
            }
            if (ptr->get_rpc_phase() == message_rpc_phase::RPC_RESPONSE){
                ptr->rpc_server_is_finish_sending = true;
            }

            simulator_event ev{simulator_net_event_type::RPC_RESPONSE_FINISH, ptr};

            //加入到全局队列中
            simulator_net_event_queue::insert_event(ev);
            fmt::print("{} tick:{} add simulator_net_event_queue\n", __FUNCTION__, tick);
            it = latency_tasks_set.erase(it);
        }
    }

}