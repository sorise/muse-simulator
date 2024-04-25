#ifndef MUSE_SIMULATOR_CPU_PROCESSING_MATRIX_HPP
#define MUSE_SIMULATOR_CPU_PROCESSING_MATRIX_HPP

#include "utils/toolkits.hpp"
#include "utils/singleton.hpp"
#include <random>
#include <ctime>
#include <unordered_map>
#include <memory>

namespace muse::simulator {
    /*
     * @CPU 处理时间矩阵，规定用户主机处理RPC请求的CPU耗时
     * @param: name: RPC方法名称
     * @param: _unified_cpu_down_ms: 统一时延下界 时间单位是  微妙
     * @param: _unified_cpu_up_ms: 统一时延上界             微妙
     * @param: down_equal_up: 统一时延下界是否等于上界
     * */
    class SIMULATOR_CPP_WIN_API cpu_processing_matrix{
    public:
        using CPU_Map_Type = std::unordered_map<std::string, std::tuple<uint32_t,uint32_t,bool>>;
    private:
        bool initial_state_; //默认还没有初始化
        uint32_t unified_cpu_down_us_; //统一时延下界
        uint32_t unified_cpu_up_us_; //统一时延上界
        bool down_equal_up_;
        std::unique_ptr<CPU_Map_Type> cpu_map_ {nullptr};
        static auto get_random_number(const uint32_t& _down, const uint32_t& _up) -> uint32_t;
    public:
        cpu_processing_matrix();

        /* RPC方法处理时间 */
        auto set_RPC_processing_time_Client(const std::string& name, const uint32_t& _unified_cpu_down_us, const uint32_t& _unified_cpu_up_us) -> void;

        auto set_RPC_processing_time_Server(const std::string& name, const uint32_t& _unified_cpu_down_us, const uint32_t& _unified_cpu_up_us) -> void;

        auto initial(const uint32_t &_unified_down_us,const uint32_t &_unified_up_us) ->bool;

        auto get_client(const std::string& name)->uint32_t ;

        auto get_server(const std::string& name)->uint32_t ;
    };

    template class SIMULATOR_CPP_WIN_API singleton_lazy_heap<cpu_processing_matrix>;
}

#define MUSE_CPU_PROCESSING_MATRIX muse::simulator::singleton_lazy_heap<muse::simulator::cpu_processing_matrix>

#endif //MUSE_SIMULATOR_CPU_PROCESSING_MATRIX_HPP
