#include "simulator/simulator_world_state.hpp"

namespace muse::simulator {

    simulator_world_state::simulator_world_state(): tick_ms(0) {

    }

    void simulator_world_state::next_tick() {
        this->tick_ms++;
    }

    uint64_t simulator_world_state::get_tick() const{
        return this->tick_ms;
    }

    void simulator_world_state::reset_simulator_world() {
        this->tick_ms = 0;
    }
}