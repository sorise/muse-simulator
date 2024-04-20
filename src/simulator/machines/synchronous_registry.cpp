#include "simulator/machines/synchronous_registry.hpp"

namespace muse::simulator{
     Controller::Controller(std::function<void(host* caller,BinarySerializer*)>&& f)
    :func(std::move(f)){

    }

    void synchronous_registry::runSafely(const std::string &name, BinarySerializer *serializer) {
        if (check(name)){
            host caller;
            concurrent_dictionary[name]->func(&caller, serializer);
        }
    }

    void synchronous_registry::runEnsured(const std::string &name, BinarySerializer *serializer) {
        host caller;
        concurrent_dictionary[name]->func(&caller, serializer);
    }

    bool synchronous_registry::check(const std::string &name) {
        return concurrent_dictionary.find(name) != concurrent_dictionary.end();
    }

    ResponseData synchronous_registry::convert_result_to_response(BinarySerializer *serializer) {
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
}