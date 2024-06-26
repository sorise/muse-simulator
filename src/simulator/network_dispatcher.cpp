#include "simulator/network_dispatcher.hpp"

namespace muse::simulator {

    network_dispatcher::~network_dispatcher() = default;

    computer *network_dispatcher::get_host(const std::string &ip_or_and_port) {
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

    const std::string network_dispatcher::PREFIX = {"@context/"};

    auto network_dispatcher::clear_hosts() -> void {
        std::unique_lock lock(this->mtx);
        this->hosts.clear();
    }

    const std::list<muse::simulator::computer*> &network_dispatcher::get_hosts_list() {
        return this->host_list_;
    }
}
