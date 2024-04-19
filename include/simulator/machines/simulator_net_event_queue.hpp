#ifndef MUSE_SIMULATOR_SIMULATOR_EVENT_QUEUE_HPP
#define MUSE_SIMULATOR_SIMULATOR_EVENT_QUEUE_HPP

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
    class simulator_event{
        simulator_net_event_type  event_type_;
        message* message_;
    };

    class simulator_net_event_queue {
    public:
        static std::unique_ptr<simulator_net_event_queue> instance_;
    };
}

#endif //MUSE_SIMULATOR_SIMULATOR_EVENT_QUEUE_HPP
