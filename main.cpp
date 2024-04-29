#include <iostream>
#include <climits>
#include <map>

#include "utils/singleton.hpp"
#include "simulator/simulator.hpp"
#include "simulator/machines/host.hpp"
#include "simulator/machines/registry.hpp"
#include "simulator/machines/computer_simulator_timer.hpp"

//通用成员函数包装器
int main() {

    MUSE_CPU_PROCESSING_MATRIX::get_ptr()->initial(20, 40);

    MUSE_CPU_PROCESSING_MATRIX::get_ptr()->set_RPC_processing_time_Server("RPC:Vote",20, 40);
    MUSE_CPU_PROCESSING_MATRIX::get_ptr()->set_RPC_processing_time_Client("RPC:Vote",10, 10);
    MUSE_CPU_PROCESSING_MATRIX::get_ptr()->set_function_processing_time("TIMER/VOTE",100, 100);

    MUSE_HOST_DELAY_MATRIX::get_ptr()->initial(muse::simulator::host_delay_type::Different_Latency, 10, 20);

    auto A1 = muse::simulator::new_by_pool_share<muse::simulator::host>(
            "215.53.2.1", UINT32_MAX, 3.5, 8,2621440ull
    );

    MUSE_NETWORK_DISPATCHER::get_reference().register_host(A1);

    auto A2 = muse::simulator::new_by_pool_share<muse::simulator::host>(
            "215.53.2.2", UINT32_MAX, 3.5, 8,2621440ull
    );

    MUSE_NETWORK_DISPATCHER::get_reference().register_host(A2);

    //注册方法
    MUSE_REGISTRY::get_reference().Bind("RPC:Vote",&muse::simulator::host::vote);

    muse::simulator::singleton_thread_pool::get_ptr()->taskSize();

    muse::simulator::simulator _simulator;

    _simulator.set_stop_condition([]()->bool {
        if (MUSE_SIMULATOR_WORLD_STATE::get_ptr()->get_tick() > 500){
            return true;
        }
        return false;
    });

    //运行模拟器
    _simulator.run();

    return 0;
}
