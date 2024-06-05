#include <iostream>
#include <climits>
#include <cassert>
#include <vector>
#include <map>

#include "simulator/utils/singleton.hpp"
#include "simulator/simulator.hpp"
#include "simulator/machines/host.hpp"
#include "simulator/machines/registry.hpp"
#include "simulator/machines/ipv4_address.hpp"
#include "simulator/machines/computer_simulator_timer.hpp"

#include "simulator/encryption/ecc_secp256k1.hpp"
#include "simulator/encryption/bitcoin_secp256k1.hpp"

//#include "mcl/bn_c384_256.h"
//#include "bls/bls.h"

int main(int argc, char *argv[]) {
    MUSE_CPU_PROCESSING_MATRIX::get_ptr()->initial(20, 40);

    MUSE_CPU_PROCESSING_MATRIX::get_ptr()->set_RPC_processing_time_Server("@context/RPC:Vote",20, 40);
    MUSE_CPU_PROCESSING_MATRIX::get_ptr()->set_RPC_processing_time_Client("@context/RPC:Vote",10, 10);

    MUSE_CPU_PROCESSING_MATRIX::get_ptr()->set_RPC_processing_time_Client("@context/RPC:Vote_Two",10, 10);
    MUSE_CPU_PROCESSING_MATRIX::get_ptr()->set_RPC_processing_time_Client("@context/RPC:Vote_Two",10, 10);

    MUSE_CPU_PROCESSING_MATRIX::get_ptr()->set_function_processing_time("TIMER/VOTE",100, 100);

    MUSE_HOST_DELAY_MATRIX::get_ptr()->initial(muse::simulator::host_delay_type::Different_Latency, 10, 20);

    auto A1 = muse::simulator::new_by_pool_share<muse::simulator::host>(
            "215.53.2.1", UINT32_MAX, 3.5, 8,2621440ull
    );
    auto A2 = muse::simulator::new_by_pool_share<muse::simulator::host>(
            "215.53.2.2", UINT32_MAX, 3.5, 8,2621440ull
    );

    MUSE_NETWORK_DISPATCHER::get_reference().register_host(A1.get());
    MUSE_NETWORK_DISPATCHER::get_reference().register_host(A2.get());

    //注册方法
    MUSE_REGISTRY::get_reference().Bind("@context/RPC:Vote",&muse::simulator::host::vote);
    MUSE_REGISTRY::get_reference().Bind("@context/RPC:Vote_Two",&muse::simulator::host::vote_to_other);

    muse::simulator::simulator _simulator;

    _simulator.set_stop_condition([]()->bool {
        if (MUSE_SIMULATOR_WORLD_STATE::get_ptr()->get_tick() > 1000){
            return true;
        }
        return false;
    });

    //运行模拟器
    _simulator.run();
    return 0;
}