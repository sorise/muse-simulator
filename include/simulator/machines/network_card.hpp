#ifndef MUSE_SIMULATOR_NETWORK_CARD_HPP
#define MUSE_SIMULATOR_NETWORK_CARD_HPP

#include <list>
#include <iostream>
#include <map>
#include "utils/toolkits.hpp"

/*
 * @author: jx.kicker QQ: 1427035242 email: jxkicker@163.com
 * @des: 模拟主机的网卡, 传输上层得到的各种包
 * */
namespace muse::simulator {

    class SIMULATOR_CPP_WIN_API network_card_task{
    private:
        uint64_t bytes;  //发送任务大小
        uint64_t start_ms; //开始发送时间
        uint64_t end_ms;   //预计结束时间
    public:
        network_card_task(uint64_t _bytes, uint64_t _start_ms, uint64_t _end_ms);

        [[nodiscard]] uint64_t get_start_ms() const;

        [[nodiscard]] auto get_end_ms() const -> uint64_t;

        void set_start_ms(const uint64_t& _start_ms);

        void set_end_ms(const uint64_t& _end_ms);
    };


    static bool operator < (const network_card_task &me, const network_card_task &other){
        if (me.get_end_ms() < other.get_end_ms()){
            return true;
        }else if(me.get_end_ms() > other.get_end_ms()){
            return false;
        }
        return me.get_start_ms() < other.get_start_ms(); //前插入的在前面 后插入的放在后面
    }

    class SIMULATOR_CPP_WIN_API network_card {
    private:
        uint64_t band_width_; //带宽 字节为单位

        //正在发送的任务
        std::map<network_card_task, bool> sending_tasks;

    public:
        explicit network_card(const uint64_t& band_width);

    };
}

#endif //MUSE_SIMULATOR_NETWORK_CARD_HPP
