#include "simulator/machines/network_card_task.hpp"

namespace muse::simulator{

    uint64_t operator+(const network_card_task &me, const network_card_task &other) {
        return me.get_bytes() + other.get_bytes();
    }

    bool operator<(const network_card_task &me, const network_card_task &other) {
        if (me.get_end_ms() < other.get_end_ms()) {
            return true;
        } else if (me.get_end_ms() > other.get_end_ms()) {
            return false;
        }
        return me.get_start_ms() < other.get_start_ms(); //前插入的在前面 后插入的放在后面
    }

    auto network_card_task::get_end_ms() const -> uint64_t {
        return this->end_ms;
    }

    uint64_t network_card_task::get_start_ms() const {
        return this->start_ms;
    }

    void network_card_task::set_start_ms(const uint64_t &_start_ms) {
        this->start_ms = _start_ms;
    }

    void network_card_task::set_end_ms(const uint64_t &_end_ms) {
        this->end_ms = _end_ms;
    }

    network_card_task::network_card_task(message *msg,uint64_t _bytes, uint64_t _start_ms, uint64_t _end_ms)
    :bytes(_bytes),
    start_ms(_start_ms),
    end_ms(_end_ms),
    msg(msg),
    send_bytes(0){

    }

    auto network_card_task::get_message() -> message * {
        return this->msg;
    }

    void network_card_task::set_message(message *_msg) {
        this->msg = _msg;
    }

    uint64_t network_card_task::operator+(const network_card_task &other) const {
        return this->bytes + other.bytes;
    }

    auto network_card_task::get_bytes() const -> uint64_t {
        return this->bytes;
    }

    auto network_card_task::get_left_bytes() const->uint64_t {
        return this->bytes  - send_bytes;
    }

    auto network_card_task::get_send_bytes() const -> uint64_t {
        return this->send_bytes;
    }

    auto network_card_task::append_send_bytes(const uint64_t &_bytes) ->void{
        if (this->send_bytes + _bytes <= this->bytes){
            this->send_bytes += _bytes;
        }
    }

//    auto network_card_task::get_float_bytes() const -> uint64_t {
//        return this->float_bytes;
//    }
}