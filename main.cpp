#include <iostream>
#include <climits>
#include "simulator/simulator.hpp"
#include "simulator/machines/registry.hpp"

struct Base{

    virtual ~Base() = default;
};

struct Son: public Base{
    ~Son() override = default;

    int func(int a, int b){
        return a + b;
    };
};

template<std::size_t... Index>
constexpr std::size_t test() {
    return (Index + ...);
}

//通用成员函数包装器

int main() {
    MUSE_CPU_PROCESSING_MATRIX::get_ptr()->initial(100, 12000);
    MUSE_HOST_DELAY_MATRIX::get_ptr()->initial(muse::simulator::host_delay_type::Different_Latency, 100, 100);

    static_assert(test<1,2,3>() == 6); ;

/*
    auto *A1 = muse::simulator::new_by_pool<muse::simulator::host>(
        "215.56.21.1", UINT32_MAX, 3.5, 8,2621440ull
    );

    auto *A2 = muse::simulator::new_by_pool<muse::simulator::host>(
         "215.56.21.2", UINT32_MAX, 3.5, 8,2621440ull
    );

    //注册方法
    MUSE_SYNCHRONOUS_REGISTRY::get_reference().Bind("RPC:Vote",&muse::simulator::host::vote);

    //注册主机
    MUSE_NETWORK_DISPATCHER::get_reference().register_host(A1);
    MUSE_NETWORK_DISPATCHER::get_reference().register_host(A2);

    muse::simulator::simulator _simulator;
    //运行模拟器
    _simulator.run();
*/
    return 0;
}
