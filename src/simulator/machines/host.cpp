#include <utility>

#include "simulator/machines/host.hpp"

namespace muse::simulator{

    auto host::get_vote() -> void {
        auto ev = new_by_pool<TransmitterEvent>("215.56.21.2",15000);
        ev->call<int>("rpc::vote");
        ev->set_callBack([&](Outcome<int> r){
            if (r.isOK()){
                fmt::println("vote {}", r.value);
            }else{
                fmt::println("get vote failed, {} {}", (short)r.protocolReason, r.response.getReason());
            }
        });
        RPC(ev);
    }

    auto host::vote() -> int {
        return 10;
    }

    host::host(std::string ip_address, const uint64_t &memory_size, const double &cpu_frequency,const uint64_t &cpu_core_number, const uint64_t& _band_width)
    :computer(std::move(ip_address), memory_size, cpu_frequency, cpu_core_number,_band_width), is_running(true) {

    }

    host::host():computer(), is_running(true) {

    }

    void host::START_UP() {
        printf("host start up\n");
    }

    host::~host() {
        printf("host disappear !\n");
    }
}