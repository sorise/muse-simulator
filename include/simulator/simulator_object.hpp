#ifndef MUSE_SIMULATOR_SIMULATOR_OBJECT_HPP
#define MUSE_SIMULATOR_SIMULATOR_OBJECT_HPP

#include "simulator_world_state.hpp"
#include <cstdint>

namespace muse::simulator {

    //组件协作模式
    class SIMULATOR_CPP_WIN_API simulator_object {
    public:
        //模拟器运行
        void run();

        virtual ~simulator_object() = default;

        //运行核心逻辑
        virtual void simulator_operating_core() = 0;

        //初始化模拟器的相关状态
        virtual void initialize_simulator() = 0;

        //什么时候停止模拟器
        virtual bool stop_simulator_condition() = 0;

        //模拟器运行结果报表生产
        virtual void simulator_report() = 0;
    };
}

#endif //MUSE_SIMULATOR_SIMULATOR_OBJECT_HPP
