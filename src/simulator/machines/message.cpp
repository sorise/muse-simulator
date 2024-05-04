#include "simulator/machines/message.hpp"

namespace muse::simulator {


    message::message(message &&other) noexcept
    :request(other.request),
    response(other.response),
    cpu_ms(other.cpu_ms),
    sender_ip(std::move(other.sender_ip)),
    acceptor_id(std::move(other.acceptor_id)),
    bytes(other.bytes),
    rpc_client_is_finish_sending(other.rpc_client_is_finish_sending),
    rpc_server_is_finish_sending(other.rpc_server_is_finish_sending),
    rpc_server_is_trigger(other.rpc_server_is_trigger),
    rpc_client_is_trigger(other.rpc_client_is_trigger)
    {

    }

    message::~message() = default;

    message &message::operator=(message &&other) noexcept {
        if (this != &other){
            cpu_ms = other.cpu_ms;
            sender_ip = std::move(other.sender_ip);
            acceptor_id = std::move(other.acceptor_id);
            bytes = other.bytes;
            request = other.request;
            other.request = nullptr;
            response = other.response;
            other.response = nullptr;
            rpc_client_is_finish_sending = other.rpc_client_is_finish_sending;
            rpc_server_is_finish_sending = other.rpc_server_is_finish_sending;
            rpc_server_is_trigger = other.rpc_server_is_trigger;
            rpc_client_is_trigger = other.rpc_client_is_trigger;
        }
        return *this;
    }

    message::message() = default;

    message_rpc_phase message::get_rpc_phase() const {
        if (rpc_server_is_trigger && !rpc_client_is_trigger){
            return message_rpc_phase::RPC_RESPONSE;
        }
        if (rpc_client_is_trigger){
            return message_rpc_phase::RPC_FINISH;
        }
        return message_rpc_phase::RPC_REQUEST;
    }

    uint64_t message::get_message_bytes() const {
        if (get_rpc_phase() == message_rpc_phase::RPC_REQUEST){
            if (request != nullptr){
                //如果已经设置了request_body_size
                if (request_tag){
                    return request_body_size;
                }
                return request->get_serializer().byteCount();
            }
        }else if (get_rpc_phase() == message_rpc_phase::RPC_RESPONSE){
            if (response != nullptr){
                //如果已经设置了response_body_size
                if (response_tag){
                    return response_body_size;
                }
                return response->getSize();
            }
        }
        return 0;
    }

    void message::set_request_body_size(uint64_t _size) {
        if (!request_tag){
            //只能设置一次
            request_tag = true;
            request_body_size = _size;
        }
    }

    void message::set_response_body_size(uint64_t _size) {
        if (!response_tag){
            //只能设置一次
            response_tag = true;
            response_body_size = _size;
        }
    }
}