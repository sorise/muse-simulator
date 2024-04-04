#ifndef MUSE_SIMULATOR_LINK_HPP
#define MUSE_SIMULATOR_LINK_HPP

#include <cstdint>
namespace muse::simulator {
    class link {
    private:
        uint64_t _bandwidth; //带宽
        uint64_t _latency; //延迟
    };
}

#endif //MUSE_SIMULATOR_LINK_HPP
