#include "simulator/network_dispatcher.hpp"

namespace muse::simulator {

    bool network_dispatcher::register_host(host *host) {
        if (host == nullptr){
            return false;
        }
        std::unique_lock lock(this->mtx);
        if (this->hosts.find(host->get_ip_address()) == this->hosts.end()){
            this->hosts.emplace(host->get_ip_address(), host);
            this->host_list_.emplace_back(host);
            return true;
        }
        return false;
    }

    network_dispatcher::~network_dispatcher() {
        for(auto &[k,host] : this->hosts){
            delete_by_pool(host);
        }
    }

    host *network_dispatcher::get_host(const std::string &ip_or_and_port) {
        std::unique_lock lock(this->mtx);
        if (this->hosts.find(ip_or_and_port) != this->hosts.end()){
            return this->hosts[ip_or_and_port];
        }
        lock.unlock();
        return nullptr;
    }

    size_t network_dispatcher::get_host_count() {
        return this->hosts.size();
    }

    auto network_dispatcher::clear_hosts() -> void {
        std::unique_lock lock(this->mtx);
        for (auto &[k,v]: this->hosts) {
            delete_by_pool(v);
            v = nullptr;
        }
    }

    const std::list<muse::simulator::host *> &network_dispatcher::get_hosts_list() {
        return this->host_list_;
    }
}
