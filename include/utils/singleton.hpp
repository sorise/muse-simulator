#ifndef MUSE_SIMULATOR_SINGLETON_HPP
#define MUSE_SIMULATOR_SINGLETON_HPP

#include <memory>
#include <mutex>
#include <shared_mutex>
#include <iostream>
#include <exception>
#include <memory>
#include <memory_resource>
#include <fmt/format.h>
#include "simulator/pool/pool.hpp"
#include "utils/toolkits.hpp"

namespace muse::simulator {
    /*
     * 单例饿汉模式： 天生就是线程安全的, 内存分配在heap,返回的是指针
     * 但是不能保证对T本身操作是线程安全的！
     * 如果需要保证T的操纵也是线程安全的需要T自己实现线程安全
     * */
    template<typename T>
    class singleton_hungry_heap {
    public:
        static T* get_ptr(){
            return singleton_hungry_heap::instance_.get();
        }

        static T& get_reference(){
            return *(singleton_hungry_heap::instance_.get());
        }
    private:
        singleton_hungry_heap() = default;
        singleton_hungry_heap(const singleton_hungry_heap&) = default;
        singleton_hungry_heap& operator=(const singleton_hungry_heap&) = default;
        ~singleton_hungry_heap()= default;
    private:
        static std::unique_ptr<T> instance_ ;
    };
    template<typename T>
    std::unique_ptr<T> singleton_hungry_heap<T>::instance_ = std::unique_ptr<T>(new T());

    /*
     * 单例饿汉模式： 天生就是线程安全的, 对象分配在静态存储区，谨慎使用！
     * 但是不能保证对T本身操作是线程安全的！
     * 如果需要保证T的操纵也是线程安全的需要T自己实现线程安全
     * */
    template<typename T>
    class singleton_hungry_static{
    public:
        static T& get_instance(){
            return instance_;
        }
    private:
        singleton_hungry_static() = default;
        singleton_hungry_static(const singleton_hungry_static&) = default;
        singleton_hungry_static& operator=(const singleton_hungry_static&) = default;
        ~singleton_hungry_static()= default;
    private:
        static T instance_ ;
    };
    template<typename T>
    T singleton_hungry_static<T>::instance_ = T();

    /*
     * 单例懒汉模式： 实现线程安全、利用pthread_once实现
     * 但是不能保证对T本身操作是线程安全的！
     * 如果需要保证T的操纵也是线程安全的需要T自己实现线程安全
     * */
    template<typename T>
    class singleton_lazy_heap{
    private:
        singleton_lazy_heap() = default;
        singleton_lazy_heap(const singleton_lazy_heap&) = default;
        singleton_lazy_heap& operator=(const singleton_lazy_heap&) = default;
        ~singleton_lazy_heap()= default;
    public:
        static T* get_ptr(){
            std::call_once(singleton_lazy_heap<T>::_flag, singleton_lazy_heap<T>::init);
            return instance_.get();
        }

        static T& get_reference(){
            std::call_once(singleton_lazy_heap<T>::_flag, singleton_lazy_heap<T>::init);
            return *(instance_.get());
        }
    private:
        static void init(){
            singleton_lazy_heap::instance_ = std::unique_ptr<T>(new T());
        }

        static std::once_flag _flag;

        static  std::unique_ptr<T> instance_;
    };

    template<typename T>
    std::once_flag singleton_lazy_heap<T>::_flag = std::once_flag();

    template<typename T>
    std::unique_ptr<T> singleton_lazy_heap<T>::instance_ = nullptr;


    template<> class SIMULATOR_CPP_WIN_API singleton_lazy_heap<std::pmr::synchronized_pool_resource>{
    private:
        singleton_lazy_heap() = default;
        singleton_lazy_heap(const singleton_lazy_heap&) = default;
        singleton_lazy_heap& operator=(const singleton_lazy_heap&) = default;
        ~singleton_lazy_heap()= default;
    public:
        static std::pmr::synchronized_pool_resource* get_ptr(){
            std::call_once(_flag, init);
            return instance_.get();
        }

        static std::pmr::synchronized_pool_resource& get_reference(){
            std::call_once(_flag, init);
            return *(instance_);
        }
    private:

        static void init(){
            std::pmr::pool_options option;
            option.largest_required_pool_block = 1024*1024*5; //5M
            option.max_blocks_per_chunk = 4096; //每一个chunk有多少个block
            instance_ = std::make_unique<std::pmr::synchronized_pool_resource>(option);
        }

        static std::once_flag _flag;

        static  std::unique_ptr<std::pmr::synchronized_pool_resource> instance_;
    };

    using singleton_memory_pool = singleton_lazy_heap<std::pmr::synchronized_pool_resource>;

    /* 如果T没有上下游继承关系，推荐使用 */
    template<typename T, typename ...Args>
    auto new_by_pool(Args&&... args) ->T*{
        static_assert(std::is_destructible<T>());
        void * place = singleton_memory_pool::get_ptr()->allocate(sizeof(T));
        if (place == nullptr){
            return nullptr;
        }
        fmt::print("new_by_pool:{}\n", sizeof(T));
        T* real = new(place) T(std::forward<Args>(args)...); //定位 new
        return real;
    }

    /* 如果T有继承关系，推荐使用 */
    template<typename T, typename ...Args>
    auto new_by_pool_share(Args&&... args) ->std::shared_ptr<T>{
        static_assert(std::is_destructible<T>());
        constexpr size_t T_size = sizeof(T);

        void * place = singleton_memory_pool::get_ptr()->allocate(T_size);
        if (place == nullptr){
            return nullptr;
        }
        T* real = new(place) T(std::forward<Args>(args)...); //定位 new
        std::shared_ptr<T> result(real, [T_size](T *p){
            p->~T(); //析构函数调用
            singleton_memory_pool::get_ptr()->deallocate(p, T_size);
        });
        return result;
    }

    template<typename T>
    auto delete_by_pool(T *ptr) ->void{
        static_assert(std::is_destructible<T>());
        ptr->~T();
        singleton_memory_pool::get_ptr()->deallocate(ptr, sizeof(T));
    }

    template<> class SIMULATOR_CPP_WIN_API singleton_lazy_heap<muse::pool::ThreadPool>{
    private:
        singleton_lazy_heap() = default;
        singleton_lazy_heap(const singleton_lazy_heap&) = default;
        singleton_lazy_heap& operator=(const singleton_lazy_heap&) = default;
        ~singleton_lazy_heap()= default;
    public:
        static muse::pool::ThreadPool* get_ptr(){
            std::call_once(_flag, init);
            return instance_.get();
        }

        static muse::pool::ThreadPool& get_reference(){
            std::call_once(_flag, init);
            return *(instance_);
        }
    private:
        static void init(){
            instance_ = std::make_unique<muse::pool::ThreadPool>(
                    2,
                    8,
                    12000,
                    muse::pool::ThreadPoolType::Flexible,
                    muse::pool::ThreadPoolCloseStrategy::WaitAllTaskFinish,
                    std::chrono::milliseconds(5000)
            );
        }

        static std::once_flag _flag;

        static  std::unique_ptr<muse::pool::ThreadPool> instance_;
    };

    using singleton_thread_pool = muse::simulator::singleton_lazy_heap<muse::pool::ThreadPool>;

}

#endif //MUSE_SIMULATOR_SINGLETON_HPP
