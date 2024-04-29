#include "simulator/machines/registry.hpp"

namespace muse::simulator {

    any_controller::any_controller(std::function<void(void *, BinarySerializer *)> &&f)
    :func(std::move(f)){

    }

    bool registry::check(const std::string &name) {
        return concurrent_dictionary.find(name) != concurrent_dictionary.end();
    }

    void registry::runSafely(computer *caller, const std::string &name, BinarySerializer *serializer) {
        if (check(name)){
            concurrent_dictionary[name]->func(caller, serializer);
        }
    }

    void registry::runEnsured(computer *caller, const std::string &name, BinarySerializer *serializer) {
        concurrent_dictionary[name]->func(caller, serializer);
    }

    ResponseData* registry::convert_result_to_response(BinarySerializer *serializer) {
        auto count = serializer->byteCount();
        auto ptr = muse::simulator::singleton_memory_pool::get_ptr()->allocate(count);

        std::shared_ptr<char[]> rdt((char*)ptr, [total = count](char *ptr){
            //fmt::print("release count {}", total);
            muse::simulator::singleton_memory_pool::get_ptr()->deallocate(ptr, total);
        });

        std::memcpy(rdt.get(),serializer->getBinaryStream(), serializer->byteCount());

        auto* responseData = new_by_pool<ResponseData>();
        responseData->data = std::move(rdt);
        responseData->set_success(true);
        responseData->total_size = count;
        return responseData;
    }



    uint64_t registry::get_remote_functions() {
        return this->concurrent_dictionary.size();
    }

    ResponseData *registry::function_not_exist_response() {
        BinarySerializer serializer;

        RpcResponseHeader header;
        header.setOkState(false);
        header.setReason(RpcFailureReason::MethodNotExist);
        serializer.input(header);

        auto count = serializer.byteCount();
        auto ptr = muse::simulator::singleton_memory_pool::get_ptr()->allocate(count);

        std::shared_ptr<char[]> rdt((char*)ptr, [total = count](char *ptr){
            //fmt::print("release count {}", total);
            muse::simulator::singleton_memory_pool::get_ptr()->deallocate(ptr, total);
        });

        auto* responseData = new_by_pool<ResponseData>();
        responseData->data = std::move(rdt);
        responseData->set_success(false);
        responseData->total_size = count;
        return responseData;
    }

    ResponseData *registry::parameter_error_response() {
        BinarySerializer serializer;

        RpcResponseHeader header;
        header.setOkState(false);
        header.setReason(RpcFailureReason::ParameterError);
        serializer.input(header);

        auto count = serializer.byteCount();
        auto ptr = muse::simulator::singleton_memory_pool::get_ptr()->allocate(count);

        std::shared_ptr<char[]> rdt((char*)ptr, [total = count](char *ptr){
            //fmt::print("release count {}", total);
            muse::simulator::singleton_memory_pool::get_ptr()->deallocate(ptr, total);
        });

        auto* responseData = new_by_pool<ResponseData>();
        responseData->data = std::move(rdt);
        responseData->set_success(false);
        responseData->total_size = count;
        return responseData;
    }

    ResponseData *registry::net_not_support_response() {
        BinarySerializer serializer;

        RpcResponseHeader header;
        header.setOkState(false);
        header.setReason(RpcFailureReason::Success);
        serializer.input(header);

        auto count = serializer.byteCount();
        auto ptr = muse::simulator::singleton_memory_pool::get_ptr()->allocate(count);

        std::shared_ptr<char[]> rdt((char*)ptr, [total = count](char *ptr){
            //fmt::print("release count {}", total);
            muse::simulator::singleton_memory_pool::get_ptr()->deallocate(ptr, total);
        });

        auto* responseData = new_by_pool<ResponseData>();
        responseData->data = std::move(rdt);
        responseData->set_success(false);
        responseData->total_size = count;
        responseData->reason = FailureReason::NetworkTimeout; //网络问题
        return responseData;
    }

    ResponseData *registry::server_inner_exception_response() {
        BinarySerializer serializer;

        RpcResponseHeader header;
        header.setOkState(false);
        header.setReason(RpcFailureReason::ServerInnerException);
        serializer.input(header);

        auto count = serializer.byteCount();
        auto ptr = muse::simulator::singleton_memory_pool::get_ptr()->allocate(count);

        std::shared_ptr<char[]> rdt((char*)ptr, [total = count](char *ptr){
            //fmt::print("release count {}", total);
            muse::simulator::singleton_memory_pool::get_ptr()->deallocate(ptr, total);
        });

        auto* responseData = new_by_pool<ResponseData>();
        responseData->data = std::move(rdt);
        responseData->set_success(false);
        responseData->total_size = count;
        return responseData;
    }
}