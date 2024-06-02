//
// Created by remix on 23-7-10.
//

#ifndef MUSE_CONF_HPP
#define MUSE_CONF_HPP
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include <memory>
#include "../utils/toolkits.hpp"

using namespace std::chrono_literals;

namespace muse::pool{
    /* 线程池模式 */
    enum class ThreadPoolType{
        Fixed = 0, //fixed 模式线程池
        Flexible = 1 //flexible 动态增长
    };

    /* 线程池关闭策略 */
    enum class ThreadPoolCloseStrategy{
        DiscardAllTasks,        //丢弃所有的任务
        WaitAllTaskFinish,      //等待任务完成
        ReturnTaskAndClose,     //返回任务，然后关闭
    };

    /*存放工作线程*/
    struct SIMULATOR_CPP_WIN_API Worker{
        std::atomic<bool> isRunning {false};
        std::shared_ptr<std::thread> workman {nullptr};
    };

    SIMULATOR_CPP_WIN_API std::chrono::milliseconds GetTick();

    /* 并发线程池的工作线程状态信息 */
    struct SIMULATOR_CPP_WIN_API ConcurrentWorker{
        std::atomic<bool> isRunning {false};
        //线程指针
        std::shared_ptr<std::thread> workman {nullptr};
        //记录当前线程有多久没工作了
        std::chrono::milliseconds noWorkTime {0ms};
        //上次循环是否在执行任务
        bool lastRunHaveTask {true};
    };

}


#endif //MUSE_CONF_HPP
