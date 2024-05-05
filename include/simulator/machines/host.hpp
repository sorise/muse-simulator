#ifndef MUSE_SIMULATOR_HOST_HPP
#define MUSE_SIMULATOR_HOST_HPP

#include <utility>

#include "ipv4_address.hpp"
#include "computer.hpp"
#include "utils/toolkits.hpp"
#include "simulator/simulator_world_state.hpp"

namespace muse::simulator{
    //定义任务
    class SIMULATOR_CPP_WIN_API host : public muse::simulator::computer{
        /* 需要有状态存储 */
    private:
        //状态存储
        bool is_running; //机器是否正常运行
        //执行网络传输
    public:
        host();

        host(std::string ip_address, const uint64_t& memory_size, const double& cpu_frequency, const uint64_t& cpu_core_number, const uint64_t& _band_width);
        //行为
        auto get_vote() -> void;

        auto get_vote_two() -> void;

        auto vote(ipv4_address _address, uint16_t _port) -> int;

        auto vote_to_other(int i, ipv4_address _address, uint16_t _port) -> int;

        void START_UP() override;

        ~host () override;
    };
}
#endif //MUSE_SIMULATOR_HOST_HPP