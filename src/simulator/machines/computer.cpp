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

    void computer::RPC_CALL(const uint16_t& port, TransmitterEvent *event) {
        //先丢给网络卡处理
        if (event != nullptr){
            message *msg = create_message_factory(this->_ip_address, port, event);
            if (msg != nullptr){
                if (event->get_ip_address() == this->_ip_address){
                    throw std::logic_error("error 4, RPC request object is the current computer");
                }
                this->network_card_.add_task(msg);
            }else{
                fmt::println("create_message_factory failed, from {}, to {}, func {}", this->_ip_address, event->ip_address, event->remote_process_name);
            }
        }
    }

    void computer::_next_tick(const uint64_t &ms_tick) {
        this->network_card_.next_tick(ms_tick); //网卡 执行发送任务
        this->_run_timer(ms_tick);   // 定时器任务运行
    }

    void computer::START_UP() {

    }

    bool computer::_run_timer(const uint64_t& ms_tick) {
        uint64_t us_tick = ms_tick * 1000;
        //判断是否用空闲的处理器  0 表示有定时器需要触发
        while (this->TIMER.checkTimeout() == 0){
            std::unique_lock lock(this->cpu_mtx_);
            auto idx = this->cpu_.get_spare_core(us_tick);
            if (idx != -1){
                //存在多核心
                auto task_name = TIMER.runTask();
                auto runtime = MUSE_CPU_PROCESSING_MATRIX::get_reference().get_function_processing_time(task_name);
                bool result = this->cpu_.carry_on_core(us_tick, runtime);
                if (!result){
                    throw std::logic_error("carry_on_core failed in computer::run_timer");
                }
            }
        }
        return true;
    }

    uint32_t computer::_get_spare_core_count(const uint64_t &ms_tick) {
        std::unique_lock lock(this->cpu_mtx_);
        return cpu_.get_spare_core_count(ms_tick);
    }

    uint32_t computer::_run_on_core(const uint64_t& ms_tick, const uint64_t &_ms_) {
        std::unique_lock lock(this->cpu_mtx_);
        this->cpu_.carry_on_core(ms_tick, _ms_ * 1000);
        return 0;
    }

    bool computer::_add_task(message *msg) {
        return this->network_card_.add_task(msg);
    }

    void computer::RPC_CALL(const uint16_t& port, TransmitterEvent *event, const uint64_t &_request_body_size) {
        if (event != nullptr){
            message *msg = create_message_factory(this->_ip_address, port, event);
            if (msg != nullptr){
                if (event->get_ip_address() == this->_ip_address){
                    throw std::logic_error("error 4, RPC request object is the current computer");
                }
                msg->set_request_body_size(_request_body_size);
                this->network_card_.add_task(msg);
            }else{
                fmt::println("create_message_factory failed, from {}, to {}, func {}", this->_ip_address, event->ip_address, event->remote_process_name);
            }
        }
    }
}