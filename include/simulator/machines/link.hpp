#ifndef MUSE_SIMULATOR_LINK_HPP
#define MUSE_SIMULATOR_LINK_HPP
#include "utils/toolkits.hpp"
#include <cstdint>

namespace muse::simulator {
    class SIMULATOR_CPP_WIN_API link {
    private:
        uint64_t _bandwidth; //带宽
        uint64_t _latency; //延迟

    public:
        link();
    };
}

#endif //MUSE_SIMULATOR_LINK_HPP
