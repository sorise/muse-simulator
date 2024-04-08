#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <assert.h>
#include "simulator/machines/computer.hpp"
#include "simulator/encryption/hash_handler.hpp"
#include "simulator/encryption/ecc_secp256k1.hpp"
#include "simulator/encryption/hash_handler.hpp"
#include "timer/timer_wheel.hpp"
#include "timer/timer_tree.hpp"
#include "thread_pool/pool.hpp"
#include "thread_pool/executor_token.h"

#include <secp256k1.h>
#include <secp256k1_extrakeys.h>
#include <secp256k1_schnorrsig.h>
#include <secp256k1_recovery.h>
#include <secp256k1_musig.h>
#include <secp256k1_ecdh.h>

#include <openssl/rand.h>
#include "utils/singleton.hpp"

using namespace muse::timer;
using namespace muse::pool;
using namespace muse::chain;

struct Person{
private:
    mutable std::shared_mutex mtx;
    int age{0};
public:
    [[nodiscard]] int get_age() const{
        std::shared_lock<std::shared_mutex> lock(this->mtx);
        return age;
    }

    void set_age(const int& _age){
        std::lock_guard<std::shared_mutex> lck(mtx);
        this->age = _age;
    }

    ~Person(){
        fmt::print("Person destructor!\n");
    }

    Person(): mtx(){
        fmt::print("Person create!\n");
    }
};


int main() {


    return 0;
}
