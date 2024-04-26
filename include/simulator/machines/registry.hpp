#ifndef MUSE_SIMULATOR_REGISTRY_HPP
#define MUSE_SIMULATOR_REGISTRY_HPP

#include <tuple>
#include <mutex>
#include <thread>
#include <memory>
#include <iostream>
#include <functional>
#include <unordered_map>

#include "simulator/machines/computer.hpp"
#include "utils/singleton.hpp"
#include "response_data.hpp"
#include "rpc_response_header.hpp"
#include "serializer/binarySerializer.h"
#include "../network_dispatcher.hpp"

using namespace muse::serializer;

namespace muse::simulator {
    struct SIMULATOR_CPP_WIN_API any_controller {
        std::function<void(void* caller, BinarySerializer*)> func;
        explicit any_controller(std::function<void(void* caller,BinarySerializer*)>&& f);
    };

    class registry{
    private:
        //方法存储中心，使用hash 列表 存储, 参数是 BinarySerializer
        //是否需要线程安全，互斥量
        std::unordered_map<std::string, std::shared_ptr<any_controller>> concurrent_dictionary;
    public:
        registry() = default;

        // 用tuple做参数调用函数模板类
        template<typename Function, typename C, typename Tuple, std::size_t... Index>
        decltype(auto) invoke_impl(Function&& func, C *c, Tuple&& t, std::index_sequence<Index...>)
        {
            return ((*c).*func)(std::get<Index>(std::forward<Tuple>(t))...);
        }

        /* 函数有返回值 */
        template<typename R, typename C, typename ...Params>
        typename std::enable_if<!std::is_same<R, void>::value, void>::type
        callBind(R(C::*func)(Params...), C* c, BinarySerializer *serializer){
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
                R result = invoke_impl(func, c,tpl, std::make_index_sequence<sizeof...(Params)>{});
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
        template<typename R,typename C, typename ...Params>
        typename std::enable_if<std::is_same<R, void>::value, void>::type
        callBind(R(C::*func)(Params...), C* c, BinarySerializer* serializer){
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
                invoke_impl(func, tpl, c, std::make_index_sequence<sizeof...(Params)>{});
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
        template<typename R, typename C, typename... Params>
        void callProxy(R(C::* func)(Params...),void *caller, BinarySerializer* serializer){
            static_assert(std::is_base_of_v<muse::simulator::computer, C>);
            C *c = static_cast<C *>(caller);
            callBind<R, C, Params...>(func, c, serializer);
        }

        template<typename F>
        inline void Proxy(F fun, void *caller, BinarySerializer* serializer)
        {
            callProxy(fun, caller, serializer);
        }

        /* 成员函数注册 */
        template<typename F>
        bool Bind(const std::string& name, F func){
            if (concurrent_dictionary.find(name) == concurrent_dictionary.end()){
                auto f = std::bind(&registry::Proxy<F>, this, func, std::placeholders::_1, std::placeholders::_2);
                auto ac = std::make_shared<any_controller>(std::move(f));
                concurrent_dictionary.emplace(name, ac);
                return true;
            }
            return false;
        }

        /*查看某个名称下是否具有方法*/
        bool check(const std::string& name);

        /* 执行方法,如果指定方法存在则调用，否则什么也不做 */
        void runSafely(const std::string& ip_address, const std::string& name,  BinarySerializer *serializer);

        /* 执行这方法之前，请先调用 check 确认执行的方法已经存在 */
        void runEnsured(const std::string& ip_address, const std::string& name, BinarySerializer *serializer);

        static ResponseData convert_result_to_response(BinarySerializer *serializer);

        uint64_t get_remote_functions();
    };
    template class SIMULATOR_CPP_WIN_API singleton_lazy_heap<registry>;

    #define MUSE_REGISTRY muse::simulator::singleton_lazy_heap<muse::simulator::registry>
}
#endif //MUSE_SIMULATOR_REGISTRY_HPP
