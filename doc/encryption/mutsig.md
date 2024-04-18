## 聚合签名实现

```cpp
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

struct signer_secrets {
    secp256k1_keypair keypair;
    secp256k1_musig_secnonce sec_nonce; // 秘密随机数
};

struct signer {
    secp256k1_pubkey pubkey;
    secp256k1_musig_pubnonce pub_nonce; //公共随机数
    secp256k1_musig_partial_sig partial_sig;
};

/* Create a key pair, store it in signer_secrets->keypair and signer->pubkey */
static int create_keypair(const secp256k1_context* ctx, struct signer_secrets *signer_secrets, struct signer *signer) {
    unsigned char seckey[32];
    while (1) {
        if (!RAND_bytes(seckey, sizeof(seckey))) {
            printf("Failed to generate randomness\n");
            return 1;
        }
        if (secp256k1_keypair_create(ctx, &signer_secrets->keypair, seckey)) {
            break;
        }
    }
    if (!secp256k1_keypair_pub(ctx, &signer->pubkey, &signer_secrets->keypair)) {
        return 0;
    }
    return 1;
}

/* Tweak the pubkey corresponding to the provided keyagg cache, update the cache
 * and return the tweaked aggregate pk. */
static int tweak(const secp256k1_context* ctx, secp256k1_xonly_pubkey *agg_pk, secp256k1_musig_keyagg_cache *cache) {
    secp256k1_pubkey output_pk;
    unsigned char plain_tweak[32] = "this could be a BIP32 tweak....";
    unsigned char xonly_tweak[32] = "this could be a taproot tweak..";

    /* Plain tweaking which, for example, allows deriving multiple child
       public keys from a single aggregate key using BIP32 */
    if (!secp256k1_musig_pubkey_ec_tweak_add(ctx, NULL, cache, plain_tweak)) {
        return 0;
    }
    /* Note that we did not provided an output_pk argument, because the
     * resulting pk is also saved in the cache and so if one is just interested
     * in signing the output_pk argument is unnecessary. On the other hand, if
     * one is not interested in signing, the same output_pk can be obtained by
     * calling `secp256k1_musig_pubkey_get` right after key aggregation to get
     * the full pubkey and then call `secp256k1_ec_pubkey_tweak_add`. */

    /* Xonly tweaking which, for example, allows creating taproot commitments */
    if (!secp256k1_musig_pubkey_xonly_tweak_add(ctx, &output_pk, cache, xonly_tweak)) {
        return 0;
    }
    /* Note that if we wouldn't care about signing, we can arrive at the same
     * output_pk by providing the untweaked public key to
     * `secp256k1_xonly_pubkey_tweak_add` (after converting it to an xonly pubkey
     * if necessary with `secp256k1_xonly_pubkey_from_pubkey`). */

    /* Now we convert the output_pk to an xonly pubkey to allow to later verify
     * the Schnorr signature against it. For this purpose we can ignore the
     * `pk_parity` output argument; we would need it if we would have to open
     * the taproot commitment. */
    if (!secp256k1_xonly_pubkey_from_pubkey(ctx, agg_pk, NULL, &output_pk)) {
        return 0;
    }
    return 1;
}

#define N_SIGNERS 3

/* Sign a message hash with the given key pairs and store the result in sig */
static int sign(const secp256k1_context* ctx, struct signer_secrets *signer_secrets, struct signer *signer, const secp256k1_musig_keyagg_cache *cache, const unsigned char *msg32, unsigned char *sig64) {
    int i;
    const secp256k1_musig_pubnonce *pubnonces[N_SIGNERS];
    const secp256k1_musig_partial_sig *partial_sigs[N_SIGNERS];

    /* The same for all signers */
    secp256k1_musig_session session;

    //生成随机数，需要交换随机数
    for (i = 0; i < N_SIGNERS; i++) {
        unsigned char seckey[32];
        unsigned char session_id[32];

        /* Create random session ID. It is absolutely necessary that the session ID is
         * unique for every call of secp256k1_musig_nonce_gen. Otherwise it's trivial
         * for an attacker to extract the secret key! */
        /* 会话 ID */
        if (!RAND_bytes(session_id, sizeof(session_id))) {
            return 0;
        }
        /* 得到私钥 */
        if (!secp256k1_keypair_sec(ctx, seckey, &signer_secrets[i].keypair)) {
            return 0;
        }
        /* 初始化回话，创建秘密密钥和公共密钥 */
        /* Initialize session and create secret nonce for signing and public nonce to send to the other signers. */
        if (!secp256k1_musig_nonce_gen(ctx, &signer_secrets[i].sec_nonce, &signer[i].pub_nonce, session_id, seckey, &signer[i].pubkey, msg32, nullptr, nullptr)) {
            return 0;
        }
        /*  */
        pubnonces[i] = &signer[i].pub_nonce;
    }

    /* Communication round 1: A production system would exchange public nonces here before moving on. */
    /* 正式系统，需要提前交换公共随机数 */
    for (i = 0; i < N_SIGNERS; i++) {
        secp256k1_musig_aggnonce agg_pubnonce;

        /* Create aggregate nonce and initialize the session */
        if (!secp256k1_musig_nonce_agg(ctx, &agg_pubnonce, pubnonces, N_SIGNERS)) {
            return 0;
        }
        if (!secp256k1_musig_nonce_process(ctx, &session, &agg_pubnonce, msg32, cache, NULL)) {
            return 0;
        }
        /* partial_sign will clear the secnonce by setting it to 0. That's because
         * you must _never_ reuse the secnonce (or use the same session_id to
         * create a secnonce). If you do, you effectively reuse the nonce and
         * leak the secret key. */
        if (!secp256k1_musig_partial_sign(ctx, &signer[i].partial_sig, &signer_secrets[i].sec_nonce, &signer_secrets[i].keypair, cache, &session)) {
            return 0;
        }
        partial_sigs[i] = &signer[i].partial_sig;
    }

    /* Communication round 2: A production system would exchange
     * partial signatures here before moving on. */
    for (i = 0; i < N_SIGNERS; i++) {
        /*
          To check whether signing was successful, it suffices to either verify
          the aggregate signature with the aggregate public key using
          secp256k1_schnorrsig_verify, or verify all partial signatures of all
          signers individually. Verifying the aggregate signature is cheaper but
          verifying the individual partial signatures has the advantage that it
          can be used to determine which of the partial signatures are invalid
          (if any), i.e., which of the partial signatures cause the aggregate
          signature to be invalid and thus the protocol run to fail. It's also
          fine to first verify the aggregate sig, and only verify the individual
          sigs if it does not work.
         */
        if (!secp256k1_musig_partial_sig_verify(ctx, &signer[i].partial_sig, &signer[i].pub_nonce, &signer[i].pubkey, cache, &session)) {
            return 0;
        }
    }
    return secp256k1_musig_partial_sig_agg(ctx, sig64, &session, partial_sigs, N_SIGNERS);
}

int main(void) {
    secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
    struct signer_secrets signer_secrets[N_SIGNERS];
    struct signer signers[N_SIGNERS];
    const secp256k1_pubkey *pubkeys_ptr[N_SIGNERS];

    /* Create a secp256k1 context */
    printf("Creating key pairs......");
    for (int i = 0; i < N_SIGNERS; i++) {
        if (!create_keypair(ctx, &signer_secrets[i], &signers[i])) {
            printf("FAILED\n");
            return 1;
        }
        pubkeys_ptr[i] = &signers[i].pubkey;
    }
    printf("ok\n");



    unsigned char msg[32] = "this_could_be_the_hash_of_a_msg"; //签名消息哈希
    unsigned char sig[64]; //最终聚合签名

    secp256k1_musig_keyagg_cache cache; //密钥聚合缓存
    printf("Combining public keys...");

    secp256k1_xonly_pubkey agg_pk; //聚合公钥
    /* If you just want to aggregate and not sign the cache can be NULL */
    if (!secp256k1_musig_pubkey_agg(ctx, nullptr, &agg_pk, &cache, pubkeys_ptr, N_SIGNERS)) {
        printf("FAILED\n");
        return 1;
    }

    printf("ok\n");
    fmt::print("agg before: {}\n", hash_handler::convert_to_hex(agg_pk.data, 64));
    printf("Tweaking................");
    /* 聚合公钥调整，也就是生成子秘钥，可选 */
    if (!tweak(ctx, &agg_pk, &cache)) {
        printf("FAILED\n");
        return 1;
    }
    fmt::print("agg after: {}\n", hash_handler::convert_to_hex(agg_pk.data, 64));

    printf("ok\n");
    printf("Signing message.........");
    if (!sign(ctx, signer_secrets, signers, &cache, msg, sig)) {
        printf("FAILED\n");
        return 1;
    }

    printf("ok\n");
    printf("Verifying signature.....");
    if (!secp256k1_schnorrsig_verify(ctx, sig, msg, 32, &agg_pk)) {
        printf("FAILED\n");
        return 1;
    }
    printf("ok\n");
    secp256k1_context_destroy(ctx);
    return 0;
}

```


### rpc
```cpp
    host ht;
    synchronous_registry registry;

    registry.Bind("rpc::vote",&host::vote);
    registry.Bind("rpc::vote_hash",&host::vote_hash);
    registry.Bind("rpc::add",&host::add);


    TransmitterEvent event("127.0.0.1",8080);
    event.call<int>("rpc::add", 15 , 25);
    event.set_callBack([](Outcome<int> t){      //设置回调
        if (t.isOK()){
            printf("OK lambda %d \n", t.value);
        }else{
            printf("fail lambda\n");
        }
    });

    network_dispatcher dispatcher;
    dispatcher.RPC_Request(std::move(event));

    event.get_serializer().reset();

    std::string rpc_name;
    event.get_serializer().output(rpc_name);
    std::cout << rpc_name << std::endl;
    registry.runEnsured("rpc::add", &(event.get_serializer()));

    ResponseData response = synchronous_registry::convert_result_to_response(&(event.get_serializer()));


    event.trigger_callBack(&response);
```