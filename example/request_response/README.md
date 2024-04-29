## [例子1： 主机A 请求 主机B的数据](#)
**介绍**: 模拟器使用介绍...

----
主机A 有一个RPC接口 `RPC/Get_Infomation`，返回当前A的信息，主机B通过该接口进行请求。


```cpp
#include <iostream>
#include <climits>
#include <map>

#include "utils/singleton.hpp"
#include "simulator/simulator.hpp"
#include "simulator/machines/host.hpp"
#include "simulator/machines/registry.hpp"
#include "simulator/machines/computer_simulator_timer.hpp"

//通用成员函数包装器
int main() {

    MUSE_CPU_PROCESSING_MATRIX::get_ptr()->initial(20, 40);

    MUSE_CPU_PROCESSING_MATRIX::get_ptr()->set_RPC_processing_time_Server(" RPC:Vote",20, 40);
    MUSE_CPU_PROCESSING_MATRIX::get_ptr()->set_RPC_processing_time_Server(" RPC:Vote",10, 10);
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

    //运行模拟器
    _simulator.run();

    return 0;
}
```


`host 类` 声明部分。

```cpp
#ifndef MUSE_SIMULATOR_HOST_HPP
#define MUSE_SIMULATOR_HOST_HPP

#include <utility>

#include "computer.hpp"
#include "utils/toolkits.hpp"
#include "simulator/simulator_world_state.hpp"

namespace muse::simulator{
    //定义任务
    class SIMULATOR_CPP_WIN_API host : public muse::simulator::computer{
        /* 需要有状态存储 */
    private:
        //状态存储
        bool is_running; //机器是否正常运行
        //执行网络传输
    public:
        host();

        host(std::string ip_address, const uint64_t& memory_size, const double& cpu_frequency, const uint64_t& cpu_core_number, const uint64_t& _band_width);
        //行为
        auto get_vote() -> void;

        auto vote() -> int;

        void START_UP() override;

        ~host () override;
    };
}
#endif //MUSE_SIMULATOR_HOST_HPP
```

`host 类` 定义部分。
```cpp
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
```