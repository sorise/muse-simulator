#include "simulator/machines/simulator_host_event_queue.hpp"

namespace muse::simulator{
    simulator_host_event_base::simulator_host_event_base(uint64_t Id, time_t exp):ID(Id),expire(exp){

    }

    uint64_t simulator_host_event_base::getID() const{
        return ID;
    }
    time_t simulator_host_event_base::getExpire() const{
        return expire;
    }

    simulator_host_event::simulator_host_event(uint64_t Id, CallBack cb, time_t exp)
    :simulator_host_event_base(Id, exp), callBack(std::move(cb)){

    }

    uint64_t simulator_host_event_queue::GenTimeTaskID(){
        std::unique_lock<std::mutex> lock(mtx);
        return  ++initID;
    }

    /* 获得当前模拟世界的时间 */
    time_t simulator_host_event_queue::GetTick(){
        return SIMULATOR_WORLD_STATE::get_ptr()->get_tick();
    }

    bool simulator_host_event_queue::clearTimeout(simulator_host_event_base &nodeBase){
        auto it = nodes.find(nodeBase);
        if (it != nodes.end()){
            nodes.erase(it);
            return true;
        }
        return false;
    }

    time_t simulator_host_event_queue::checkTimeout(){
        if (nodes.empty()) return -1;//没有定时器时间
        time_t diff = nodes.begin()->getExpire() - simulator_host_event_queue::GetTick();
        return diff > 0? diff:0;
    }


    bool simulator_host_event_queue::runTask(){
        if (!nodes.empty()){
            //迭代器，但是加锁了
            auto it = nodes.begin();
            time_t diff = it->getExpire() - simulator_host_event_queue::GetTick();
            if (diff <= 0){
                it->callBack(); //调用 会不会有异常啊
                nodes.erase(it);
                return true;
            }
        }
        return false;
    }

    void simulator_host_event_queue::runTaskLoop() {
        auto current = simulator_host_event_queue::GetTick();
        auto start = nodes.begin();
        while (start != nodes.end()){
            time_t diff = start->getExpire() - current;
            if (diff <= 0){
                start->callBack();
                start = nodes.erase(start);
            }else{
                break;
            }
        }
    }
}