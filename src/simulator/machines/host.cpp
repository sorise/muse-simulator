#include <utility>

#include "simulator/machines/host.hpp"

namespace muse::simulator{

    auto host::vote() -> int {
        std::cout << "vote" << std::endl;
        return 5;
    }

    auto host::vote_hash(std::string name) -> std::string {
        return "val";
    }

    auto host::add(int a, int b) -> int {
        return a + b;
    }

    host::host(std::string ip_address, const uint64_t &memory_size, const double &cpu_frequency,const uint64_t &cpu_core_number)
    :computer(std::move(ip_address), memory_size, cpu_frequency, cpu_core_number), is_running(true) {

    }

    host::host():computer(), is_running(true) {

    }
}