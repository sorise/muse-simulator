#ifndef MUSE_SIMULATOR_TRANSMITTER_EVENT_HPP
#define MUSE_SIMULATOR_TRANSMITTER_EVENT_HPP
#include <unordered_map>
#include <iostream>
#include <utility>
#include <bitset>

#include "../pool/pool.hpp"
#include "../serializer/binarySerializer.h"
#include "outcome.hpp"
#include "response_data.hpp"
#include "../utils/toolkits.hpp"
#include "fmt/format.h"

namespace muse::simulator {
    /* 包装一个发送任务 */
    class SIMULATOR_CPP_WIN_API TransmitterEvent{
    public:
        std::string ip_address;             // 服务器地址
        uint16_t port;                      // 端口号
        std::string remote_process_name;    // 方法名称
        muse::serializer::BinarySerializer serializer;        // 数据位置
        std::function<void(ResponseData*)> callBack;  // 回调函数
        bool is_set_callback;
        bool is_set_remote_func;
    private:
        /* 可调用对象 返回值 R 不是 void */
        template<typename R>
        typename std::enable_if<!std::is_same<R, void>::value, void>::type
        callBind(std::function<void(Outcome<R>)> func, ResponseData* responseData){
            Outcome<R> result;
            if (responseData->isOk()){
                //写入数据
                try {
                    muse::serializer::BinaryDeserializeView viewer;
                    viewer.reset_bytestream(responseData->data.get(), responseData->getSize());
                    viewer.output(result.response);
                    if (result.response.getOkState()){
                        viewer.output(result.value);
                    }
                } catch (...) {
                    //读取错误，返回值非预期
                    fmt::print("read error!\n");
                    result.response.setOkState(false);
                    result.response.setReason(RpcFailureReason::UnexpectedReturnValue);
                }
            }else{
                //这是协议层的错误
                result.protocolReason = responseData->getFailureReason();
            }
            //触发回调函数
            func(result);
        }

        /* 可调用对象 返回值 R 是 void */
        template<typename R>
        typename std::enable_if<std::is_same<R, void>::value, void>::type
        callBind(std::function<void(Outcome<R>)> func, ResponseData* responseData){
            Outcome<void> result;
            if (responseData->isOk()){
                //写入数据
                try {
                    muse::serializer::BinaryDeserializeView viewer;
                    viewer.reset_bytestream(responseData->data.get(), responseData->getSize());
                    viewer.output(result.response);
                } catch (...) {
                    //读取错误，返回值非预期
                    result.response.setOkState(false);
                    std::string vals(responseData->data.get(), responseData->getSize());
                    std::cout << "get size: " << responseData->getSize() << "\n";
                    for (int i = 0; i < vals.size(); ++i) {
                        std::cout << (int)vals[i] << " ";
                    }
                    std::cout << "\n-------------------------------\n";
                    result.response.setReason(RpcFailureReason::UnexpectedReturnValue);
                }
            }else{
                //这是协议层的错误
                result.protocolReason = responseData->getFailureReason();
            }
            std::string vals(responseData->data.get(), responseData->getSize());
            //触发回调函数
            func(result);
        }

        /* 成员函数有返回值 R != void */
        template<typename R, typename C>
        typename std::enable_if<!std::is_same<R, void>::value, void>::type
        callBind(void(C::*func)(Outcome<R>), C* c, ResponseData* responseData){
            Outcome<R> result;
            if (responseData->isOk()){
                //写入数据
                try {
                    muse::serializer::BinaryDeserializeView viewer;
                    viewer.reset_bytestream(responseData->data.get(), responseData->getSize());
                    viewer.output(result.response);
                    if (result.response.getOkState()){
                        viewer.output(result.value);
                    }
                } catch (...) {
                    //读取错误，返回值非预期
                    result.response.setOkState(false);
                    result.response.setReason(RpcFailureReason::UnexpectedReturnValue);
                }
            }else{
                //这是协议层的错误
                result.protocolReason = responseData->getFailureReason();
            }
            std::string vals(responseData->data.get(), responseData->getSize());
            //触发回调函数
            ((*c).*func)(result);
        }

