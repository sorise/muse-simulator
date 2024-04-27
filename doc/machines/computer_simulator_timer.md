

### 使用例子：
```cpp
muse::simulator::computer_simulator_timer _timer;
_timer.setInternal(1000, [](int a, int b){
    fmt::println("a + b = {}", a + b);
},10,25);

auto r= _timer.setTimeout(3000, [](int a, int b){
    fmt::println("a + b = {}", a + b);
},19,215);

auto ptr = SIMULATOR_WORLD_STATE::get_ptr();

while (SIMULATOR_WORLD_STATE::get_ptr()->get_tick() < 10000){
    _timer.runTaskLoop();
    SIMULATOR_WORLD_STATE::get_ptr()->next_tick();
}
```