#include "simulator/simulator_object.hpp"

namespace muse::simulator{
    void simulator_object::run() {
        //初始化
        initialize_simulator();
        while(true){
            //运行核心
            simulator_operating_core();
            //停止条件
            if (stop_simulator_condition()){
                break;
            }
            //进入下一个时隙
            MUSE_SIMULATOR_WORLD_STATE::get_ptr()->next_tick();
        }
        //生成报告
        simulator_report();
        //资源清理
        simulator_clean_up_resources();
    }

    simulator_object::simulator_object(): sequence(muse::serializer::getByteSequence()) {

    }
}