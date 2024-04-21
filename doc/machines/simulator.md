

```cpp
HOST_DELAY_MATRIX::get_ptr()->initial(muse::simulator::host_delay_type::Unified_Latency,10,30);
CPU_PROCESSING_MATRIX::get_ptr()->initial(1,12);


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
```