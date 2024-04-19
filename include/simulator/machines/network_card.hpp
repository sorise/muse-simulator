#ifndef MUSE_SIMULATOR_NETWORK_CARD_HPP
#define MUSE_SIMULATOR_NETWORK_CARD_HPP

#include <list>
#include <iostream>
#include <map>
#include <algorithm>
#include <numeric>
#include "utils/toolkits.hpp"
#include "message.hpp"

/*
 * @author: jx.kicker QQ: 1427035242 email: jxkicker@163.com
 * @des: 模拟主机的网卡, 传输上层得到的各种包
 * */
namespace muse::simulator {

    class SIMULATOR_CPP_WIN_API network_card_task{
    private:
        uint64_t bytes;    //发送任务大小
        uint64_t float_bytes; //流动 float_bytes
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

        [[nodiscard]] auto get_float_bytes() const -> uint64_t;
    };

    static bool operator+(const network_card_task &me, const network_card_task &other){
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

    /*
    struct SIMULATOR_CPP_WIN_API network_card_task_comparator {
        bool operator()(const network_card_task& me, const network_card_task& other) const {
            if (me.get_end_ms() < other.get_end_ms()){
                return true;
            }else if(me.get_end_ms() > other.get_end_ms()){
                return false;
            }
            return me.get_start_ms() < other.get_start_ms(); //前插入的在前面 后插入的放在后面
        }
    };
    */

    class SIMULATOR_CPP_WIN_API network_card {
    private:
        //带宽 字节为单位, Mb/s, 说明每毫秒可以发送多少字节
        uint64_t band_width_;
        uint64_t band_width_current_ms_;
        //正在发送的任务
        std::map<network_card_task, message*> sending_tasks;

        std::list<network_card_task> waiting_tasks;
    public:

        explicit network_card(const uint64_t& band_width);

        bool add_task(message* msg);

        bool get_leftover_task();
    };
}

#endif //MUSE_SIMULATOR_NETWORK_CARD_HPP
