#ifndef MUSE_SIMULATOR_SIMULATOR_TIMER_HPP
#define MUSE_SIMULATOR_SIMULATOR_TIMER_HPP

#include <cstdint>
#include <atomic>
#include <iostream>
#include "utils/toolkits.hpp"

namespace muse::simulator {
    /* 模拟环境下的定时器 */
    class SIMULATOR_CPP_WIN_API simulator_timer {
    private:
        std::atomic<uint64_t> tick;
    public:
        auto next() -> void ;
    };
}

#endif //MUSE_SIMULATOR_SIMULATOR_TIMER_HPP
