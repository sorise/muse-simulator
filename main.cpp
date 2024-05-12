
#include <iostream>
#include <climits>
#include <cassert>
#include <vector>
#include <map>

#include "utils/singleton.hpp"
#include "simulator/simulator.hpp"
#include "simulator/machines/host.hpp"
#include "simulator/machines/registry.hpp"
#include "simulator/machines/ipv4_address.hpp"
#include "simulator/machines/computer_simulator_timer.hpp"

#include "simulator/encryption/ecc_secp256k1.hpp"
#include "simulator/encryption/bitcoin_secp256k1.hpp"


int main(int argc, char *argv[]) {
    auto keypair = muse::chain::ecc_secp256k1::get_key();

    //签名消息
    const unsigned char message[64] = {"this is test data!"};


    return 0;
}