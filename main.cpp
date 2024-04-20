#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <assert.h>

#include "simulator/machines/computer.hpp"
#include "simulator/machines/message.hpp"
#include "simulator/machines/host_delay_matrix.hpp"
#include "simulator/machines/network_card.hpp"
#include "simulator/machines/transmitter_event.hpp"
#include "simulator/machines/synchronous_registry.hpp"
#include "simulator/machines/cpu_processing_matrix.hpp"

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
#include "utils/singleton.hpp"
#include "serializer/binarySerializer.h"
#include "serializer/IbinarySerializable.h"

using namespace muse::timer;
using namespace muse::pool;
using namespace muse::chain;

int main() {

    HOST_DELAY_MATRIX::get_ptr()->initial(muse::simulator::host_delay_type::Unified_Latency,15,30);
    CPU_PROCESSING_MATRIX::get_ptr()->initial(1,12);

    auto sin__ = muse::simulator::singleton_lazy_heap<muse::simulator::host_delay_matrix>::get_ptr();


    muse::simulator::host me("159.56.17.52", 2621440ull, 3.5, 8);
    muse::simulator::host other("159.56.17.53", 262144ull, 3.5, 8);


    auto* ev = muse::simulator::new_by_pool<muse::simulator::TransmitterEvent>("159.56.17.52", 2600);
    ev->call<int>("rpc::vote",150,500,std::string("i wanna fuck you father today!"));
    ev->set_callBack([](muse::simulator::Outcome<int> outcome){
        if (outcome.isOK()){

        }
    });

    auto* ev2 = muse::simulator::new_by_pool<muse::simulator::TransmitterEvent>("159.56.17.52", 2600);
    ev2->call<int>("rpc::vote",150,500,std::string("i wanna fuck you father today!"));

    auto m1 = create_message_factory(&me, ev);
    auto m2 = create_message_factory(&other, ev2);

    muse::simulator::network_card nc(2621440ull);


    nc.add_task(m1);
    nc.add_task(m2);

    uint64_t tick = 0;

    while (tick < 50){
        nc.next_tick(tick);
        tick++;
    };

    bool success = true;
    muse::simulator::simulator_event s_event = muse::simulator::simulator_net_event_queue::pop_event(success);
    while (success){
        if (s_event.event_type_ == muse::simulator::simulator_net_event_type::RPC_REQUEST_FINISH){

        }else if(s_event.event_type_ == muse::simulator::simulator_net_event_type::RPC_RESPONSE_FINISH){

        }else{

        }
        s_event = muse::simulator::simulator_net_event_queue::pop_event(success);
    }



    delete_message_factory(m1);
    delete_message_factory(m2);

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
