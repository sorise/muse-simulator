#include <utility>

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
                return request->get_serializer().byteCount();
            }
        }else if (get_rpc_phase() == message_rpc_phase::RPC_RESPONSE){
            if (response != nullptr){
                return response->getSize();
            }
        }
        return 0;
    };
}