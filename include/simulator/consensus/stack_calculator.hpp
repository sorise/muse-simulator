//
// Created by 14270 on 2024-04-08.
//

#ifndef MUSE_SIMULATOR_STACK_CALCULATOR_HPP
#define MUSE_SIMULATOR_STACK_CALCULATOR_HPP
#include <iostream>
#include <cstdint>
#include <cmath>
#include "../utils/toolkits.hpp"

namespace muse::simulator{
    class SIMULATOR_CPP_WIN_API stack_calculator {
    public:
        static auto epoch(uint64_t chain_height, uint64_t block_height, uint32_t n) -> int64_t;

        static auto decay(int64_t _epoch)->double;
    };
}
#endif //MUSE_SIMULATOR_STACK_CALCULATOR_HPP
