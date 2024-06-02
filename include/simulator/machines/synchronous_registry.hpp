#ifndef MUSE_SIMULATOR_SYNCHRONOUS_REGISTRY_HPP
#define MUSE_SIMULATOR_SYNCHRONOUS_REGISTRY_HPP
#include <iostream>
#include <unordered_map>

#include "../serializer/binarySerializer.h"
#include "../network_dispatcher.hpp"
#include "rpc_response_header.hpp"

#include <tuple>
#include <thread>
#include <mutex>
#include <memory>
#include <functional>
#include "response_data.hpp"
#include "host.hpp"


using namespace muse::serializer;

namespace muse::simulator {
    struct SIMULATOR_CPP_WIN_API Controller {
        std::function<void(host* caller, BinarySerializer*)> func;
        explicit Controller(std::function<void(host* caller,BinarySerializer*)>&& f);
    };

    /* 自己同步方法注册器 */
    class SIMULATOR_CPP_WIN_API synchronous_registry {
    private:
        //方法存储中心，使用hash 列表 存储, 参数是 BinarySerializer
        //是否需要线程安全，互斥量
        std::unordered_map<std::string, std::shared_ptr<Controller>> concurrent_dictionary;
    public:

        synchronous_registry() = default;

        // 用tuple做参数调用函数模板类
        template<typename Function, typename Tuple, std::size_t... Index>
        decltype(auto) invoke_impl(Function&& func, host*c,  Tuple&& t, std::index_sequence<Index...>)
        {
            return ((*c).*func)(std::get<Index>(std::forward<Tuple>(t))...);
        }

        /* 函数有返回值 */
        template<typename R, typename ...Params>
        typename std::enable_if<!std::is_same<R, void>::value, void>::type
        callBind(R(host::*func)(Params...), host* caller, BinarySerializer *serializer){
            RpcResponseHeader header;
            std::tuple<typename std::decay<Params>::type...> tpl;
            try {
                serializer->output(tpl);
            }catch (const BinarySerializerException &bet){
                header.setReason(RpcFailureReason::ParameterError);
                serializer->clear();
                serializer->input(header);
                return;
            }catch (const std::exception &ex) {
                header.setReason(RpcFailureReason::ServerInnerException);
                serializer->clear();
                serializer->input(header);
                return;
            }
            //防止参数解析失败
            try {
                R result = invoke_impl(func, caller,tpl, std::make_index_sequence<sizeof...(Params)>{});
                serializer->clear();
                header.setOkState(true); //执行成功
                serializer->input(header); //先写入头，再写入结果
                serializer->input(result);
            }
            catch(...) {
                header.setReason(RpcFailureReason::MethodExecutionError);
                serializer->clear();
                serializer->input(header);
                return;
            }
        }

        /* 成员函数无返回值 */
        template<typename R, typename ...Params>
        typename std::enable_if<std::is_same<R, void>::value, void>::type
        callBind(R(host::*func)(Params...), host* caller, BinarySerializer* serializer){
            //防止参数解析失败
            RpcResponseHeader header;
            std::tuple<typename std::decay<Params>::type...> tpl;
            try {
                serializer->output(tpl);
            }catch (const BinarySerializerException &bet){
                header.setReason(RpcFailureReason::ParameterError);
                serializer->clear();
                serializer->input(header);
                return;
            }catch (const std::exception &ex) {
                header.setReason(RpcFailureReason::ServerInnerException);
                serializer->clear();
                serializer->input(header);
                return;
            }
            //防止参数解析失败
            try {
                invoke_impl(func, caller,tpl, std::make_index_sequence<sizeof...(Params)>{});
                serializer->clear();
                header.setOkState(true); //执行成功
                serializer->input(header); //先写入头，再写入结果
            }
            catch(...) {
                header.setReason(RpcFailureReason::MethodExecutionError);
                serializer->clear();
                serializer->input(header);
                return;
            }
        }

        //函数成员指针
        template<typename R, typename... Params>
        void callProxy(R(host::* func)(Params...), host* caller, BinarySerializer* serializer){
            callBind(func, caller ,serializer);
        }

        template<typename F>
        inline void Proxy(F fun, host* caller, BinarySerializer* serializer)
        {
            callProxy(fun,caller,serializer);
        }

        /* 成员函数注册 */
        template<typename F>
        bool Bind(const std::string& name, F func){
            if (concurrent_dictionary.find(name) == concurrent_dictionary.end()){
                concurrent_dictionary.emplace(name, std::make_shared<Controller>(std::bind(&synchronous_registry::Proxy<F>, this, func, std::placeholders::_1, std::placeholders::_2)));
                return true;
            }
            return false;
        }

        /*查看某个名称下是否具有方法*/
        bool check(const std::string& name);

        /* 执行方法,如果指定方法存在则调用，否则什么也不做 */
        void runSafely(const std::string& name, BinarySerializer *serializer);

        /* 执行这方法之前，请先调用 check 确认执行的方法已经存在 */
        void runEnsured(const std::string& name, BinarySerializer *serializer);

        static ResponseData convert_result_to_response(BinarySerializer *serializer);

        uint64_t get_remote_functions();
    };

    template class SIMULATOR_CPP_WIN_API singleton_lazy_heap<synchronous_registry>;

    #define MUSE_SYNCHRONOUS_REGISTRY muse::simulator::singleton_lazy_heap<muse::simulator::synchronous_registry>
}





#endif //MUSE_SIMULATOR_SYNCHRONOUS_REGISTRY_HPP
