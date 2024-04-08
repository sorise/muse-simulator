#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <assert.h>

#include "simulator/machines/computer.hpp"
#include "simulator/encryption/hash_handler.hpp"
#include "simulator/encryption/ecc_secp256k1.hpp"
#include "simulator/encryption/hash_handler.hpp"
#include "simulator/db/block.hpp"

#include "thread_pool/pool.hpp"
#include "thread_pool/executor_token.h"

#include "timer/timer_wheel.hpp"
#include "timer/timer_tree.hpp"


#include <secp256k1.h>
#include <secp256k1_extrakeys.h>
#include <secp256k1_schnorrsig.h>
#include <secp256k1_recovery.h>
#include <secp256k1_musig.h>
#include <secp256k1_ecdh.h>

#include <openssl/rand.h>
#include "utils/singleton.hpp"
#include "serializer/binarySerializer.h"
#include "serializer/IbinarySerializable.h"

using namespace muse::timer;
using namespace muse::pool;
using namespace muse::chain;

struct Person: public muse::serializer::IBinarySerializable{
private:
    mutable std::shared_mutex mtx;
    int age{0};
public:
    MUSE_IBinarySerializable(age);

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
    muse::serializer::BinarySerializer serializer;
    muse::simulator::block blk;
    blk.height = 1;
    blk.prev_block_hash = uint256("0x26a1a2bc4");
    blk.transaction_count = 100;
    blk.create_time = 123456789;
    blk.block_size = 250 + 100 * 250;

    serializer.inputArgs(blk);

    serializer.saveToFile("F:\\db\\me\\su\\1.bin");
    return 0;
}
