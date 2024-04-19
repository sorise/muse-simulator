#ifndef MUSE_SIMULATOR_NETWORK_CARD_HPP
#define MUSE_SIMULATOR_NETWORK_CARD_HPP

#include <list>
#include <iostream>
#include <map>
#include <algorithm>
#include <numeric>
#include "simulator_net_event_queue.hpp"
#include "utils/toolkits.hpp"
#include "host_delay_matrix.hpp"
#include "message.hpp"

/*
 * @author: jx.kicker QQ: 1427035242 email: jxkicker@163.com
 * @des: 模拟主机的网卡, 传输上层得到的各种包
 * */
namespace muse::simulator {

    class SIMULATOR_CPP_WIN_API network_card_task{
    private:
        uint64_t bytes;    //发送任务大小
        uint64_t send_bytes; //已经发送了多少字节
        uint64_t start_ms; //开始发送时间
        uint64_t end_ms;   //预计结束时间
        message* msg;      //
    public:
        network_card_task(message *msg, uint64_t _bytes, uint64_t _start_ms, uint64_t _end_ms);

        [[nodiscard]] uint64_t get_start_ms() const;

        [[nodiscard]] auto get_end_ms() const -> uint64_t;

        void set_start_ms(const uint64_t& _start_ms);

        void set_end_ms(const uint64_t& _end_ms);

        void set_message(message* _msg);

        [[nodiscard]] auto get_message() -> message*;

        uint64_t operator+(const network_card_task &other) const;

        [[nodiscard]] auto get_bytes() const -> uint64_t;

        [[nodiscard]] auto get_send_bytes() const -> uint64_t;

        [[nodiscard]] auto append_send_bytes(const uint64_t& _bytes);

        [[nodiscard]] auto get_left_bytes() const -> uint64_t ;
    };

    static uint64_t operator+(const network_card_task &me, const network_card_task &other){
        return me.get_bytes() + other.get_bytes();
    }

    static bool operator<(const network_card_task &me, const network_card_task &other){
        if (me.get_end_ms() < other.get_end_ms()){
            return true;
        }else if(me.get_end_ms() > other.get_end_ms()){
            return false;
        }
        return me.get_start_ms() < other.get_start_ms(); //前插入的在前面 后插入的放在后面
    }

    class SIMULATOR_CPP_WIN_API network_card {
    private:
        //带宽 字节为单位, Mb/s, 说明每毫秒可以发送多少字节
        uint64_t band_width_;
        uint64_t band_width_current_ms_;
        //正在发送的任务
        std::map<network_card_task, message*> sending_tasks;

        std::list<network_card_task> waiting_tasks;

        std::map<uint64_t ,message*, std::less<>> waiting_tasks_set;

        uint64_t real_tick_{0};

        uint64_t last_update_ms_{0};
    public:

        explicit network_card(const uint64_t& band_width);

        /* 下一个时间点
         * 用于驱动网卡去发送数据，如果发送队为空，将发送任务从等待队列中取出，加入到发送队列中
         * 需要主动运行该函数，该方法是非线程安全的，属于模拟器内部调用的函数。
         * todo 需要提高性能的时候再考虑多线程问题
         * */
        auto next_tick(const uint64_t& tick) -> void;

        bool add_task(message* msg);

        bool get_leftover_task();
    };
}

#endif //MUSE_SIMULATOR_NETWORK_CARD_HPP
