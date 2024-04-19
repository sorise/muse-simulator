#include <utility>

#include "simulator/machines/message.hpp"

namespace muse::simulator {


    message::message(message &&other) noexcept
    :rpc_client_event(other.rpc_client_event),
    rpc_server_response(other.rpc_server_response),
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

    message::~message() {

    }

    message &message::operator=(message &&other) noexcept {
        if (this != &other){
            cpu_ms = other.cpu_ms;
            sender_ip = std::move(other.sender_ip);
            acceptor_id = std::move(other.acceptor_id);
            bytes = other.bytes;
            rpc_client_event = other.rpc_client_event;
            other.rpc_client_event = nullptr;
            rpc_server_response = other.rpc_server_response;
            other.rpc_server_response = nullptr;
            rpc_client_is_finish_sending = other.rpc_client_is_finish_sending;
            rpc_server_is_finish_sending = other.rpc_server_is_finish_sending;
            rpc_server_is_trigger = other.rpc_server_is_trigger;
            rpc_client_is_trigger = other.rpc_client_is_trigger;
        }
        return *this;
    }

    message::message() {

    }
}