#ifndef MUSE_SIMULATOR_SIMULATOR_TIMER_HPP
#define MUSE_SIMULATOR_SIMULATOR_TIMER_HPP

#include <cstdint>
#include <atomic>
#include <iostream>
#include <functional>

#include "simulator/simulator_world_state.hpp"
#include "utils/toolkits.hpp"
#include "host.hpp"

namespace muse::simulator {

    //事件
    struct SIMULATOR_CPP_WIN_API simulator_timer_node_base{
    private:
        uint64_t ID; //标识ID
        time_t expire;  //啥时候过期
        bool duplicate{true}; //是否重复执行
        time_t delay_interval{0}; //间隔
    public:
        simulator_timer_node_base(uint64_t Id, time_t exp);
        void set_duplicate(const bool &_duplicate, const time_t& _interval);
        [[nodiscard]] uint64_t getID() const;
        [[nodiscard]] time_t getExpire() const;
        [[nodiscard]] bool isDuplicate() const;
        [[nodiscard]] time_t getInternal() const;
    };

    static bool operator < (const simulator_timer_node_base &me, const simulator_timer_node_base &other){
        if (me.getExpire() < other.getExpire()){
            return true;
        }else if(me.getExpire() > other.getExpire()){
            return false;
        }
        return me.getID() < other.getID(); //前插入的在前面 后插入的放在后面
    }

    class SIMULATOR_CPP_WIN_API simulator_timer_node: public simulator_timer_node_base{
    public:
        using CallBack = std::function<void()>;
        CallBack callBack; //毁掉函数
        simulator_timer_node(uint64_t Id,CallBack cb, time_t exp);
    };

    /* 模拟环境下的定时器 */
    class SIMULATOR_CPP_WIN_API simulator_timer {
    private:
        std::mutex mtx;
        uint64_t initID {0};
        /* 存放执行节点 */
        std::set<simulator_timer_node, std::less<>> nodes;
        /* 获得当前时间 */
        static time_t GetTick();
        /* 获得时间唯一ID */
        uint64_t GenTimeTaskID();
    public:
        simulator_timer() = default;

        //添加到树上
        template<class F, class ...Args >
        simulator_timer_node_base setTimeout(long long milliseconds, F && f, Args&&... args){
            simulator_timer_node::CallBack callBack = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            simulator_timer_node tNode(GenTimeTaskID() ,callBack, GetTick() + milliseconds);
            nodes.insert(tNode);
            return static_cast<simulator_timer_node_base>(tNode);
        }

        //添加到树上普通 函数指针  + 引用
        template<typename F,typename R, typename ...Args>
        simulator_timer_node_base setTimeout(long long milliseconds, F&& f, R& r,  Args&&... args){
            simulator_timer_node::CallBack callBack = std::bind(std::forward<F>(f) , std::ref(r) ,std::forward<Args>(args)...);
            simulator_timer_node tNode(GenTimeTaskID() ,callBack, GetTick() + milliseconds);
            nodes.insert(tNode);
            return static_cast<simulator_timer_node_base>(tNode);
        }

        //添加到树上 函数指针 + 指针
        template<typename F, typename R, typename ...Args>
        simulator_timer_node_base setTimeout(long long milliseconds, F&& f, R* r,  Args&&... args){
            simulator_timer_node::CallBack callBack = std::bind(std::forward<F>(f), r ,std::forward<Args>(args)...);
            simulator_timer_node tNode(GenTimeTaskID() ,callBack, GetTick() + milliseconds);
            nodes.insert(tNode);
            return static_cast<simulator_timer_node_base>(tNode);
        }

        /*******************************************/
        //添加到树上
        template<class F, class ...Args >
        simulator_timer_node_base setInternal(long long milliseconds, F && f, Args&&... args){
            simulator_timer_node::CallBack callBack = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            simulator_timer_node tNode(GenTimeTaskID() ,callBack, GetTick() + milliseconds);
            tNode.set_duplicate(true, milliseconds);
            nodes.insert(tNode);
            return static_cast<simulator_timer_node_base>(tNode);
        }

        //添加到树上普通 函数指针  + 引用
        template<typename F,typename R, typename ...Args>
        simulator_timer_node_base setInternal(long long milliseconds, F&& f, R& r,  Args&&... args){
            simulator_timer_node::CallBack callBack = std::bind(std::forward<F>(f) , std::ref(r) ,std::forward<Args>(args)...);
            simulator_timer_node tNode(GenTimeTaskID() ,callBack, GetTick() + milliseconds);
            tNode.set_duplicate(true, milliseconds);
            nodes.insert(tNode);
            return static_cast<simulator_timer_node_base>(tNode);
        }

        //添加到树上 函数指针 + 指针
        template<typename F, typename R, typename ...Args>
        simulator_timer_node_base setInternal(long long milliseconds, F&& f, R* r,  Args&&... args){
            simulator_timer_node::CallBack callBack = std::bind(std::forward<F>(f), r ,std::forward<Args>(args)...);
            simulator_timer_node tNode(GenTimeTaskID() ,callBack, GetTick() + milliseconds);
            tNode.set_duplicate(true, milliseconds);
            nodes.insert(tNode);
            return static_cast<simulator_timer_node_base>(tNode);
        }

        /* 取消任务 */
        bool clearTimeout(simulator_timer_node_base &nodeBase);

        /* 返回还有多久时间才有时间进行触发 */
        time_t checkTimeout();

        /* 执行任务 */
        bool runTask();

        void runTaskLoop();
    }
}

#endif //MUSE_SIMULATOR_SIMULATOR_TIMER_HPP
