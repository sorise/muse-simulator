#include <iostream>
#include <climits>
#include "simulator/simulator.hpp"

namespace muse::simulator{
    class router_program: public muse::simulator::host{
        router_program(std::string ip_address, const uint64_t& memory_size, const double& cpu_frequency, const uint64_t& cpu_core_number, const uint64_t& _band_width)
        : host(std::move(ip_address), memory_size, cpu_frequency, cpu_core_number, _band_width)
        {

        }
    };
}

int main() {
    MUSE_CPU_PROCESSING_MATRIX::get_ptr()->initial(100, 12000);
    MUSE_HOST_DELAY_MATRIX::get_ptr()->initial(muse::simulator::host_delay_type::Different_Latency, 100, 100);

    auto *A1 = muse::simulator::new_by_pool<muse::simulator::host>(
        "215.56.21.1", UINT32_MAX, 3.5, 8,2621440ull
    );
    auto *A2 = muse::simulator::new_by_pool<muse::simulator::host>(
         "215.56.21.2", UINT32_MAX, 3.5, 8,2621440ull
    );

    MUSE_NETWORK_DISPATCHER::get_reference().register_host(A1);
    MUSE_NETWORK_DISPATCHER::get_reference().register_host(A2);

    muse::simulator::simulator world;
    world.run();
    return 0;
}
