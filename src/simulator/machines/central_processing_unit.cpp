#include "simulator/machines/central_processing_unit.hpp"

namespace muse::simulator {

    central_processing_unit::central_processing_unit(const uint32_t &_core_num)
    : core_num_(_core_num){
        for (int i = 0; i < this->core_num_ * 2; ++i) {
            this->core_load_.emplace(0);
        }
    }

    int32_t central_processing_unit::get_spare_core(const uint64_t &tick) {
        if (this->core_load_.top())
        return 0;
    }


}