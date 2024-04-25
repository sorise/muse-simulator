#ifndef MUSE_SIMULATOR_CENTRAL_PROCESSING_UNIT_HPP
#define MUSE_SIMULATOR_CENTRAL_PROCESSING_UNIT_HPP

#include "utils/singleton.hpp"
#include "utils/toolkits.hpp"

#include <condition_variable>
#include <iostream>
#include <queue>
#include <mutex>

namespace muse::simulator {
    /*
     * @author: jx.kicker QQ: 1427035242 email: jxkicker@163.com
     * @des: 模拟主机的CPU
     * */
    class SIMULATOR_CPP_WIN_API central_processing_unit {
    private:
        /* 多少个核心 n核2n线程 */
        uint32_t core_num_;
        uint32_t thread_max_run_;
        std::vector<uint64_t> core_load_; //ns
    public:
        explicit central_processing_unit(const uint32_t& _core_num);

        central_processing_unit(const central_processing_unit &_cpu) = delete;

        central_processing_unit(central_processing_unit &&_cpu) noexcept;

        central_processing_unit& operator=(const central_processing_unit &_cpu) = delete;

        central_processing_unit& operator=(central_processing_unit &&_cpu) noexcept;

        int32_t get_spare_core(const uint64_t& us_tick);

        bool carry_on_core(const uint64_t& tick, const uint64_t &_us_);

        /* 毫秒 */
        void print_core_state(const uint64_t &ms_tick);

        ~central_processing_unit() = default;
    };
}

#endif //MUSE_SIMULATOR_CENTRAL_PROCESSING_UNIT_HPP
