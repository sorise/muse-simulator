#include "simulator/simulator_object.hpp"

namespace muse::simulator{
    void simulator_object::run() {
        //初始化
        initialize_simulator();
        while(true){
            //运行核心
            simulator_operating_core();

            if (stop_simulator_condition()){
                break;
            }
            //进入下一个时隙
            SIMULATOR_WORLD_STATE::get_ptr()->next_tick();
        }
        simulator_report();
    }
}