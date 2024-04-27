#include "simulator/simulator.hpp"

namespace muse::simulator{

    simulator::~simulator() = default;

    void simulator::initialize_simulator() {
        //注册主机
        if (MUSE_NETWORK_DISPATCHER::get_reference().get_host_count() <= 1){
            fmt::println("error 1, simulating the world requires more than two hosts. current host count: {}.", MUSE_NETWORK_DISPATCHER::get_reference().get_host_count());
        }
        if (MUSE_REGISTRY::get_reference().get_remote_functions() < 1){
            fmt::println("error 2, simulating the world requires more than two hosts. current host count: {}.", MUSE_NETWORK_DISPATCHER::get_reference().get_host_count());
        }
        //调用初始化方法
        const auto hosts =  MUSE_NETWORK_DISPATCHER::get_ptr()->get_hosts_list();
        //需要并行算法
        std::for_each(hosts.begin(), hosts.end(), [=](auto host_ptr){
            host_ptr->START_UP();
        });
    }

    void simulator::simulator_operating_core() {
        uint64_t ms_tick = SIMULATOR_WORLD_STATE::get_ptr()->get_tick();
        const auto hosts =  MUSE_NETWORK_DISPATCHER::get_ptr()->get_hosts_list();
        //先执行网络事件
        std::for_each(hosts.begin(), hosts.end(), [=](auto host_ptr){
            host_ptr->next_tick(ms_tick);
        });

        simulator_net_event_queue::for_each([=](simulator_event& sev)->bool {
            if (sev.event_type_ == simulator_net_event_type::RPC_REQUEST_FINISH){
                //已经完成传输需要等待服务端进行处理
                auto host = MUSE_NETWORK_DISPATCHER::get_reference().get_host(sev.message_->acceptor_id);
                if (host->get_spare_core(ms_tick)){

                }
                return true;
            } else if (sev.event_type_ == simulator_net_event_type::RPC_RESPONSE_FINISH){

                return true;
            }else {

            }
            return true;
        });

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