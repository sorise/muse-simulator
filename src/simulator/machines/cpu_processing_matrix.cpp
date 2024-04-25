#include "simulator/machines/cpu_processing_matrix.hpp"

namespace muse::simulator{
    auto cpu_processing_matrix::get_random_number(const uint32_t &_down, const uint32_t &_up) -> uint32_t {
        std::random_device rd;
        std::default_random_engine e(rd() ); //传入一个随机数种子
        std::uniform_int_distribution<unsigned int > latency(_down,_up);//得到0到9之间的数
        return latency(e);
    }

    auto cpu_processing_matrix::set_RPC_processing_time_Server(const std::string &name,const uint32_t &_unified_cpu_down_us,const uint32_t &_unified_cpu_up_us) -> void {
        if (!this->initial_state_){
            throw std::logic_error("The cpu_processing_matrix is not initialized");
        }
        bool is_equal = (_unified_cpu_down_us == _unified_cpu_up_us);
        std::string key = name + " -> " + "trigger";
        this->cpu_map_->operator[](key) = std::make_tuple(_unified_cpu_down_us, _unified_cpu_up_us, is_equal);
    }

    auto cpu_processing_matrix::set_RPC_processing_time_Client(const std::string &name, const uint32_t &_unified_cpu_down_us, const uint32_t &_unified_cpu_up_us) -> void {
        if (!this->initial_state_){
            throw std::logic_error("The cpu_processing_matrix is not initialized");
        }
        bool is_equal = (_unified_cpu_down_us == _unified_cpu_up_us);
        std::string key = name + " -> " + "callback";
        this->cpu_map_->operator[](key) = std::make_tuple(_unified_cpu_down_us, _unified_cpu_up_us, is_equal);
    }


    auto cpu_processing_matrix::initial(const uint32_t &_unified_down_us,const uint32_t &_unified_up_us) -> bool {
        if (this->initial_state_){
            return false;
        }
        if (_unified_down_us > _unified_up_us){
            throw std::logic_error("The unified_latency_down_us must be less than or equal to the unified_latency_up_us");
        }
        this->initial_state_ = true;
        this->unified_cpu_down_us_ = _unified_down_us;
        this->unified_cpu_up_us_ = _unified_up_us;
        this->down_equal_up_ = (_unified_down_us == _unified_up_us);

        this->cpu_map_ = std::make_unique<CPU_Map_Type>();
        return true;
    }

    cpu_processing_matrix::cpu_processing_matrix()
    : initial_state_(false), unified_cpu_down_us_(0), unified_cpu_up_us_(0),down_equal_up_(true){

    }

    auto cpu_processing_matrix::get_client(const std::string &name)->uint32_t {
        if (!this->initial_state_){
            throw std::logic_error("The cpu_processing_matrix is not initialized");
        }
        std::string key = name + " -> " + "callback";
        if(this->cpu_map_->find(key) == this->cpu_map_->end()){
            if (this->down_equal_up_){
                return this->unified_cpu_down_us_;
            }else{
                return get_random_number(this->unified_cpu_down_us_, this->unified_cpu_up_us_);//得到0到9之间的数
            }
        }else{
            auto tpl = this->cpu_map_->at(key);
            if (std::get<0>(tpl) == std::get<1>(tpl)){
                return std::get<0>(tpl);
            }else{
                return get_random_number(std::get<0>(tpl) , std::get<1>(tpl));
            }
        }
    }

    auto cpu_processing_matrix::get_server(const std::string &name)->uint32_t {
        if (!this->initial_state_){
            throw std::logic_error("The cpu_processing_matrix is not initialized");
        }
        std::string key = name + " -> " + "trigger";
        auto it = this->cpu_map_->find(key);
        if( it == this->cpu_map_->end()){
            if (this->down_equal_up_){
                return this->unified_cpu_down_us_;
            }else{
                return get_random_number(this->unified_cpu_down_us_, this->unified_cpu_up_us_);//得到0到9之间的数
            }
        }else{
            auto tpl = this->cpu_map_->at(key);
            if (std::get<0>(tpl) == std::get<1>(tpl)){
                return std::get<0>(tpl);
            }else{
                return get_random_number(std::get<0>(tpl) , std::get<1>(tpl));
            }
        }
    }
}