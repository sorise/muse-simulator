#include "simulator/machines/timer_node.hpp"

namespace muse::simulator{
    timer_node::timer_node(const uint64_t &_id,CallBack ca, bool duplicate, uint64_t interval, uint64_t expire)
    :callBack(std::move(ca)),
    duplicate_(duplicate),
    interval_(interval),
    expire_(expire),
    run_times_(1),
    id_(_id){
        add_time_ = SIMULATOR_WORLD_STATE::get_reference().get_tick();
    }

    uint64_t timer_node::get_expire() const {
        return this->expire_;
    }

    bool timer_node::is_duplicate() const {
        return this->duplicate_;
    }

    uint64_t timer_node::get_internal() const {
        return this->interval_;
    }

    void timer_node::stop() {
        this->clear_state_.store(true);
    }

    uint64_t timer_node::get_add_time() const {
        return this->add_time_;
    }

    void timer_node::duplicate_run_setting() {
        if (this->duplicate_ && !this->clear_state_.load()){
            this->expire_ = this->add_time_ + this->interval_ * (this->run_times_ + 1);
            this->run_times_++;
        }
    }

    void timer_node::trigger() const{
        if (!this->clear_state_.load()){
            this->callBack();
        }
    }

    bool timer_node::get_clear_state() const {
        return this->clear_state_.load();
    }

    uint64_t timer_node::get_id() const {
        return this->id_;
    }

    void timer_node::set_id(const uint64_t& _id){
        this->id_ = _id;
    }

    const std::string &timer_node::get_task_name() const {
        return this->name_;
    }

    void timer_node::set_task_name(const std::string &_name) {
        this->name_ = _name;
    }

    timer_node_wrapper::timer_node_wrapper(std::shared_ptr<timer_node> _node)
    : node(std::move(_node)){

    }

    uint64_t timer_node_wrapper::expire() const {
        return node->get_expire();
    }

    void timer_node_wrapper::trigger() const {
        this->node->trigger();
    }

    bool timer_node_wrapper::is_duplicate() const {
        return this->node->is_duplicate();
    }

    timer_node_wrapper::timer_node_wrapper(const timer_node_wrapper &other)
    : node(other.node){

    }

    timer_node_wrapper::timer_node_wrapper(timer_node_wrapper &&other) noexcept
    : node(std::move(other.node)){

    }

    timer_node_wrapper &timer_node_wrapper::operator=(const timer_node_wrapper &other) {
        if (this != &other){
            this->node = other.node;
        }
        return *this;
    }

    timer_node_wrapper &timer_node_wrapper::operator=(timer_node_wrapper &&other) noexcept {
        if (this != &other){
            this->node = std::move(other.node);
        }
        return *this;
    }

    void timer_node_wrapper::duplicate_run_setting() {
        this->node->duplicate_run_setting();
    }

    bool timer_node_wrapper::get_clear_state() const {
        return node->get_clear_state();
    }

    void timer_node_wrapper::stop() {
        this->node->stop();
    }

    uint64_t timer_node_wrapper::get_id() const {
        return this->node->get_id();
    }

    const std::string &timer_node_wrapper::get_task_name() const {
        return this->node->get_task_name();
    }
}