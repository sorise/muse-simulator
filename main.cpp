#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <assert.h>

#include "simulator/machines/computer.hpp"
#include "simulator/machines/central_processing_unit.hpp"
#include "simulator/machines/message.hpp"
#include "simulator/machines/host_delay_matrix.hpp"
#include "simulator/machines/network_card.hpp"
#include "simulator/machines/transmitter_event.hpp"
#include "simulator/machines/synchronous_registry.hpp"
#include "simulator/machines/cpu_processing_matrix.hpp"
#include "simulator/machines/central_processing_unit.hpp"

#include "simulator/network_dispatcher.hpp"
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
#include <queue>
#include "utils/singleton.hpp"
#include "serializer/binarySerializer.h"
#include "serializer/IbinarySerializable.h"

using namespace muse::timer;
using namespace muse::pool;
using namespace muse::chain;

int main() {
    //muse::simulator::central_processing_unit cpu(8);
    muse::simulator::central_processing_unit cpu(8);
    cpu.print_core_state(1);


    /*
    HOST_DELAY_MATRIX::get_ptr()->initial(muse::simulator::host_delay_type::Unified_Latency,10,30);
    CPU_PROCESSING_MATRIX::get_ptr()->initial(1,12);
    */



    //nc.add_task()


//    network_card nc(2621440ull); // 2.5MB/s = 10Mbps
//
//    network_card_task task1(nullptr,100,5,15);
//    network_card_task task2(nullptr,100,15,25);
//    network_card_task task3(nullptr,120,18,25);
//    network_card_task task4(nullptr,120,18,75);
//
//    std::map<network_card_task, bool> sending_tasks;
//
//    sending_tasks.emplace(task1, true);
//    sending_tasks.emplace(task2, true);
//    sending_tasks.emplace(task3, true);
//    sending_tasks.emplace(task4, true);
//
//    for (auto& [k,v]: sending_tasks) {
//        fmt::print("{}:{}\n",k.get_start_ms(), k.get_end_ms());
//    }

//    host *h1 = new host("159.52.112.10", 2147483648ll * 8l, 3.5, 8);
//    host *h2 = new host("159.52.112.11", 2147483648ll * 8l, 3.5, 8);
//
//    network_dispatcher net;
//    net.register_host(h1);
//    net.register_host(h2);
//
//
//
//    delete h1, h2;

    return 0;
}
