#include "simulator/network_dispatcher.hpp"

namespace muse::simulator {

    bool network_dispatcher::register_host(host *host) {
        if (host == nullptr){
            return false;
        }
        std::unique_lock lock(this->mtx);
        if (this->hosts.find(host->get_ip_address()) != this->hosts.end()){
            this->hosts.emplace(host->get_ip_address(), host);
            return true;
        }
        return false;
    }

    bool network_dispatcher::RPC_Request(host *caller, TransmitterEvent &&event) {
        std::shared_lock lock(this->mtx);
        //先得到有这个主机和方法
        if (this->hosts.find(event.get_ip_address()) == this->hosts.end()){
            return false;
        }

        //存储这个msg 等待传输完成，再执行回调
        //为了方便查找，需要添加一个map

        return true;


    }

}
