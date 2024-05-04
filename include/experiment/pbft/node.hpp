#ifndef MUSE_SIMULATOR_NODE_HPP
#define MUSE_SIMULATOR_NODE_HPP

#include "utils/toolkits.hpp"
#include "simulator/machines/computer.hpp"

namespace experiment::pbft{

    struct SIMULATOR_CPP_WIN_API PBFT_Node_config{
        std::string ip_address;  //IP地址

        uint64_t memory_size; //byte 内存大小

        double cpu_frequency; //hz cpu频率

        uint64_t cpu_core_number; //核心数量

        uint64_t band_width; //带宽
    };

    class SIMULATOR_CPP_WIN_API PBFT_Node : public muse::simulator::computer{
        explicit PBFT_Node(const PBFT_Node_config& conf);

        void START_UP() override;
    };
}

#endif //MUSE_SIMULATOR_NODE_HPP
