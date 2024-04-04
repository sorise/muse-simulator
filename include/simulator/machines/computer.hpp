//
// Created by remix on 24-3-26.
//

#ifndef MUSE_SIMULATOR_COMPUTER_HPP
#define MUSE_SIMULATOR_COMPUTER_HPP
#include "fmt/format.h"
#include "utils/toolkits.hpp"
#include <iostream>
#include <cstdint>

namespace muse::simulator{
    class SIMULATOR_CPP_WIN_API computer{
    private:
        std::string _ip_address;
        uint64_t _memory_size; //byte 内存大小
        double _cpu_frequency; //hz cpu频率
        uint64_t _cpu_core_number; //核心数量
    public:
        computer();

        computer(const computer& other) = default;

        computer(computer&& other) noexcept ;
        /* 单位为 字节 */
        auto set_memory_size(const uint64_t& memory_size) -> void;
        /* 单位为 GHZ */
        auto set_cpu_frequency(const double& cpu_frequency) -> void;
        /* 单位为 单个,意味着能够同时处理多少个任务  */
        auto set_cpu_core_number(const uint64_t& cpu_core_number) -> void;
        /* 单位为 IPV4  */
        auto set_ip_address(const std::string& ip_address) -> void;
        /* 字符串 */
        auto to_string() -> std::string;
    };
}

#endif //MUSE_SIMULATOR_COMPUTER_HPP
