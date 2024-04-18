#include <utility>

#include "simulator/machines/message.hpp"

namespace muse::simulator {

    message::message(TransmitterEvent &&_event, const uint64_t &_cpu_ms, std::string _sender_ip)
    :event(std::move(_event)), cpu_ms(_cpu_ms), sender_ip(std::move(_sender_ip)), is_trigger(false){
        this->acceptor_id = this->event.get_ip_address();
        this->bytes = this->event.get_serializer().byteCount();

    }


}