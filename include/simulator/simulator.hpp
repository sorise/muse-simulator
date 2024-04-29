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

        std::function<bool(void)> stop_function_; //停止条件
    public:
        /* 设置仿真停止条件。*/
        void set_stop_condition(std::function<bool(void)>&& f);

        simulator() = default;

        ~simulator() override;
    };
}

#endif //MUSE_SIMULATOR_SIMULATOR_HPP
