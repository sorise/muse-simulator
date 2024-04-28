#include <iostream>
#include <climits>
#include <map>

#include "utils/singleton.hpp"
#include "simulator/simulator.hpp"
#include "simulator/machines/host.hpp"
#include "simulator/machines/registry.hpp"
#include "simulator/machines/computer_simulator_timer.hpp"

struct Base{

    Base(){
        std::cout << "Base Constructor" << std::endl;
    }

    virtual ~Base(){
        std::cout << "Base Destructor" << std::endl;
    };
};

struct A:public Base{
    A(){
        std::cout << "A Constructor" << std::endl;
    }

    int add(int a, int b){
        return  a + b;
    }

    ~A() override{
        std::cout << "A Destructor" << std::endl;
    }
};

//通用成员函数包装器
int main() {
    MUSE_CPU_PROCESSING_MATRIX::get_ptr()->initial(100, 12000);
    MUSE_HOST_DELAY_MATRIX::get_ptr()->initial(muse::simulator::host_delay_type::Different_Latency, 100, 100);

    for (int i = 0; i < 2506; ++i) {
        std::string ip = "215.53.2." + std::to_string(i);
        auto A1 = muse::simulator::new_by_pool_share<muse::simulator::host>(
                ip, UINT32_MAX, 3.5, 8,2621440ull
        );
        MUSE_NETWORK_DISPATCHER::get_reference().register_host(A1);
    }

    //注册方法
    MUSE_REGISTRY::get_reference().Bind("RPC:Vote",&muse::simulator::host::vote);

    muse::simulator::singleton_thread_pool::get_ptr()->taskSize();

    muse::simulator::simulator _simulator;
    //运行模拟器
    _simulator.run();

    return 0;
}
