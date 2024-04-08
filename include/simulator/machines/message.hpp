#ifndef MUSE_SIMULATOR_MESSAGE_HPP
#define MUSE_SIMULATOR_MESSAGE_HPP

#include "utils/toolkits.hpp"
#include <cstdint>

namespace muse::simulator{
    class SIMULATOR_CPP_WIN_API message{
        uint64_t cpu_ms; //处理时间 ms
        uint64_t sender_id;
        uint64_t acceptor_id;
        uint64_t bytes; //大小、字节
    };
}

#endif //MUSE_SIMULATOR_MESSAGE_HPP
