#include "simulator/network_dispatcher.hpp"

namespace muse::simulator {

    bool network_dispatcher::register_host(host *host) {
        if (host == nullptr){
            return false;
        }
        std::unique_lock lock(this->mtx);
        if (this->ips.find(host->get_ip_address()) != this->ips.end()){
            return false;
        }
        this->ips.insert({host->get_ip_address(), this->hosts.size()});
        this->hosts.push_back(host);
        return true;
    }

    bool network_dispatcher::RPC_Request(TransmitterEvent &&event) {
        return true;
    }
}
