#ifndef MUSE_SIMULATOR_COMPUTER_HPP
#define MUSE_SIMULATOR_COMPUTER_HPP

#include "fmt/format.h"
#include "network_card.hpp"
#include "utils/toolkits.hpp"
#include "central_processing_unit.hpp"
#include "transmitter_event.hpp"

#include <iostream>
#include <cstdint>

namespace muse::simulator{
    class SIMULATOR_CPP_WIN_API computer{
    private:
        std::string _ip_address;

        uint64_t _memory_size; //byte 内存大小

        double _cpu_frequency; //hz cpu频率

        uint64_t _cpu_core_number; //核心数量

        uint64_t band_width_; //带宽

        central_processing_unit cpu_;

        network_card network_card_;
    public:
        computer();

        computer(std::string ip_address, const uint64_t& memory_size, const double& cpu_frequency, const uint64_t& cpu_core_number, const uint64_t& _band_width);

        computer(const computer& other) = delete;

        computer(computer&& other) noexcept ;

        ~computer() = default;

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

        [[nodiscard]] auto get_ip_address() const -> std::string;

        [[nodiscard]] auto get_memory_size() const -> uint64_t;

        [[nodiscard]] auto get_cpu_frequency() const -> double;

        [[nodiscard]] auto get_cpu_core_number() const -> uint64_t;

        void RPC(TransmitterEvent* event);

        void next_tick(const uint64_t& ms_tick);

    };
}

#endif //MUSE_SIMULATOR_COMPUTER_HPP
