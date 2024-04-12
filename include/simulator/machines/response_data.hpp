#ifndef MUSE_SIMULATOR_RESPONSEDATA_HPP
#define MUSE_SIMULATOR_RESPONSEDATA_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <memory_resource>
#include "failure_reason.hpp"
#include "utils/toolkits.hpp"

namespace muse::simulator{
    //RPC响应数据
    class SIMULATOR_CPP_WIN_API ResponseData{
    public:
        //总共大小
        uint32_t total_size;
        //是否成功
        bool isSuccess ;
        //失败原因，一般是网络原因
        FailureReason reason{ FailureReason::OK };
    public:
        //数据
        std::shared_ptr<char[]> data;
        //设置成功
        void set_success(const bool & suc);
        //成功状态
        [[nodiscard]] bool get_success() const;

        ResponseData();

        ResponseData(const ResponseData& other);

        ResponseData(ResponseData&& other) noexcept;

        ResponseData& operator=(const ResponseData& other);

        ResponseData& operator=(ResponseData&& other) noexcept;

        //是否成功
        [[nodiscard]] bool isOk() const;

        [[nodiscard]] uint32_t getSize() const;

        [[nodiscard]] FailureReason getFailureReason() const;
    };
}

#endif //MUSE_SIMULATOR_RESPONSEDATA_HPP
