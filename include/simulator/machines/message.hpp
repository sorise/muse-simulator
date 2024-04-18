#ifndef MUSE_SIMULATOR_MESSAGE_HPP
#define MUSE_SIMULATOR_MESSAGE_HPP

#include "utils/toolkits.hpp"
#include "transmitter_event.hpp"

#include <cstdint>
#include <string>

namespace muse::simulator{
    struct SIMULATOR_CPP_WIN_API message{
        uint64_t cpu_ms;           //处理时间 ms
        std::string sender_ip;     //发送者 id
        std::string acceptor_id;   //接受者 id
        uint64_t bytes;            //大小、字节

        TransmitterEvent event; /* 等待触发的事件 */
        bool is_trigger;

        message(TransmitterEvent && _event,const uint64_t& _cpu_ms, std::string _sender_ip);
        message(const message& other) = delete;
    };
}

#endif //MUSE_SIMULATOR_MESSAGE_HPP
