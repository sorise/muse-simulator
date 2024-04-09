#include "simulator/machines/host.hpp"

namespace muse::simulator{

    auto host::vote() -> int {
        return 5;
    }

    auto host::vote_hash(std::string name) -> std::string {
        return "val";
    }
}