#include "utils/singleton.hpp"

namespace muse::simulator {
    std::once_flag singleton_lazy_heap<std::pmr::synchronized_pool_resource>::_flag = std::once_flag();

    std::unique_ptr<std::pmr::synchronized_pool_resource> singleton_lazy_heap<std::pmr::synchronized_pool_resource>::instance_ = nullptr;

    std::once_flag singleton_lazy_heap<muse::pool::ThreadPool>::_flag = std::once_flag();

    std::unique_ptr<muse::pool::ThreadPool> singleton_lazy_heap<muse::pool::ThreadPool>::instance_ = nullptr;

}