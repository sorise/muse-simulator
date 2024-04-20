#ifndef MUSE_SIMULATOR_HOST_DELAY_MATRIX_HPP
#define MUSE_SIMULATOR_HOST_DELAY_MATRIX_HPP

#include "utils/singleton.hpp"
#include "utils/toolkits.hpp"
#include <random>
#include <ctime>
#include <unordered_map>
#include <memory>

/*
 * @author: jx.kicker QQ: 1427035242 email: jxkicker@163.com
 * @des: 完成任意主机之间的时延模拟
 * */
namespace muse::simulator {
    //时延类型
    enum class host_delay_type:uint8_t{
        Unified_Latency = 0, //统一时延
        Different_Latency = 1, //不同时延
    };

    /*
     * 时延矩阵，记录任意两个主机之间的时延。
     * 单例模式时延
     * */
    class SIMULATOR_CPP_WIN_API host_delay_matrix {
    public:
        using Latency_Map_Type = std::unordered_map<std::string, std::tuple<uint32_t,uint32_t,bool>>;
    private:
        host_delay_type type_;
        bool initial_state_; //默认还没有初始化
        uint32_t unified_latency_down_ms_; //统一时延下界
        uint32_t unified_latency_up_ms_; //统一时延上界
        bool down_equal_up_;

        std::unique_ptr<Latency_Map_Type> latency_map_;
    public:

        host_delay_matrix();

        auto initial(host_delay_type _type, const uint32_t& _unified_latency_down_ms, const uint32_t& _unified_latency_up_ms) ->bool;

        auto set_host_delay(const std::string& host_a_ip_address, const std::string& host_b_ip_address, const uint32_t& _delay_down_ms, const uint32_t& _delay_up_ms) -> void;

        auto get_delay(const std::string& host_a_ip_address, const std::string& host_b_ip_address) -> uint32_t;

        static auto get_random_number(const uint32_t& _down, const uint32_t& _up) -> uint32_t;
    };

    template class SIMULATOR_CPP_WIN_API muse::simulator::singleton_lazy_heap<muse::simulator::host_delay_matrix>;

    #define HOST_DELAY_MATRIX muse::simulator::singleton_lazy_heap<muse::simulator::host_delay_matrix>
}

#endif //MUSE_SIMULATOR_HOST_DELAY_MATRIX_HPP
