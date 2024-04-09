#ifndef MUSE_SIMULATOR_NETWORK_DISPATCHER_HPP
#define MUSE_SIMULATOR_NETWORK_DISPATCHER_HPP
#include "../utils/toolkits.hpp"
#include "machines/host.hpp"
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <unordered_map>

namespace muse::simulator {
    class SIMULATOR_CPP_WIN_API network_dispatcher {
    private:
        //那些已经注册了的主机
        std::unordered_map<std::string, uint64_t> ips;
        std::vector<host*> hosts;
        std::shared_mutex mtx;
    public:
        bool register_host(host* host);


    };
}

#endif //MUSE_SIMULATOR_NETWORK_DISPATCHER_HPP

