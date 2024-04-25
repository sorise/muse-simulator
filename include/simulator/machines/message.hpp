#ifndef MUSE_SIMULATOR_MESSAGE_HPP
#define MUSE_SIMULATOR_MESSAGE_HPP

#include "utils/toolkits.hpp"
#include "utils/singleton.hpp"
#include "host_delay_matrix.hpp"
#include "transmitter_event.hpp"
#include "cpu_processing_matrix.hpp"

#include <cstdint>
#include <string>

namespace muse::simulator{

    enum class message_rpc_phase:uint8_t {
        RPC_REQUEST = 0,
        RPC_RESPONSE,
        RPC_FINISH,
    };

    struct SIMULATOR_CPP_WIN_API message{
        uint64_t cpu_ms{0};           //处理时间 ms
        std::string sender_ip;     //发送者 id
        std::string acceptor_id;   //接受者 id
        uint64_t bytes{0};            //大小、字节

        TransmitterEvent* request{nullptr}; /* 等待触发的事件 */

        ResponseData* response{nullptr}; /* 服务端响应 */

        /* 当前RPC 请求是否完成发送 */
        bool rpc_client_is_finish_sending{false};

        /* 服务端RPC函数是否已经触发 */
        bool rpc_server_is_trigger{false};

        /* 当前RPC 请求是否完成接收 */
        bool rpc_server_is_finish_sending{false};

        /* 客户端回调是否已经触发 */
        bool rpc_client_is_trigger{false};

        message();

        message(const message& other) = delete;

        message(message&& other) noexcept;

        message& operator=(const message& other) = delete;

        message& operator=(message&& other) noexcept;

        [[nodiscard]] message_rpc_phase get_rpc_phase() const;

        [[nodiscard]] uint64_t get_message_bytes() const;

        virtual ~message();
    };

    static message* create_message_factory(const std::string& sender, TransmitterEvent* _request){
        if (_request == nullptr){
            return nullptr;
        }
        //申请一个message
        auto msg_ptr = muse::simulator::new_by_pool<message>();
        msg_ptr->request = _request;
        msg_ptr->response = nullptr;
        msg_ptr->sender_ip = sender;
        msg_ptr->acceptor_id = _request->get_ip_address();
        msg_ptr->bytes = _request->get_serializer().byteCount();
        msg_ptr->cpu_ms = MUSE_CPU_PROCESSING_MATRIX::get_ptr()->get_server(_request->remote_process_name);
        return msg_ptr;
    }

    static void update_message_factory(message *msg, ResponseData *data) {
        if (msg == nullptr){
            return;
        }
        msg->response = data;
        msg->cpu_ms = MUSE_CPU_PROCESSING_MATRIX::get_ptr()->get_client(msg->request->remote_process_name);
        msg->bytes = data->getSize();
        msg->rpc_server_is_trigger = true;
    }

    static void delete_message_factory(message *msg) {
        if (msg != nullptr){
            if (msg->request != nullptr) muse::simulator::delete_by_pool(msg->request);
            if (msg->response != nullptr) muse::simulator::delete_by_pool(msg->response);
            muse::simulator::delete_by_pool(msg);
        }
    };

}

#endif //MUSE_SIMULATOR_MESSAGE_HPP
