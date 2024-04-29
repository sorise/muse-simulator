#ifndef MUSE_SIMULATOR_SIMULATOR_WORLD_STATE_HPP
#define MUSE_SIMULATOR_SIMULATOR_WORLD_STATE_HPP

#include <cstdint>
#include "utils/singleton.hpp"

namespace muse::simulator{
    class SIMULATOR_CPP_WIN_API simulator_world_state {
    private:
        uint64_t tick_ms; //毫秒
    public:
        simulator_world_state();

        //进行更新
        void next_tick();

        //返回 milliseconds 毫秒
        [[nodiscard]] uint64_t get_tick() const;

        //重置世界状态
        void reset_simulator_world();
    };

    template class  SIMULATOR_CPP_WIN_API muse::simulator::singleton_lazy_heap<muse::simulator::simulator_world_state>;

    #define MUSE_SIMULATOR_WORLD_STATE muse::simulator::singleton_lazy_heap<muse::simulator::simulator_world_state>

    static auto get_simulator_tick_ms() -> uint64_t{
        return MUSE_SIMULATOR_WORLD_STATE::get_ptr()->get_tick();
    }
}
#endif //MUSE_SIMULATOR_SIMULATOR_WORLD_STATE_HPP
