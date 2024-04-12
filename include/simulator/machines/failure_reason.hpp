//
// Created by 14270 on 2024-04-12.
//

#ifndef MUSE_SIMULATOR_FAILURE_REASON_HPP
#define MUSE_SIMULATOR_FAILURE_REASON_HPP
namespace muse::simulator{
    enum class FailureReason: short {
        OK, //没有失败
        TheServerResourcesExhausted, //服务器资源耗尽，请勿链接
        NetworkTimeout, //网络连接超时
        TheRunningLogicOfTheServerIncorrect, //服务器运行逻辑错误，返回的报文并非所需
    };
}
#endif //MUSE_SIMULATOR_FAILURE_REASON_HPP
