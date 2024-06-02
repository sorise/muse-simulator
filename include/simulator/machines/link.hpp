#ifndef MUSE_SIMULATOR_LINK_HPP
#define MUSE_SIMULATOR_LINK_HPP
#include "../utils/toolkits.hpp"
#include <cstdint>

namespace muse::simulator {
    /*
     * 表示一个主机的网卡，存储发送任务
     * */
    class SIMULATOR_CPP_WIN_API link {
    private:
        uint64_t _bandwidth; //带宽
        uint64_t _latency_down = 0; //延迟下界
        uint64_t _latency_up = 0; //延迟上界
        
        uint64_t _left_bandwidth;
    public:
        link();
    };
}

#endif //MUSE_SIMULATOR_LINK_HPP
