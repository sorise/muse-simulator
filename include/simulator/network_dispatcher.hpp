#ifndef MUSE_SIMULATOR_NETWORK_DISPATCHER_HPP
#define MUSE_SIMULATOR_NETWORK_DISPATCHER_HPP

#include "../utils/toolkits.hpp"
#include "../utils/singleton.hpp"

#include <vector>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <unordered_map>

#include "machines/host.hpp"
#include "machines/transmitter_event.hpp"
#include "machines/message.hpp"
#include "machines/network_card.hpp"

namespace muse::simulator {
    /*
     * @网络调度器 需要有一个网络节点列表，一个事件队列，一个事件队列锁
     * */
    class SIMULATOR_CPP_WIN_API network_dispatcher {
    private:
        //那些已经注册了的主机  ip+端口
        std::unordered_map<std::string, muse::simulator::host*> hosts;
        std::shared_mutex mtx;
        //事件队列 ， RPC Server 触发、 RPC Client 回调函数触发
    public:
        /* 注册主机 */
        bool register_host(muse::simulator::host* host_ptr);

        host * get_host(const std::string& ip_or_and_port);

        ~network_dispatcher();
    };

    template class SIMULATOR_CPP_WIN_API singleton_lazy_heap<network_dispatcher>;
}

#define MUSE_NETWORK_DISPATCHER muse::simulator::singleton_lazy_heap<muse::simulator::network_dispatcher>

#endif //MUSE_SIMULATOR_NETWORK_DISPATCHER_HPP

