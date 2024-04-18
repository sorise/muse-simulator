#include "simulator/machines/network_card.hpp"

namespace muse::simulator {

    network_card::network_card(const uint64_t &band_width):band_width_(band_width) {

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

    network_card_task::network_card_task(uint64_t _bytes, uint64_t _start_ms, uint64_t _end_ms)
    :bytes(_bytes), start_ms(_start_ms), end_ms(_end_ms){

    }


}