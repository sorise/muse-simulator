#include "simulator/machines/network_card.hpp"

namespace muse::simulator {

    network_card::network_card(const uint64_t &band_width):band_width_(band_width) {
        this->band_width_current_ms_ = band_width / 1000;
    }

    bool network_card::add_task(message *msg) {
        if (msg == nullptr){
            return false;
        }
        //需要判断 msg的哪个阶段

        network_card_task task(msg, msg->bytes, 0,0);
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
        //判断是否有任务等待发送
        uint64_t left_float_bytes = this->band_width_current_ms_;

        if (!sending_tasks.empty()){
            //检查是否发生完了，发生完毕需要触发事件
            auto start = sending_tasks.begin();
            while (start != sending_tasks.end()){
                //数据已经发生完毕了，需要挂到延迟队列里面去
                if (start->first.get_left_bytes() == 0){
                    message* ptr = start->second;
                    //如果再请求阶段，需要服务器触发事件
                    if (ptr->get_rpc_phase() == message_rpc_phase::RPC_REQUEST){
                        auto delay = tick + HOST_DELAY_MATRIX::get_reference().get_delay(ptr->sender_ip, ptr->acceptor_id);
                        waiting_tasks_set.emplace(delay, ptr);
                    }
                    //发送完毕了
                    start = sending_tasks.erase(start); //摘掉
                }
                //计算剩余可用带宽
                auto kv = sending_tasks.begin();
                auto left_bytes = left_float_bytes - kv->first.get_left_bytes();
                if ( left_bytes > 0){

                }
            }
        }else{

        }


    }


    auto network_card_task::get_end_ms() const -> uint64_t {
        return this->end_ms;
    }

    uint64_t network_card_task::get_start_ms() const {
        return this->start_ms;
    }

    void network_card_task::set_start_ms(const uint64_t &_start_ms) {
        this->start_ms = _start_ms;
    }

    void network_card_task::set_end_ms(const uint64_t &_end_ms) {
        this->end_ms = _end_ms;
    }

    network_card_task::network_card_task(message *msg,uint64_t _bytes, uint64_t _start_ms, uint64_t _end_ms)
    :bytes(_bytes), start_ms(_start_ms), end_ms(_end_ms), msg(msg){

    }

    auto network_card_task::get_message() -> message * {
        return this->msg;
    }

    void network_card_task::set_message(message *_msg) {
        this->msg = _msg;
    }

    uint64_t network_card_task::operator+(const network_card_task &other) const {
        return this->bytes + other.bytes;
    }

    auto network_card_task::get_bytes() const -> uint64_t {
        return this->bytes;
    }

    auto network_card_task::get_left_bytes() const->uint64_t {
        return this->bytes  - send_bytes;
    }

    auto network_card_task::get_send_bytes() const -> uint64_t {
        return this->send_bytes;
    }

    auto network_card_task::append_send_bytes(const uint64_t &_bytes) {
        this->send_bytes += _bytes;
    }

//    auto network_card_task::get_float_bytes() const -> uint64_t {
//        return this->float_bytes;
//    }
}