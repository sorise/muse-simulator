#include "simulator/machines/host_delay_matrix.hpp"

namespace muse::simulator{
    host_delay_matrix::host_delay_matrix()
    :initial_state_(false),
    type_(host_delay_type::Unified_Latency),
    unified_latency_down_ms_(0),
    unified_latency_up_ms_(0),
    down_equal_up_(true),
    latency_map_(nullptr){

    }

    auto host_delay_matrix::initial(host_delay_type _type, const uint32_t& _unified_latency_down_ms, const uint32_t& _unified_latency_up_ms) -> bool {
        if (this->initial_state_){
            return false;
        }
        if (_unified_latency_down_ms > _unified_latency_up_ms){
            throw std::logic_error("The unified_latency_down_ms must be less than or equal to the unified_latency_up_ms");
        }
        this->initial_state_ = true;
        this->unified_latency_down_ms_ = _unified_latency_down_ms;
        this->unified_latency_up_ms_ = _unified_latency_up_ms;
        this->type_ = _type;
        this->down_equal_up_ = (_unified_latency_down_ms == _unified_latency_up_ms);
        if(this->type_ == host_delay_type::Different_Latency){
            this->latency_map_ = std::make_unique<Latency_Map_Type>();
        }
        return true;
    }

    auto host_delay_matrix::get_delay(const std::string& host_a_ip_address, const std::string& host_b_ip_address) -> uint32_t {
        if (!this->initial_state_){
            throw std::logic_error("The host_delay_matrix has not yet been initialized by calling the initial method");
        };
        if (this->type_ == host_delay_type::Unified_Latency){
            if (this->down_equal_up_){
                return this->unified_latency_up_ms_;
            }else{
                return get_random_number(this->unified_latency_down_ms_, this->unified_latency_up_ms_);
            }
        }else if (this->type_ == host_delay_type::Different_Latency){
            std::string key = host_a_ip_address + ":" + host_b_ip_address;
            if(this->latency_map_->find(key) == this->latency_map_->end()){
                if (this->down_equal_up_){
                    return this->unified_latency_up_ms_;
                }else{
                    return get_random_number(this->unified_latency_down_ms_, this->unified_latency_up_ms_);//得到0到9之间的数
                }
            }else{
                auto tpl = this->latency_map_->at(key);
                if (std::get<0>(tpl) == std::get<1>(tpl)){
                    return std::get<0>(tpl);
                }else{
                    return get_random_number(std::get<0>(tpl) , std::get<1>(tpl));
                }
            }
        };
        return this->unified_latency_up_ms_;
    }

    auto host_delay_matrix::set_host_delay(const std::string& host_a_ip_address, const std::string& host_b_ip_address,
                                           const uint32_t &_delay_down_ms, const uint32_t &_delay_up_ms) -> void {
        std::string key = host_a_ip_address + ":" + host_b_ip_address;
        if (_delay_down_ms > _delay_up_ms){
            throw std::logic_error("The _delay_down_ms must be less than or equal to the _delay_up_ms");
        }
        bool is_equal = (_delay_down_ms == _delay_up_ms);
        if (this->latency_map_->find(key) == this->latency_map_->end()){
            this->latency_map_->emplace(key, std::make_tuple(_delay_down_ms, _delay_up_ms, is_equal));
        }else{
            this->latency_map_->at(key) = std::make_tuple(_delay_down_ms, _delay_up_ms,is_equal);
        }
    }

    auto host_delay_matrix::get_random_number(const uint32_t &_down, const uint32_t &_up) -> uint32_t {
        std::random_device rd;
        std::default_random_engine e(rd() ); //传入一个随机数种子
        std::uniform_int_distribution<unsigned int > latency(_down,_up);//得到0到9之间的数
        return latency(e);
    }
}

