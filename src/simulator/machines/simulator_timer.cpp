#include "simulator/machines/simulator_timer.hpp"

namespace muse::simulator {

    auto simulator_timer::next() -> void {
        tick.fetch_add(1);
    }
}