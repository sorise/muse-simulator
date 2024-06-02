#ifndef MUSE_SIMULATOR_BITCOIN_SECP256K1_HPP
#define MUSE_SIMULATOR_BITCOIN_SECP256K1_HPP

#ifdef _WIN32
    #ifndef OPENSSL_LINK_ADD
        #define OPENSSL_LINK_ADD 1
        #include "openssl/applink.c"
    #endif
#endif

#ifndef OPENSSL_API_COMPAT
    #define OPENSSL_API_COMPAT 0x10100000L
#endif

#include "secp256k1.h"
#include "secp256k1_schnorrsig.h"
#include "openssl/rand.h"
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include "../utils/toolkits.hpp"

namespace muse::simulator{
    /*
     * Function to convert serialized public key to EVP_PKEY
     * serialized_pubkey_len = 65
     */
    SIMULATOR_CPP_WIN_API EVP_PKEY*  convert_serialized_pubkey_no_compressed(unsigned char *serialized_pubkey, size_t serialized_pubkey_len);


    /* Function to convert private key to EVP_PKEY
     * @private_key = 32 bytes
     * */
    SIMULATOR_CPP_WIN_API EVP_PKEY* convert_private_key(unsigned char *private_key);
}

#endif //MUSE_SIMULATOR_BITCOIN_SECP256K1_HPP
