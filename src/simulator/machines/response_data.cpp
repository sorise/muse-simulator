#include "simulator/machines/response_data.hpp"

namespace muse::simulator{
    bool ResponseData::isOk() const { return this->isSuccess;};

    uint32_t ResponseData::getSize() const {return this->total_size;};

    FailureReason ResponseData::getFailureReason() const { return this->reason;};

    ResponseData::ResponseData()
    :total_size(0), isSuccess(false), data(nullptr){

    }

    void ResponseData::set_success(const bool &suc) {
        this->isSuccess = suc;
    }

    bool ResponseData::get_success() const {
        return this->isSuccess;
    }

    ResponseData::ResponseData(const ResponseData &other):
    total_size(other.total_size),
    data(other.data),
    isSuccess(other.isSuccess),
    reason(other.reason){

    }

    ResponseData::ResponseData(ResponseData &&other) noexcept
    :total_size(other.total_size), data(std::move(other.data)), isSuccess(other.isSuccess), reason(other.reason){

    }

    ResponseData &ResponseData::operator=(const ResponseData &other) {
        if (this != &other){
            this->total_size = other.total_size;
            this->data = other.data;
            this->isSuccess = other.isSuccess;
            this->reason = other.reason;
        }
        return *this;
    }

    ResponseData &ResponseData::operator=(ResponseData &&other) noexcept {
        if (this != &other){
            this->total_size = other.total_size;
            this->data = std::move(other.data);
            this->isSuccess = other.isSuccess;
            this->reason = other.reason;
        }
        return *this;
    }


}