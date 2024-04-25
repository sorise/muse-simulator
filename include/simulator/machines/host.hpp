#ifndef MUSE_SIMULATOR_HOST_HPP
#define MUSE_SIMULATOR_HOST_HPP

#include "utils/toolkits.hpp"
#include "computer.hpp"

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

        auto vote() -> int;
    };
}
#endif //MUSE_SIMULATOR_HOST_HPP