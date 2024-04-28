#include <list>
#include <iostream>
#include <map>
#include <algorithm>
#include <numeric>

#include "utils/toolkits.hpp"
#include "message.hpp"

#ifndef MUSE_SIMULATOR_NETWORK_CARD_TASK_HPP
#define MUSE_SIMULATOR_NETWORK_CARD_TASK_HPP

namespace muse::simulator {
    class SIMULATOR_CPP_WIN_API network_card_task{
    private:
        uint64_t bytes;    //发送任务大小
        uint64_t send_bytes; //已经发送了多少字节
        uint64_t start_ms; //开始发送时间
        uint64_t end_ms;   //结束发送时间
        message* msg;
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

        auto append_send_bytes(const uint64_t& _bytes) ->void;

        [[nodiscard]] auto get_left_bytes() const -> uint64_t;
    };

    static uint64_t operator+(const network_card_task &me, const network_card_task &other);

    static bool operator<(const network_card_task &me, const network_card_task &other);
}
#endif //MUSE_SIMULATOR_NETWORK_CARD_TASK_HPP
