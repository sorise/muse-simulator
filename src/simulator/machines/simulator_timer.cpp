#include "simulator/machines/simulator_timer.hpp"

namespace muse::simulator {

    auto simulator_timer::next_tick() -> void {
        tick.fetch_add(1);
    }
}