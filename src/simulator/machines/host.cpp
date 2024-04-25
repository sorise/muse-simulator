#include <utility>

#include "simulator/machines/host.hpp"

namespace muse::simulator{

    auto host::vote() -> int {
        auto ev = new_by_pool<TransmitterEvent>("127.0.0.1",15000);
        ev->call<int>("rpc::vote",12);
        ev->set_callBack([&](Outcome<int> r){

        });
        RPC(ev);
        return 5;
    }

    auto host::vote_hash(std::string name) -> std::string {
        return "val";
    }

    auto host::add(int a, int b) -> int {
        return a + b;
    }

    host::host(std::string ip_address, const uint64_t &memory_size, const double &cpu_frequency,const uint64_t &cpu_core_number, const uint64_t& _band_width)
    :computer(std::move(ip_address), memory_size, cpu_frequency, cpu_core_number,_band_width), is_running(true) {

    }

    host::host():computer(), is_running(true) {

    }
}