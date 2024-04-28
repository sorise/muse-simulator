#ifndef MUSE_SIMULATOR_SIMULATOR_SETTING_HPP
#define MUSE_SIMULATOR_SIMULATOR_SETTING_HPP

#include <cstdint>

namespace muse::simulator {
    class simulator_setting {
    public:
        static constexpr const uint32_t HOST_USE_THREAD_COUNT = 1024;

        static constexpr const uint32_t NET_EVENT_USE_THREAD_TIMEOUT = 4096;
    };

    using MUSE_SIMULATOR_SETTING = muse::simulator::simulator_setting;
}

#endif //MUSE_SIMULATOR_SIMULATOR_SETTING_HPP
