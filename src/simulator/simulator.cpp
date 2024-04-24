#include "simulator/simulator.hpp"

namespace muse::simulator{

    simulator::~simulator() = default;

    void simulator::initialize_simulator() {

    }

    void simulator::simulator_operating_core() {

    }

    bool simulator::stop_simulator_condition() {
        return true;
    }

    void simulator::simulator_report() {
        fmt::println("simulator report");
    }
}