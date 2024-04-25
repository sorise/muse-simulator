#include <algorithm>
#include "simulator/machines/central_processing_unit.hpp"

namespace muse::simulator {

    central_processing_unit::central_processing_unit(const uint32_t &_core_num)
    :core_num_(_core_num),thread_max_run_(2*_core_num){
        this->core_load_.reserve(this->thread_max_run_);
        this->core_load_.resize(this->thread_max_run_);
    }

    int32_t central_processing_unit::get_spare_core(const uint64_t &us_tick) {
        auto it = std::min_element(this->core_load_.begin(), this->core_load_.end());
        size_t minIndex = -1;
        if (it != this->core_load_.end()) {
             minIndex = std::distance(this->core_load_.begin(), it);
        }
        if (*it <= us_tick){
            return static_cast<int32_t>(minIndex);
        }
        return -1;
    }

    void central_processing_unit::print_core_state(const uint64_t &ms_tick) {
        int i = 0;
        auto tick = ms_tick * 1000; //毫秒转换为微妙
        int vacant = 0, occupied = 0;
        for (auto& core: this->core_load_) {
            if (core <= tick){
                vacant++;
            }else{
                occupied++;
            }
            fmt::print("[CPU Core {} state : {} last running us: {}]\n", i++, core <= tick?std::string("vacant"):std::string("running"), core);
        }
        fmt::print("vacant core: {} running core: {}\n", vacant, occupied);
    }

    //前面是毫秒，后面是运行时间
    bool central_processing_unit::carry_on_core(const uint64_t &_ms_tick, const uint64_t & _us_runtime) {
        auto us_tick = _ms_tick * 1000;
        auto idx = get_spare_core(us_tick);
        if ( idx >= 0 ){
            this->core_load_[idx] = us_tick + _us_runtime;
            return true;
        }
        return false;
    }

    central_processing_unit::central_processing_unit(central_processing_unit &&_cpu) noexcept
    :core_num_(_cpu.core_num_),
    thread_max_run_(_cpu.thread_max_run_),
    core_load_(std::move(_cpu.core_load_)){

    }

    central_processing_unit &central_processing_unit::operator=(central_processing_unit &&_cpu) noexcept {
        if (this != &_cpu){
            this->core_num_ = _cpu.core_num_;
            this->thread_max_run_ = _cpu.thread_max_run_;
            this->core_load_ = std::move(_cpu.core_load_);
            _cpu.core_num_ = 0;
            _cpu.thread_max_run_ = 0;
            _cpu.core_load_.clear();
        }
        return *this;
    }
}