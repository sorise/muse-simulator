#ifndef MUSE_SIMULATOR_MESSAGE_RPC_PHASE_HPP
#define MUSE_SIMULATOR_MESSAGE_RPC_PHASE_HPP

#include <cstdint>

namespace muse::simulator {
    enum class message_rpc_phase:uint8_t {
        RPC_REQUEST = 0,
        RPC_RESPONSE,
        RPC_FINISH,
    };
}

#endif //MUSE_SIMULATOR_MESSAGE_RPC_PHASE_HPP
