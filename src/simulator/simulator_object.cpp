#include "simulator/simulator_object.hpp"

namespace muse::simulator{
    void simulator_object::run() {
        //初始化
        initialize_simulator();
        while(true){
            if ( MUSE_SIMULATOR_WORLD_STATE::get_ptr()->get_tick() == 1001){
                int i = 12;
            }
            //运行核心
            simulator_operating_core();
            //停止条件
            if (stop_simulator_condition()){
                break;
            }
            //进入下一个时隙
            MUSE_SIMULATOR_WORLD_STATE::get_ptr()->next_tick();
        }
        simulator_report();
    }
}