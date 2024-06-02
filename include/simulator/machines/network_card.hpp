#include "host_delay_matrix.hpp"
#include "simulator_net_event_queue.hpp"
#include "network_card_task.hpp"

#ifndef MUSE_SIMULATOR_NETWORK_CARD_HPP
#define MUSE_SIMULATOR_NETWORK_CARD_HPP
/*
 * @author: jx.kicker QQ: 1427035242 email: jxkicker@163.com
 * @des: 模拟主机的网卡, 传输上层得到的各种包
 * */
namespace muse::simulator {
    class SIMULATOR_CPP_WIN_API network_card {
    private:
        //带宽 字节为单位, Mb/s, 说明每毫秒可以发送多少字节
        uint64_t band_width_;

        uint64_t band_width_current_ms_;

        //正在发送的任务，应该和等待发送任务队列合在一起。。。。。 实在是懒得改代码了 todo
        std::list<network_card_task> sending_tasks;
        //等待发送的任务
        std::list<network_card_task> waiting_tasks;

        std::mutex mtx_waiting_tasks_;

        //时延缓存队列, 如果
        std::multimap<uint64_t ,message*, std::less<>> latency_tasks_set;

        //响应延迟发送队列 互斥量
        std::mutex rlt_set_mtx;
        //response 延迟发送队列, 需要考虑CPU处理时间的特殊队列
        std::multimap<uint64_t, message*, std::less<>> response_latency_tasks_set;

        uint64_t real_tick_{0};
    public:
        explicit network_card(const uint64_t& band_width);

        network_card(const network_card&other) = delete;

        network_card(network_card&&other) noexcept;

        network_card& operator=(const network_card&other) = delete;

        network_card& operator=(network_card&&other) noexcept;

        /*
         * todo 需要提高性能的时候再考虑多线程问题
         * 下一个时间点
         * 用于驱动网卡去发送数据，如果发送队为空，将发送任务从等待队列中取出，加入到发送队列中
         * 需要主动运行该函数，该方法是非线程安全的，属于模拟器内部调用的函数。
         * */
        auto next_tick(const uint64_t& tick) -> void;

        bool add_task(message* msg);
    };
}

#endif //MUSE_SIMULATOR_NETWORK_CARD_HPP
