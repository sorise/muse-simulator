#ifndef MUSE_SIMULATOR_SIMULATOR_HPP
#define MUSE_SIMULATOR_SIMULATOR_HPP

#include <cstdint>
#include "machines/central_processing_unit.hpp"
#include "machines/synchronous_registry.hpp"
#include "machines/cpu_processing_matrix.hpp"
#include "machines/host_delay_matrix.hpp"
#include "simulator_object.hpp"
#include "fmt/format.h"

namespace muse::simulator{
    class SIMULATOR_CPP_WIN_API simulator: public simulator_object {
    private:
        void initialize_simulator() override;

        void simulator_operating_core() override;

        bool stop_simulator_condition() override;

        void simulator_report() override;

        void simulator_clean_up_resources() override;
    public:
        simulator() = default;

        ~simulator() override;
    };
}

#endif //MUSE_SIMULATOR_SIMULATOR_HPP
