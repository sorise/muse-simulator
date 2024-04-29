#ifndef MUSE_SIMULATOR_COMPUTER_HPP
#define MUSE_SIMULATOR_COMPUTER_HPP

#include "fmt/format.h"
#include "utils/toolkits.hpp"
#include "network_card.hpp"
#include "transmitter_event.hpp"
#include "cpu_processing_matrix.hpp"
#include "central_processing_unit.hpp"
#include "computer_simulator_timer.hpp"

#include <cstdint>
#include <iostream>

namespace muse::simulator{
    class SIMULATOR_CPP_WIN_API network_dispatcher;

    /*
     * 模拟显示中的计算机
     * _function 表示为模拟器调度的方法，不应该由用户主动调用，否则会导致模拟失败。
     * */
    class SIMULATOR_CPP_WIN_API computer{
    private:
        std::string _ip_address;

        uint64_t _memory_size; //byte 内存大小

        double _cpu_frequency; //hz cpu频率

        uint64_t _cpu_core_number; //核心数量

        uint64_t band_width_; //带宽

        std::mutex cpu_mtx_;

        central_processing_unit cpu_;  //CPU 模拟器

        network_card network_card_;    //网卡 模拟器
    protected:
        computer_simulator_timer TIMER; //分布式仿真定时器
    public:
        computer();

        computer(std::string ip_address, const uint64_t& memory_size, const double& cpu_frequency, const uint64_t& cpu_core_number, const uint64_t& _band_width);

        computer(const computer& other) = delete;

        computer(computer&& other) noexcept ;

        virtual ~computer() = default;

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

        void RPC_CALL(TransmitterEvent* event);

        /* 进入下一个时间 */
        auto _next_tick(const uint64_t& ms_tick) ->void;
        /* 运行定时器 */
        auto _run_timer(const uint64_t& ms_tick) ->bool;
        //看空闲核数量
        auto _get_spare_core_count(const uint64_t& ms_tick) ->uint32_t;
        /* 传递 毫秒时间 */
        auto _run_on_core(const uint64_t& ms_tick, const uint64_t &_ms_) ->uint32_t;
        //给网卡添加一个传输任务
        auto _add_task(message* msg) ->bool;
        //启动程序的函数
        virtual void START_UP() = 0;
    };
}
#endif //MUSE_SIMULATOR_COMPUTER_HPP
