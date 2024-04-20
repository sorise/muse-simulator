#include "simulator/machines/simulator_net_event_queue.hpp"

namespace muse::simulator{
    std::unique_ptr<std::queue<simulator_event>> simulator_net_event_queue::instance_ = std::make_unique<std::queue<simulator_event>>();
    std::shared_mutex simulator_net_event_queue::mutex_ = std::shared_mutex();

    auto simulator_net_event_queue::insert_event(simulator_event event) -> void {
        std::lock_guard lock(mutex_);
        instance_->push(event);
    }

    auto simulator_net_event_queue::is_empty() -> bool {
        std::shared_lock lock(mutex_);
        return instance_->empty();
    }

    auto simulator_net_event_queue::pop_event(bool &success) -> simulator_event {
        std::unique_lock lock(mutex_);
        if (!instance_->empty()){
            simulator_event ev = instance_->front();
            instance_->pop();
            success = true;
            return ev;
        }
        success = false;
        return {};
    }
}