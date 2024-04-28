#ifndef MUSE_THREAD_POOL_COMMIT_RESULT_HPP
#define MUSE_THREAD_POOL_COMMIT_RESULT_HPP 1

#include "conf.h"

namespace muse::pool{
    enum class RefuseReason{
        NoRefuse,                       //没有拒绝
        CannotExecuteAgain,             //请勿重复执行
        ThreadPoolHasStoppedRunning,    //线程池已经停止运行
        TaskQueueFulled,                //任务队列已经满了
        MemoryNotEnough,                //内存不足
    };

    struct SIMULATOR_CPP_WIN_API CommitResult{
        bool isSuccess;       //是否添加成功
        RefuseReason reason;  //失败原因
    };
}
#endif //MUSE_THREAD_POOL_COMMIT_RESULT_HPP
