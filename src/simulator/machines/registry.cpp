#include "simulator/machines/registry.hpp"

namespace muse::simulator {

    any_controller::any_controller(std::function<void(void *, BinarySerializer *)> &&f)
    :func(std::move(f)){

    }

    bool registry::check(const std::string &name) {
        return concurrent_dictionary.find(name) != concurrent_dictionary.end();
    }

    void registry::runSafely(const std::string &ip_address, const std::string &name, BinarySerializer *serializer) {
        if (check(name)){
            void *caller = MUSE_NETWORK_DISPATCHER::get_ptr()->get_host(ip_address);
            concurrent_dictionary[name]->func(caller, serializer);
        }
    }

    void registry::runEnsured(const std::string &ip_address, const std::string &name, BinarySerializer *serializer) {

    }

    ResponseData registry::convert_result_to_response(BinarySerializer *serializer) {
        auto count = serializer->byteCount();
        auto ptr = muse::simulator::singleton_memory_pool::get_ptr()->allocate(count);

        std::shared_ptr<char[]> rdt((char*)ptr, [total = count](char *ptr){
            fmt::print("release count {}", total);
            muse::simulator::singleton_memory_pool::get_ptr()->deallocate(ptr, total);
        });

        std::memcpy(rdt.get(),serializer->getBinaryStream(), serializer->byteCount());

        ResponseData responseData;
        responseData.data = std::move(rdt);
        responseData.set_success(true);
        responseData.isSuccess = true;
        responseData.total_size = count;
        return responseData;
    }

    uint64_t registry::get_remote_functions() {
        return this->concurrent_dictionary.size();
    }
}