#include "simulator/machines/computer.hpp"


namespace muse::simulator{
    computer::computer()
    :_ip_address("0.0.0.0"), _memory_size(false),_cpu_frequency(0),_cpu_core_number(0) {

    }

    auto computer::set_memory_size(const uint64_t &memory_size) -> void {
        this->_memory_size = memory_size;
    }

    auto computer::set_cpu_frequency(const double &cpu_frequency) -> void {
        this->_cpu_frequency = cpu_frequency;
    }

    auto computer::set_ip_address(const std::string &ip_address) -> void {
        this->_ip_address = ip_address;
    }

    auto computer::set_cpu_core_number(const uint64_t &cpu_core_number) -> void {
        this->_cpu_core_number = cpu_core_number;
    }

    computer::computer(computer &&other) noexcept
    :_ip_address(std::move(other._ip_address)), _memory_size(other._memory_size), _cpu_frequency(other._cpu_frequency), _cpu_core_number(other._cpu_core_number){

    }

    auto computer::to_string() -> std::string {
        return fmt::format("ip_address: {}, memory_size: {}, cpu_frequency: {}GHz, cpu_core_number: {}.", this->_ip_address, this->_memory_size, this->_cpu_frequency, this->_cpu_core_number);
    }


}