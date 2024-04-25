#include <utility>

#include "simulator/machines/computer.hpp"


namespace muse::simulator{
    computer::computer()
    :_ip_address("127.0.0.1"),
    _memory_size(false),_cpu_frequency(2.5),
    _cpu_core_number(4), cpu_(4),
    network_card_(5120000),
    band_width_(0) {

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
    :_ip_address(std::move(other._ip_address)),
    band_width_(other.band_width_),
    _memory_size(other._memory_size),
    _cpu_frequency(other._cpu_frequency),
    _cpu_core_number(other._cpu_core_number),
     cpu_(std::move(other.cpu_)),
     network_card_(std::move(other.network_card_)){

    }

    auto computer::to_string() -> std::string {
        return fmt::format("ip_address: {}, memory_size: {}, cpu_frequency: {}GHz, cpu_core_number: {}.", this->_ip_address, this->_memory_size, this->_cpu_frequency, this->_cpu_core_number);
    }

    auto computer::get_ip_address() const -> std::string {
        return this->_ip_address;
    }

    auto computer::get_memory_size() const -> uint64_t {
        return this->_memory_size;
    }

    auto computer::get_cpu_frequency() const -> double {
        return this->_cpu_frequency;
    }

    auto computer::get_cpu_core_number() const -> uint64_t {
        return this->_cpu_core_number;
    }


    computer::computer(std::string ip_address, const uint64_t &memory_size, const double &cpu_frequency,const uint64_t &cpu_core_number, const uint64_t& _band_width)
    :_ip_address(std::move(ip_address)),
    _memory_size(memory_size),
    _cpu_frequency(cpu_frequency),
    _cpu_core_number(cpu_core_number),
    band_width_(_band_width),
    network_card_(_band_width),
    cpu_(cpu_core_number){

    }

    void computer::RPC(TransmitterEvent *event) {
        //先丢给网络卡处理
        if (event != nullptr){
            message *msg = create_message_factory(this->_ip_address,event);
            if (msg != nullptr){
                this->network_card_.add_task(msg);
            }else{
                fmt::println("create_message_factory failed, from {}, to {}, func {}", this->_ip_address, event->ip_address, event->remote_process_name);
            }
        }
    }

    void computer::next_tick(const uint64_t &ms_tick) {
        this->network_card_.next_tick(ms_tick);
    }
}