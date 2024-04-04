#ifndef MUSE_SIMULATOR_SIMULATOR_TIMER_HPP
#define MUSE_SIMULATOR_SIMULATOR_TIMER_HPP

#include <cstdint>
#include <iostream>

namespace muse::simulator {
    /* 模拟环境下的定时器 */
    class simulator_timer {
    private:
        uint64_t tick;
    };
}

#endif //MUSE_SIMULATOR_SIMULATOR_TIMER_HPP
