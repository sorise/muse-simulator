#include "utils/singleton.hpp"
namespace muse::chain {

    std::once_flag singleton_lazy_heap<std::pmr::synchronized_pool_resource>::_flag = std::once_flag();

    std::unique_ptr<std::pmr::synchronized_pool_resource> singleton_lazy_heap<std::pmr::synchronized_pool_resource>::instance_ = nullptr;
}