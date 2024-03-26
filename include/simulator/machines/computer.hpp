//
// Created by remix on 24-3-26.
//

#ifndef MUSE_SIMULATOR_COMPUTER_HPP
#define MUSE_SIMULATOR_COMPUTER_HPP
#include <iostream>

namespace simulator{
    class computer{
    private:
         u_int64_t _bandwidth; //bit/s
    public:
        computer();

        computer(const uint64_t& bandwidth);
    };
}

#endif //MUSE_SIMULATOR_COMPUTER_HPP
