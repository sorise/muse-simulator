#include <iostream>
#include <vector>
#include "simulator/machines/computer.hpp"
#include "timer/timer_wheel.hpp"
#include "timer/timer_tree.hpp"
#include "thread_pool/pool.hpp"
#include "thread_pool/executor_token.h"
#include "secp256k1.h"
#include "secp256k1_schnorrsig.h"
#include "openssl/rand.h"

using namespace muse::timer;
using namespace muse::pool;

int main() {
    // 创建 secp256k1 上下文
    secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);

    // 生成随机私钥
    unsigned char private_key[32];
    if (RAND_bytes(private_key, sizeof(private_key)) != 1) {
        std::cerr << "无法生成随机私钥" << std::endl;
        secp256k1_context_destroy(ctx);
        return 1;
    }

    // 生成公钥
    secp256k1_pubkey pubkey;
    if (secp256k1_ec_pubkey_create(ctx, &pubkey, private_key) != 1) {
        std::cerr << "无法生成公钥" << std::endl;
        secp256k1_context_destroy(ctx);
        return 1;
    }

    // 将公钥序列化
    unsigned char serialized_pubkey[65];
    size_t pubkey_len = sizeof(serialized_pubkey);
    if (secp256k1_ec_pubkey_serialize(ctx, serialized_pubkey, &pubkey_len, &pubkey, SECP256K1_EC_COMPRESSED) != 1) {
        std::cerr << "无法序列化公钥" << std::endl;
        secp256k1_context_destroy(ctx);
        return 1;
    }

    // 输出私钥和公钥
    std::cout << "private key: ";
    for (int i = 0; i < sizeof(private_key); ++i) {
        std::cout << std::hex << (int)private_key[i];
    }
    std::cout << std::endl;

    std::cout << "public key: ";
    for (int i = 0; i < pubkey_len; ++i) {
        std::cout << std::hex << (int)serialized_pubkey[i];
    }
    std::cout << std::endl;

    // 释放上下文
    secp256k1_context_destroy(ctx);

    return 0;
}
