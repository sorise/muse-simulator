#include "simulator/machines/host.hpp"

namespace muse::simulator{

    auto host::get_vote() -> void {
        if (this->get_ip_address() != "215.53.2.1"){
            auto ev = new_by_pool<TransmitterEvent>("215.53.2.1",15000);
            ev->call<int>("RPC:Vote");
            ev->set_callBack([&](Outcome<int> r){
                if (r.isOK()){
                    fmt::println("{}/ {} get vote {}", get_simulator_tick_ms() ,this->get_ip_address(), r.value);
                }else{
                    fmt::println("get vote failed, {} {}", (short)r.protocolReason, r.response.getReason());
                }
            });
            this->RPC_CALL(ev);
        }
    }

    auto host::vote() -> int {
        fmt::println("{}/ {} put vote {}", get_simulator_tick_ms() ,this->get_ip_address(), 10);
        return 10;
    }

    host::host(std::string ip_address, const uint64_t &memory_size, const double &cpu_frequency,const uint64_t &cpu_core_number, const uint64_t& _band_width)
    :computer(std::move(ip_address), memory_size, cpu_frequency, cpu_core_number,_band_width), is_running(true) {

    }

    host::host():computer(), is_running(true) {

    }

    void host::START_UP() {
        if (is_running){
            this->TIMER.setTimeout("TIMER/VOTE", 1, &host::get_vote, this);
        }
    }

    host::~host(){
        printf("host disappear !\n");
    }
}