
### 例子:

```cpp
MUSE_CPU_PROCESSING_MATRIX::get_ptr()->initial(20, 40);

MUSE_CPU_PROCESSING_MATRIX::get_ptr()->set_RPC_processing_time_Server("@context/RPC:Vote",20, 40);
MUSE_CPU_PROCESSING_MATRIX::get_ptr()->set_RPC_processing_time_Client("@context/RPC:Vote",10, 10);

MUSE_CPU_PROCESSING_MATRIX::get_ptr()->set_RPC_processing_time_Client("@context/RPC:Vote_Two",10, 10);
MUSE_CPU_PROCESSING_MATRIX::get_ptr()->set_RPC_processing_time_Client("@context/RPC:Vote_Two",10, 10);

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
MUSE_REGISTRY::get_reference().Bind("@context/RPC:Vote",&muse::simulator::host::vote);
MUSE_REGISTRY::get_reference().Bind("@context/RPC:Vote_Two",&muse::simulator::host::vote_to_other);

muse::simulator::singleton_thread_pool::get_ptr()->taskSize();

muse::simulator::simulator _simulator;

_simulator.set_stop_condition([]()->bool {
    if (MUSE_SIMULATOR_WORLD_STATE::get_ptr()->get_tick() > 1000){
        return true;
    }
    return false;
});

//运行模拟器
_simulator.run();
```


### 使用例子：
```cpp
muse::simulator::computer_simulator_timer _timer;
_timer.setInternal(1000, [](int a, int b){
    fmt::println("a + b = {}", a + b);
},10,25);

auto r= _timer.setTimeout(3000, [](int a, int b){
    fmt::println("a + b = {}", a + b);
},19,215);

auto ptr = MUSE_SIMULATOR_WORLD_STATE::get_ptr();

while (MUSE_SIMULATOR_WORLD_STATE::get_ptr()->get_tick() < 10000){
    _timer.runTaskLoop();
    MUSE_SIMULATOR_WORLD_STATE::get_ptr()->next_tick();
}
```