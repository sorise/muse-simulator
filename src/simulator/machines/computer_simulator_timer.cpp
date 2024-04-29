#include "simulator/machines/computer_simulator_timer.hpp"

namespace muse::simulator {


    time_t computer_simulator_timer::GetTick() {
        return MUSE_SIMULATOR_WORLD_STATE::get_reference().get_tick();
    }

    computer_simulator_timer::computer_simulator_timer() = default;

    void computer_simulator_timer::clearTimeout(timer_node_wrapper &nd) {
         nd.stop();
    }

    void computer_simulator_timer::clearInternal(timer_node_wrapper &nd) {
        nd.stop();
    }

    uint64_t computer_simulator_timer::checkTimeout() {
        if (nodes.empty()) return -1;//没有定时器时间
        uint64_t diff = nodes.begin()->expire() - GetTick();
        return diff > 0? diff:0;
    }

    std::string computer_simulator_timer::runTask() {
        if(!nodes.empty()){
            //迭代器，但是加锁了
            uint64_t current = GetTick();
            auto it = nodes.begin();
            timer_node_wrapper wrapper(*it);
            //已经到期了
            if (it->expire() <= current){
                //是否可以触发
                if (!it->get_clear_state()){
                    //调用 会不会有异常啊
                    it->trigger();
                    //是否是 internal
                    nodes.erase(it);
                    if (wrapper.is_duplicate()){
                        wrapper.duplicate_run_setting();
                        nodes.insert(wrapper);
                    }
                    return wrapper.get_task_name();
                }else{
                    nodes.erase(it);
                }
            }else{
                return {};
            }
        }
        return {};
    }

    void computer_simulator_timer::runTaskLoop() {
        uint64_t current = GetTick();
        auto start = nodes.begin();
        std::list<timer_node_wrapper> nodes_copy_wait;
        while (start != nodes.end()){
            if (start->expire() <= current){
                if (!start->get_clear_state()){
                    start->trigger();
                    if (start->is_duplicate()){
                        timer_node_wrapper wrapper = *start;
                        start = nodes.erase(start);
                        wrapper.duplicate_run_setting();
                        nodes_copy_wait.push_back(wrapper);
                    }else{
                        start = nodes.erase(start);
                    }
                }else{
                    start = nodes.erase(start);
                }
            }else{
                break;
            }
        }
        if (!nodes_copy_wait.empty()){
            nodes.insert(nodes_copy_wait.begin(), nodes_copy_wait.end());
        }
    }

    uint64_t computer_simulator_timer::get_time_node_id() {
        std::lock_guard lock(mtx);
        this->initial_id++;
        return initial_id;
    }
}