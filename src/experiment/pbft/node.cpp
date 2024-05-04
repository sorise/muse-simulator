#include "experiment/pbft/node.hpp"

namespace experiment::pbft{
    PBFT_Node::PBFT_Node(const PBFT_Node_config& config)
    :muse::simulator::computer(config.ip_address,config.memory_size, config.cpu_frequency, config.cpu_core_number, config.band_width){

    }

    void PBFT_Node::START_UP() {

    }
}