#include "simulator/machines/network_card.hpp"

namespace muse::simulator {

    network_card::network_card(const uint64_t &band_width):band_width_(band_width) {
        this->band_width_current_ms_ = band_width / 1000;
    }

    bool network_card::add_task(message *msg) {
        if (msg == nullptr){
            return false;
        }
        network_card_task task(msg, msg->bytes, 0,0);
        //计算剩余可用带宽


        return true;
    }

    bool network_card::get_leftover_task() {
        if (sending_tasks.empty()){
            return this->band_width_;
        }
        uint64_t used_band_width = 0;
        std::for_each(sending_tasks.begin(), sending_tasks.end(), [&used_band_width](const std::pair<network_card_task, message*>& kv){
            used_band_width += kv.first.get_float_bytes();
        });
        return this->band_width_ - used_band_width;
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
    :bytes(_bytes), start_ms(_start_ms), end_ms(_end_ms), msg(msg), float_bytes(0){

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

    auto network_card_task::get_float_bytes() const -> uint64_t {
        return this->float_bytes;
    }
}