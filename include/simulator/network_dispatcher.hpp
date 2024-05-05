#ifndef MUSE_SIMULATOR_NETWORK_DISPATCHER_HPP
#define MUSE_SIMULATOR_NETWORK_DISPATCHER_HPP

#include "../utils/toolkits.hpp"
#include "../utils/singleton.hpp"

#include <mutex>
#include <vector>
#include <string>
#include <shared_mutex>
#include <unordered_map>

#include "machines/computer.hpp"
#include "machines/transmitter_event.hpp"
#include "machines/message.hpp"
#include "machines/network_card.hpp"

namespace muse::simulator {
    class SIMULATOR_CPP_WIN_API computer;
    /*
     * @网络调度器 需要有一个网络节点列表，一个事件队列，一个事件队列锁
     * */
    class SIMULATOR_CPP_WIN_API network_dispatcher {
    public:
        static const std::string PREFIX;
    private:
        //那些已经注册了的主机  ip+端口
        std::unordered_map<std::string, std::shared_ptr<muse::simulator::computer>> hosts;
        std::list<std::shared_ptr<muse::simulator::computer>> host_list_;
        std::shared_mutex mtx;
        //事件队列 ， RPC Server 触发、 RPC Client 回调函数触发
    public:
        /* 注册主机 */
        template<class C,  typename = std::enable_if_t<std::is_base_of<muse::simulator::computer, C>::value>>
        bool register_host(std::shared_ptr<C> host_ptr){
            static_assert(!std::is_same<C, computer>::value, "Type C must publicly derive from muse::simulator::computer");
            if (host_ptr == nullptr){
                return false;
            }
            std::unique_lock lock(this->mtx);
            if (this->hosts.find(host_ptr->get_ip_address()) == this->hosts.end()){
                this->hosts.emplace(host_ptr->get_ip_address(), host_ptr);
                this->host_list_.emplace_back(host_ptr);
                return true;
            }
            return false;
        }

        computer * get_host(const std::string& ip_or_and_port);

        size_t get_host_count();

        auto clear_hosts() -> void;

        const std::list<std::shared_ptr<muse::simulator::computer>>& get_hosts_list();

        ~network_dispatcher();
    };

    template class SIMULATOR_CPP_WIN_API muse::simulator::singleton_lazy_heap<muse::simulator::network_dispatcher>;

    #define MUSE_NETWORK_DISPATCHER muse::simulator::singleton_lazy_heap<muse::simulator::network_dispatcher>

}


#endif //MUSE_SIMULATOR_NETWORK_DISPATCHER_HPP

