#include "simulator/machines/simulator_timer.hpp"

namespace muse::simulator {

    simulator_timer_node_base::simulator_timer_node_base(uint64_t Id, time_t exp):ID(Id),expire(exp){

    }

    uint64_t simulator_timer_node_base::getID() const{
        return ID;
    }
    time_t simulator_timer_node_base::getExpire() const{
        return expire;
    }

    void simulator_timer_node_base::set_duplicate(const bool &_duplicate, const time_t &_interval) {
        this->duplicate = _duplicate;
        this->delay_interval = _interval;
    }

    bool simulator_timer_node_base::isDuplicate() const {
        return this->duplicate;
    }

    time_t simulator_timer_node_base::getInternal() const {
        return this->delay_interval;
    }


    simulator_timer_node::simulator_timer_node(uint64_t Id, CallBack cb, time_t exp)
            :simulator_timer_node_base(Id, exp), callBack(std::move(cb)){

    }
    /* 获得当前模拟世界的时间 */
    time_t simulator_timer::GetTick(){
        return SIMULATOR_WORLD_STATE::get_ptr()->get_tick();
    }

    bool simulator_timer::clearTimeout(simulator_timer_node_base &nodeBase){
        auto it = nodes.find(nodeBase);
        if (it != nodes.end()){
            nodes.erase(it);
            return true;
        }
        return false;
    }

    time_t simulator_timer::checkTimeout(){
        if (nodes.empty()) return -1;//没有定时器时间
        time_t diff = nodes.begin()->getExpire() - simulator_timer::GetTick();
        return diff > 0? diff:0;
    }


    bool simulator_timer::runTask(){
        if (!nodes.empty()){
            //迭代器，但是加锁了
            time_t current = simulator_timer::GetTick();
            auto it = nodes.begin();
            time_t diff = it->getExpire() - current;
            if (diff <= 0){
                it->callBack(); //调用 会不会有异常啊
                if (it->isDuplicate()){
                    simulator_timer_node newNode(GenTimeTaskID() ,it->callBack,   current + it->getInternal());
                    newNode.set_duplicate(true, it->getInternal());
                    nodes.insert(newNode);
                }
                nodes.erase(it);
                return true;
            }
        }
        return false;
    }

    void simulator_timer::runTaskLoop() {
        auto current = simulator_timer::GetTick();
        auto start = nodes.begin();
        while (start != nodes.end()){
            time_t diff = start->getExpire() - current;
            if (diff <= 0){
                start->callBack();
                if (start->isDuplicate()){
                    simulator_timer_node newNode(GenTimeTaskID() ,start->callBack,   current + start->getInternal());
                    newNode.set_duplicate(true, start->getInternal());
                    nodes.insert(newNode);
                }
                start = nodes.erase(start);
            }else{
                break;
            }
        }
    }

    uint64_t simulator_timer::GenTimeTaskID() {
        std::unique_lock<std::mutex> lock(mtx);
        return  ++initID;
    }
}
