#ifndef MUSE_SIMULATOR_NETWORK_DISPATCHER_HPP
#define MUSE_SIMULATOR_NETWORK_DISPATCHER_HPP
#include "../utils/toolkits.hpp"
#include "machines/host.hpp"
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include "../utils/singleton.hpp"
#include "machines/transmitter_event.hpp"
#include "machines/message.hpp"
#include "machines/network_card.hpp"

namespace muse::simulator {
    /* 网络调度器
     * 需要有一个网络节点列表，一个事件队列，一个事件队列锁
     * */
    class SIMULATOR_CPP_WIN_API network_dispatcher {
    private:
        //那些已经注册了的主机  ip+端口
        std::unordered_map<std::string, muse::simulator::host*> hosts;
        std::shared_mutex mtx;
        //事件队列 ， RPC Server 触发、 RPC Client 回调函数触发
    public:
        bool register_host(muse::simulator::host* host_ptr);

        bool RPC_Request(host*caller, TransmitterEvent &&event);
    };
}

#endif //MUSE_SIMULATOR_NETWORK_DISPATCHER_HPP

