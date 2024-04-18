//
// Created by 14270 on 2024-04-08.
//

#ifndef MUSE_SIMULATOR_SIMULATOR_HPP
#define MUSE_SIMULATOR_SIMULATOR_HPP
#include <cstdint>
#include "network_dispatcher.hpp"
#include "../utils/toolkits.hpp"

namespace muse::simulator{
    class SIMULATOR_CPP_WIN_API simulator {
    private:
        uint64_t tick;

        network_dispatcher dispatcher;
    public:
        simulator();
        //运行实验
        auto run() -> void;
    };
}

#endif //MUSE_SIMULATOR_SIMULATOR_HPP
