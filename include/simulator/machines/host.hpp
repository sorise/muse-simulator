//
// Created by 14270 on 2024-03-30.
//

#ifndef MUSE_SIMULATOR_HOST_HPP
#define MUSE_SIMULATOR_HOST_HPP

#include "computer.hpp"

namespace muse::simulator{
    //定义任务
    class host : public muse::simulator::computer{
        /* 需要有状态存储 */
    private:
        //状态存储
        bool is_running; //机器是否正常运行
    public:
        //行为
        auto vote() -> int;

        auto vote_hash(std::string name) -> std::string;
    };
}
#endif //MUSE_SIMULATOR_HOST_HPP
