#ifndef MUSE_SIMULATOR_SIMULATOR_HOST_EVENT_QUEUE_HPP
#define MUSE_SIMULATOR_SIMULATOR_HOST_EVENT_QUEUE_HPP

#include <map>
#include <mutex>
#include <set>
#include <utility>
#include <functional>

#include "../simulator_world_state.hpp"
#include "utils/toolkits.hpp"
#include "host.hpp"

namespace muse::simulator {
    //事件
    struct SIMULATOR_CPP_WIN_API simulator_host_event_base{
    private:
        uint64_t ID;    //标识ID
        time_t expire;  //啥时候过期
    public:
        simulator_host_event_base(uint64_t Id, time_t exp);
        uint64_t getID() const;
        time_t getExpire() const;
    };

    static bool operator < (const simulator_host_event_base &me, const simulator_host_event_base &other){
        if (me.getExpire() < other.getExpire()){
            return true;
        }else if(me.getExpire() > other.getExpire()){
            return false;
        }
        return me.getID() < other.getID(); //前插入的在前面 后插入的放在后面
    }

    class SIMULATOR_CPP_WIN_API simulator_host_event: public simulator_host_event_base{
    public:
        using CallBack = std::function<void()>;
        CallBack callBack; //毁掉函数
        simulator_host_event(uint64_t Id,CallBack cb, time_t exp);
    };

    /* 主机内部事件队列 */
    class simulator_host_event_queue {
    private:
        std::mutex mtx;
        uint64_t initID {0};
        /* 存放执行节点 */
        std::set<simulator_host_event, std::less<>> nodes;
        /* 获得当前时间 */
        static time_t GetTick();
        /* 获得时间唯一ID */
        uint64_t GenTimeTaskID();
    public:
        simulator_host_event_queue() = default;

        //添加到树上
        template<class F, class ...Args >
        simulator_host_event_base setTimeout(long long milliseconds, F && f, Args&&... args){
            simulator_host_event::CallBack callBack = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            simulator_host_event tNode(GenTimeTaskID() ,callBack, GetTick() + milliseconds);
            nodes.insert(tNode);
            return static_cast<simulator_host_event_base>(tNode);
        }

        //添加到树上普通 函数指针  + 引用
        template<typename F,typename R, typename ...Args>
        simulator_host_event_base setTimeout(long long milliseconds, F&& f, R& r,  Args&&... args){
            simulator_host_event::CallBack callBack = std::bind(std::forward<F>(f) , std::ref(r) ,std::forward<Args>(args)...);
            simulator_host_event tNode(GenTimeTaskID() ,callBack, GetTick() + milliseconds);
            nodes.insert(tNode);
            return static_cast<simulator_host_event_base>(tNode);
        }

        //添加到树上 函数指针 + 指针
        template<typename F, typename R, typename ...Args>
        simulator_host_event_base setTimeout(long long milliseconds, F&& f, R* r,  Args&&... args){
            simulator_host_event::CallBack callBack = std::bind(std::forward<F>(f), r ,std::forward<Args>(args)...);
            simulator_host_event tNode(GenTimeTaskID() ,callBack, GetTick() + milliseconds);
            nodes.insert(tNode);
            return static_cast<simulator_host_event_base>(tNode);
        }

        /* 取消任务 */
        bool clearTimeout(simulator_host_event_base &nodeBase);

        /* 返回还有多久时间才有时间进行触发 */
        time_t checkTimeout();

        /* 执行任务 */
        bool runTask();

        void runTaskLoop();
    };
}
#endif //MUSE_SIMULATOR_SIMULATOR_HOST_EVENT_QUEUE_HPP
