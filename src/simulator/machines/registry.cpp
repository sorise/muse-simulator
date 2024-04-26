#include "simulator/machines/registry.hpp"

namespace muse::simulator {

    any_controller::any_controller(std::function<void(void *, BinarySerializer *)> &&f)
    :func(std::move(f)){

    }

    bool registry::check(const std::string &name) {
        return false;
    }
}