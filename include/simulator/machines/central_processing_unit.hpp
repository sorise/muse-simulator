#ifndef MUSE_SIMULATOR_CENTRAL_PROCESSING_UNIT_HPP
#define MUSE_SIMULATOR_CENTRAL_PROCESSING_UNIT_HPP

#include "utils/singleton.hpp"
#include "utils/toolkits.hpp"
#include <iostream>
#include <queue>

namespace muse::simulator {

    /*
     * @author: jx.kicker QQ: 1427035242 email: jxkicker@163.com
     * @des: 模拟主机的CPU
     * */
    class SIMULATOR_CPP_WIN_API central_processing_unit {
    private:
        /* 多少个核心 n核2n线程 */
        uint32_t core_num_;
        std::priority_queue<uint64_t> core_load_;
    public:
        explicit central_processing_unit(const uint32_t& _core_num);

        int32_t get_spare_core(const uint64_t& tick);

        ~central_processing_unit() = default;
    };
}

#endif //MUSE_SIMULATOR_CENTRAL_PROCESSING_UNIT_HPP
