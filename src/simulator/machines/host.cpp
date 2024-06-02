#include "simulator/machines/host.hpp"

namespace muse::simulator{

    auto host::get_vote() -> void {
        if (this->get_ip_address() != "215.53.2.1"){
            auto ev = new_by_pool<TransmitterEvent>("215.53.2.1",15000);
            ev->call<int>("@context/RPC:Vote");
            ev->set_callBack([&](Outcome<int> r){
                if (r.isOK()){
                    fmt::println("{}/ {} get vote {}", get_simulator_tick_ms() ,this->get_ip_address(), r.value);
                }else{
                    fmt::println("get vote failed, {} {}", (short)r.protocolReason, r.response.getReason());
                }
            });
            this->RPC_CALL(15000,ev);
        }
    }


    auto host::get_vote_two() -> void {
        if (this->get_ip_address() != "215.53.2.1"){
            auto ev = new_by_pool<TransmitterEvent>("215.53.2.1",15000);
            ev->call<int>("@context/RPC:Vote_Two", 10);
            ev->set_callBack([&](Outcome<int> r){
                if (r.isOK()){
                    fmt::println("{}/ {} get vote two {}", get_simulator_tick_ms() ,this->get_ip_address(), r.value);
                }else{
                    fmt::println("get vote two failed, {} {}", (short)r.protocolReason, r.response.getReason());
                }
            });
            this->RPC_CALL(15000,ev);
        }
    }


    auto host::vote(ipv4_address _address, uint16_t _port) -> int {
        fmt::println("{}/ {} put vote {} to {}:{}", get_simulator_tick_ms() ,this->get_ip_address(), 10, _address.to_string(), _port);
        return 10;
    }

    auto host::vote_to_other(int i, ipv4_address _address, uint16_t _port) -> int {
        fmt::println("{}/ {} put vote {} to {}:{}", get_simulator_tick_ms() ,this->get_ip_address(), 10, _address.to_string(), _port);
        return 12 + i;
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

        if (is_running){
            this->TIMER.setTimeout("TIMER/VOTE", 100, &host::get_vote_two, this);
        }
    }

    host::~host(){
        printf("this %p host disappear !\n", this);
    }
}