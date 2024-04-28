#ifndef MUSE_SIMULATOR_SIMULATOR_HPP
#define MUSE_SIMULATOR_SIMULATOR_HPP

#include <cstdint>
#include "fmt/format.h"
#include "simulator_object.hpp"
#include "machines/registry.hpp"
#include "machines/simulator_setting.hpp"
#include "machines/host_delay_matrix.hpp"
#include "machines/cpu_processing_matrix.hpp"
#include "machines/central_processing_unit.hpp"
#include "simulator/pool/pool.hpp"

namespace muse::simulator{
    class SIMULATOR_CPP_WIN_API simulator: public simulator_object {
    private:
        void initialize_simulator() override;

        /* 每毫秒运行一次，可以理解为帧 */
        void simulator_operating_core() override;

        bool stop_simulator_condition() override;

        void simulator_report() override;

        void simulator_clean_up_resources() override;
    public:
        simulator() = default;

        ~simulator() override;

        //得到当前微秒
        static std::chrono::microseconds get_tick(){
            std::chrono::time_point<std::chrono::system_clock> tp =
                    std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
            return std::chrono::duration_cast<std::chrono::microseconds>(tp.time_since_epoch());
        }
    };
}

#endif //MUSE_SIMULATOR_SIMULATOR_HPP
