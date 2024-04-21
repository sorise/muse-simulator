#include "simulator/machines/central_processing_unit.hpp"

namespace muse::simulator {

    central_processing_unit::central_processing_unit(const uint32_t &_core_num)
    : core_num_(_core_num),thread_max_run_(2*_core_num){
        this->core_load_.reserve(this->thread_max_run_);
        this->core_load_.resize(this->thread_max_run_);
    }

    int32_t central_processing_unit::get_spare_core(const uint64_t &tick) {
        auto it = std::min_element(this->core_load_.begin(), this->core_load_.end());
        size_t minIndex = -1;
        if (it != this->core_load_.end()) {
             minIndex = std::distance(this->core_load_.begin(), it);
        }
        if (*it <= tick){
            return static_cast<int32_t>(minIndex);
        }
        return -1;
    }

    void central_processing_unit::print_core_state(const uint64_t &tick) {
        int i = 0;
        for (auto& core: this->core_load_) {
            fmt::print("[CPU Core {} state : {}]\n", i++, core <= tick?std::string("vacant"):std::string("running"));
        }
    }


}