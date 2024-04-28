#include "simulator/machines/simulator_net_event_queue.hpp"

namespace muse::simulator{
    std::unique_ptr<std::list<simulator_event>> simulator_net_event_queue::instance_ = std::make_unique<std::list<simulator_event>>();
    std::shared_mutex simulator_net_event_queue::mutex_ = std::shared_mutex();

    auto simulator_net_event_queue::insert_event(simulator_event event) -> void {
        std::lock_guard lock(mutex_);
        instance_->emplace_back(event);
    }

    auto simulator_net_event_queue::is_empty() -> bool {
        std::shared_lock lock(mutex_);
        return instance_->empty();
    }

    auto simulator_net_event_queue::pop_event(bool &success) -> simulator_event {
        std::unique_lock lock(mutex_);
        if (!instance_->empty()){
            simulator_event ev = instance_->front();
            instance_->pop_front();
            success = true;
            return ev;
        }
        success = false;
        return {};
    }

    auto simulator_net_event_queue::reset() -> void {
        std::unique_lock lock(mutex_);
        for(auto& it : *instance_){
            delete_message_factory(it.message_);
        }
    }

    auto simulator_net_event_queue::for_each(std::function<bool(simulator_event&)>&& f) -> void{
        //考虑消息数量，如果数量太多需要 线程池优化
        std::unique_lock lock(mutex_);
        if (instance_->size() < MUSE_SIMULATOR_SETTING::NET_EVENT_USE_THREAD_TIMEOUT){
            auto last =std::remove_if(instance_->begin(), instance_->end(), f);
            instance_->erase(last, instance_->end()); //将后面的无用内存擦除掉
        }else{

        }

    }

}