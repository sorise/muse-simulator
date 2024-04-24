#ifndef MUSE_SIMULATOR_SIMULATOR_EVENT_QUEUE_HPP
#define MUSE_SIMULATOR_SIMULATOR_EVENT_QUEUE_HPP

#include <mutex>
#include <shared_mutex>
#include "message.hpp"
#include "host.hpp"

namespace muse::simulator {

    enum class simulator_net_event_type:uint8_t {
        /*
         * 客户端主机已经完成请求发送，可以出发服务端的函数了
         * */
        RPC_REQUEST_FINISH = 0,
        /*
         * 服务端主机已经完成函数调用，返回响应到请求主机，请求主机可以触发回调函数
         * */
        RPC_RESPONSE_FINISH,
    };

    //事件
    struct simulator_event{
        simulator_net_event_type event_type_;
        message* message_;
    };

    class simulator_net_event_queue {
    private:
        static std::unique_ptr<std::queue<simulator_event>> instance_;
        static std::shared_mutex mutex_;
    public:
        //插入事件
        static auto insert_event(simulator_event event) -> void;

        //判断空
        static auto is_empty() -> bool;

        //获取事件
        static auto pop_event(bool &success) -> simulator_event;

        //重置时间队列
        static auto reset() -> void;
    };
}

#endif //MUSE_SIMULATOR_SIMULATOR_EVENT_QUEUE_HPP
