//
// Created by 14270 on 2024-04-12.
//

#ifndef MUSE_SIMULATOR_OUTCOME_HPP
#define MUSE_SIMULATOR_OUTCOME_HPP
#include "failure_reason.hpp"
#include "rpc_response_header.hpp"
#include "utils/toolkits.hpp"

namespace muse::simulator{
    template<typename R>
    struct Outcome {
        R value;
        RpcResponseHeader response;
        FailureReason protocolReason {FailureReason::OK};
        [[nodiscard]] bool isOK() const;
    };

    template<>
    struct Outcome<void> {
        RpcResponseHeader response;
        FailureReason protocolReason {FailureReason::OK};
        [[nodiscard]] bool isOK() const;
    };

    template<typename R>
    bool Outcome<R>::isOK() const{
        return response.getOkState() && (protocolReason == FailureReason::OK);
    }
}
#endif //MUSE_SIMULATOR_OUTCOME_HPP
