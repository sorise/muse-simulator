#ifndef MUSE_SIMULATOR_COMPUTER_SIMULATOR_TIMER_HPP
#define MUSE_SIMULATOR_COMPUTER_SIMULATOR_TIMER_HPP

#include <cstdint>
#include <list>
#include <set>
#include <string>
#include "timer_node.hpp"
#include "simulator/simulator_world_state.hpp"

namespace muse::simulator {
    class SIMULATOR_CPP_WIN_API computer_simulator_timer {
    private:
        uint64_t initial_id {0};

        std::mutex mtx;
        /* 存放执行节点 */
        std::set<timer_node_wrapper, std::less<>> nodes;
        /*活动时间*/
        static time_t GetTick();

        /* 获得时间唯一ID */
        uint64_t get_time_node_id();
    public:
        computer_simulator_timer();

        //添加到树上
        template<class F, class ...Args >
        timer_node_wrapper setTimeout(const std::string& _task_name,long long milliseconds, F && f, Args&&... args){
            if (_task_name.empty()) {
                throw std::runtime_error("task name is empty");
            }
            auto sh = new_by_pool_share<timer_node>(get_time_node_id(),std::bind(std::forward<F>(f), std::forward<Args>(args)...), false, 0 , GetTick() + milliseconds);
            sh->set_task_name(_task_name);
            timer_node_wrapper wrapper(sh);
            nodes.insert(wrapper);
            return wrapper;
        }

        //添加到树上普通 函数指针  + 引用
        template<typename F,typename R, typename ...Args>
        timer_node_wrapper setTimeout(const std::string& _task_name, long long milliseconds, F&& f, R& r,  Args&&... args){
            if (_task_name.empty()) {
                throw std::runtime_error("task name is empty");
            }
            timer_node::CallBack callBack = std::bind(std::forward<F>(f) , std::ref(r) ,std::forward<Args>(args)...);
            auto sh = new_by_pool_share<timer_node>(get_time_node_id(),callBack, false, 0 , GetTick() + milliseconds);
            sh->set_task_name(_task_name);
            timer_node_wrapper wrapper(sh);
            nodes.insert(wrapper);
            return wrapper;
        }

        //添加到树上 函数指针 + 指针
        template<typename F, typename R, typename ...Args>
        timer_node_wrapper setTimeout(const std::string& _task_name, long long milliseconds, F&& f, R* r,  Args&&... args){
            if (_task_name.empty()) {
                throw std::runtime_error("task name is empty");
            }
            timer_node::CallBack callBack = std::bind(std::forward<F>(f), r ,std::forward<Args>(args)...);
            auto sh = new_by_pool_share<timer_node>(get_time_node_id(), callBack, false, 0 , GetTick() + milliseconds);
            sh->set_task_name(_task_name);
            timer_node_wrapper wrapper(sh);
            nodes.insert(wrapper);
            return wrapper;
        }

        /*******************************************/
        //添加到树上
        template<class F, class ...Args >
        timer_node_wrapper setInternal(const std::string& _task_name, long long milliseconds, F && f, Args&&... args){
            if (_task_name.empty()) {
                throw std::runtime_error("task name is empty");
            }
            timer_node::CallBack callBack = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            auto sh = new_by_pool_share<timer_node>(get_time_node_id(), callBack, true, milliseconds , GetTick() + milliseconds);
            sh->set_task_name(_task_name);
            timer_node_wrapper wrapper(sh);
            nodes.insert(wrapper);
            return wrapper;
        }

        //添加到树上普通 函数指针  + 引用
        template<typename F,typename R, typename ...Args>
        timer_node_wrapper setInternal(const std::string& _task_name, long long milliseconds, F&& f, R& r,  Args&&... args){
            if (_task_name.empty()) {
                throw std::runtime_error("task name is empty");
            }
            timer_node::CallBack callBack = std::bind(std::forward<F>(f) , std::ref(r) ,std::forward<Args>(args)...);
            auto sh = new_by_pool_share<timer_node>(get_time_node_id(), callBack, true, milliseconds , GetTick() + milliseconds);
            sh->set_task_name(_task_name);
            timer_node_wrapper wrapper(sh);
            nodes.insert(wrapper);
        }

        //添加到树上 函数指针 + 指针
        template<typename F, typename R, typename ...Args>
        timer_node_wrapper setInternal(const std::string& _task_name, long long milliseconds, F&& f, R* r,  Args&&... args){
            if (_task_name.empty()) {
                throw std::runtime_error("task name is empty");
            }
            timer_node::CallBack callBack = std::bind(std::forward<F>(f), r ,std::forward<Args>(args)...);
            auto sh = new_by_pool_share<timer_node>(get_time_node_id(), callBack, true, milliseconds , GetTick() + milliseconds);
            sh->set_task_name(_task_name);
            timer_node_wrapper wrapper(sh);
            nodes.insert(wrapper);
        }

        /* 取消任务 */
        static void clearTimeout(timer_node_wrapper &nd);

        static void clearInternal(timer_node_wrapper &nd);

        /* 返回还有多久时间才有时间进行触发 */
        uint64_t checkTimeout();

        /* 执行任务 */
        std::string runTask();

        void runTaskLoop();
    };
}
#endif //MUSE_SIMULATOR_COMPUTER_SIMULATOR_TIMER_HPP
