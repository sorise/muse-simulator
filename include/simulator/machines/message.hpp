#ifndef MUSE_SIMULATOR_MESSAGE_HPP
#define MUSE_SIMULATOR_MESSAGE_HPP

#include "utils/toolkits.hpp"
#include "transmitter_event.hpp"

#include <cstdint>
#include <string>

namespace muse::simulator{
    struct SIMULATOR_CPP_WIN_API message{
        uint64_t cpu_ms{0};           //处理时间 ms
        std::string sender_ip;     //发送者 id
        std::string acceptor_id;   //接受者 id
        uint64_t bytes{0};            //大小、字节

        TransmitterEvent* rpc_client_event{nullptr}; /* 等待触发的事件 */

        ResponseData* rpc_server_response{nullptr}; /* 服务端响应 */

        /* 当前RPC 请求是否完成发送 */
        bool rpc_client_is_finish_sending{false};
        /* 当前RPC 请求是否完成接收 */
        bool rpc_server_is_finish_sending{false};
        /* 客户端回调是否已经触发 */
        bool rpc_client_is_trigger{false};
        /* 服务端RPC函数是否已经触发 */
        bool rpc_server_is_trigger{false};

        message();

        message(const message& other) = delete;

        message(message&& other) noexcept;

        message& operator=(const message& other) = delete;

        message& operator=(message&& other) noexcept;

        virtual ~message();
    };
}

#endif //MUSE_SIMULATOR_MESSAGE_HPP
