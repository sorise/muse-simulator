#include <iostream>
#include <climits>
#include <map>

#include "utils/singleton.hpp"
#include "simulator/simulator.hpp"
#include "simulator/machines/host.hpp"
#include "simulator/machines/registry.hpp"
#include "simulator/machines/ipv4_address.hpp"
#include "simulator/machines/computer_simulator_timer.hpp"

void test(){
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

};

[[maybe_unused]] uint32_t string_to_ipv4(const std::string& ip){
    uint32_t result = 0; //主机字节序
    auto ip_len = ip.size();
    if (ip_len < 7){
        throw std::logic_error("the argument ip is empty.");
    }
    if (ip_len > 15){
        throw std::logic_error("the argument ip is too long.");
    }
    uint32_t last_8byte = 0xFF; //截取后面
    uint32_t segment_value = 0;
    int gap = 0;

    for (int i = 0; i < ip_len; ++i) {
        if (ip[i] == '.'){
            if (gap > 3){
                throw std::logic_error("error the argument is too long.");
            }
            if (segment_value > 0xFF) {
                throw std::logic_error("error the argument is too long.");
            }
            gap++;
            result = result | segment_value;
            result = result << 8;
            segment_value = 0;
        }else if (ip[i] >= '0' && ip[i] <= '9') {
            segment_value = segment_value * 10 + (ip[i] - '0');
        } else {
            throw std::logic_error(std::string("illegal characters."));
        }
    }
    if (gap != 3) {
        throw std::logic_error("content error.");
    }
    result = result | segment_value;
    return result;
}

[[maybe_unused]] std::string ipv4_to_string(uint32_t _ip_value){
    std::string result;
    for(int i = 3; i >= 0; --i) {
        uint32_t segment = (_ip_value >> (i * 8)) & 0xFF;
        if(!result.empty()) {
            result += ".";
        }
        result += std::to_string(segment);
    }
    return result;
}


int main() {

    try {
        std::string val = "10.1.121.11";
        muse::simulator::ipv4_address ip(val);

        std::cout << std::boolalpha << ip.is_private_net_address() << std::endl;
        std::cout << std::boolalpha << ip.to_string() << std::endl;

    } catch (const std::logic_error &ler) {
        fmt::println("error logic: {}", ler.what());
    }
    return 0;
}