        /* 成员函数无返回值  R = void */
        template<typename R, typename C>
        typename std::enable_if<std::is_same<R, void>::value, void>::type
        callBind(void(C::*func)(Outcome<R>), C* c, ResponseData* responseData){
            Outcome<void> result;
            if (responseData->isOk()){
                //写入数据
                try {
                    muse::serializer::BinaryDeserializeView viewer;
                    viewer.reset_bytestream(responseData->data.get(), responseData->getSize());
                    viewer.output(result.response);
                } catch (...) {
                    //读取错误，返回值非预期
                    result.response.setOkState(false);
                    result.response.setReason(RpcFailureReason::UnexpectedReturnValue);
                }
            }else{
                //这是协议层的错误
                result.protocolReason = responseData->getFailureReason();
            }
            //触发回调函数
            ((*c).*func)(result);
        }

        template<typename R>
        void callProxy(std::function<void(Outcome<R>)> func, ResponseData* responseData){
            callBind(func, responseData);
        }

        // 函数指针
        template<typename R>
        void callProxy(void(*func)(Outcome<R>), ResponseData* responseData) {
            callProxy(std::function<void(Outcome<R>)>(func), responseData);
        }

        //函数成员指针
        template<typename R, typename C>
        void callProxy(void(C::* func)(Outcome<R>), C* c, ResponseData* responseData){
            callBind(func, c ,responseData);
        }

        template<typename F>
        inline void Proxy(F func, ResponseData* responseData)
        {
            callProxy(func, responseData);
        }

        template<typename F, typename C>
        inline void Proxy(F fun, C * c, ResponseData* responseData)
        {
            callProxy(fun, c, responseData);
        }

        template<typename F>
        void lambdaProxy(F func, ResponseData* responseData){
            lambdaHelper(std::function(func), responseData);
        }

        //函数成员指针
        template<typename R>
        void lambdaHelper(std::function<void(Outcome<R>)> func, ResponseData* responseData){
            callBind(func, responseData);
        }

    public:
        TransmitterEvent(std::string _ip_address, const uint16_t& _port);
        TransmitterEvent(const TransmitterEvent& other) = delete;
        TransmitterEvent(TransmitterEvent&& event) noexcept;
        TransmitterEvent& operator=(const TransmitterEvent& other) = delete;
        TransmitterEvent& operator=(TransmitterEvent&& event) noexcept;

        virtual ~TransmitterEvent();

        [[nodiscard]] bool get_callBack_state() const;

        [[nodiscard]] bool get_remote_state() const;

        muse::serializer::BinarySerializer& get_serializer();

        const std::string&  get_ip_address();

        const uint16_t& get_port();

        /* 决定调用哪个访问 */
        template<typename R, typename ...Argc>
        bool call(const std::string& Name, Argc&&...argc){
            remote_process_name = Name;
            serializer.clear();
            if (!is_set_remote_func ) is_set_remote_func = true;
            std::tuple<Argc...> tpl(argc...);
            serializer.input(Name); //写入远程名称
            serializer.input(tpl);  //写入参数
            //发送任务写到 序列化器中
            return true;
        }

        /* 普通函数指针 */
        template<typename F>
        void set_callBack(F &func){
            if (!is_set_callback ) is_set_callback = true;
            callBack = std::bind(&TransmitterEvent::Proxy<F>, this, func, std::placeholders::_1);
        }

        /* 普通函数指针 */
        template<typename F>
        void set_callBack(F &&func){
            if (!is_set_callback ) is_set_callback = true;
            callBack = std::bind(&TransmitterEvent::lambdaProxy<F>, this, std::forward<F>(func), std::placeholders::_1);
        }

        /* 成员函数 */
        template<typename C, typename F>
        void set_callBack(F func, C *c){
            if (!is_set_callback ) is_set_callback = true;
            callBack = std::bind(&TransmitterEvent::Proxy<F, C>, this, func, c, std::placeholders::_1);
        }

        /* 触发回调函数 */
        void trigger_callBack(ResponseData* responseData);
    };


}

#endif //MUSE_SIMULATOR_TRANSMITTER_EVENT_HPP
