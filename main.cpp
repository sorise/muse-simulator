#include <iostream>
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

using namespace muse::timer;
using namespace muse::pool;
using namespace muse::chain;

int main() {

    return 0;
}
