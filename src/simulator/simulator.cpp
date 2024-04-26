#include "simulator/simulator.hpp"

namespace muse::simulator{

    simulator::~simulator() = default;

    void simulator::initialize_simulator() {
        //注册主机
        if (MUSE_NETWORK_DISPATCHER::get_reference().get_host_count() <= 1){
            fmt::println("error 1, simulating the world requires more than two hosts. current host count: {}.", MUSE_NETWORK_DISPATCHER::get_reference().get_host_count());
        }
        if (MUSE_SYNCHRONOUS_REGISTRY::get_reference().get_remote_functions() < 1){
            fmt::println("error 2, simulating the world requires more than two hosts. current host count: {}.", MUSE_NETWORK_DISPATCHER::get_reference().get_host_count());
        }
        //调用初始化方法
        const auto hosts =  MUSE_NETWORK_DISPATCHER::get_ptr()->get_hosts_list();
        std::for_each(hosts.begin(), hosts.end(), [=](auto host_ptr){
            host_ptr->START_UP();
        });
    }

    void simulator::simulator_operating_core() {
        uint64_t ms_tick = SIMULATOR_WORLD_STATE::get_ptr()->get_tick();
        const auto hosts =  MUSE_NETWORK_DISPATCHER::get_ptr()->get_hosts_list();
        std::for_each(hosts.begin(), hosts.end(), [=](auto host_ptr){
            host_ptr->next_tick(ms_tick);
        });
        //处理网络事件
        if (simulator_net_event_queue::is_empty()){
            //处理网络事件
            bool success = false;
            simulator_event ev = simulator_net_event_queue::pop_event(success);
            if (ev.event_type_ == simulator_net_event_type::RPC_REQUEST_FINISH){

            } else if (ev.event_type_ == simulator_net_event_type::RPC_RESPONSE_FINISH){

            }else {

            }
        }

    }

    bool simulator::stop_simulator_condition() {
        return true;
    }

    void simulator::simulator_report() {
        fmt::println("simulator report");
    }

    void simulator::simulator_clean_up_resources() {

    }
}