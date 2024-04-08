//
// Created by 14270 on 2024-04-07.
//

#ifndef MUSE_SIMULATOR_SINGLETON_HPP
#define MUSE_SIMULATOR_SINGLETON_HPP

#include <iostream>
#include <exception>
#include <fmt/format.h>

namespace muse::chain {
    /*
     * 单例饿汉模式： 天生就是线程安全的, 内存分配在heap,返回的是指针
     * 但是不能保证对T本身操作是线程安全的！
     * 如果需要保证T的操纵也是线程安全的需要T自己实现线程安全
     * */
    template<typename T>
    class singleton_hungry_heap {
    public:
        static T* get_ptr(){
            return instance_.get();
        }

        static T& get_reference(){
            return *instance_;
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
            std::call_once(_flag, init);
            return instance_.get();
        }

        static T& get_reference(){
            std::call_once(_flag, init);
            return *instance_;
        }

        static void init(){
            instance_ = std::unique_ptr<T>(new T());
        }
    private:
        static std::once_flag _flag;
        static  std::unique_ptr<T> instance_;
    };

    template<typename T>
    std::once_flag singleton_lazy_heap<T>::_flag = std::once_flag();
    template<typename T>
    std::unique_ptr<T> singleton_lazy_heap<T>::instance_ = nullptr;


}

#endif //MUSE_SIMULATOR_SINGLETON_HPP
