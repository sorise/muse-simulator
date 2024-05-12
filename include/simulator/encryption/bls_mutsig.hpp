#ifndef MUSE_SIMULATOR_BLS_MUTSIG_HPP
#define MUSE_SIMULATOR_BLS_MUTSIG_HPP

#include "openssl/rand.h"
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/pem.h>

#include <memory>
#include "utils/toolkits.hpp"

namespace muse::chain {
    class SIMULATOR_CPP_WIN_API bls_multiple_signatures {
        static auto get_bls_key() -> std::shared_ptr<EVP_PKEY>;
    };
}


#endif //MUSE_SIMULATOR_BLS_MUTSIG_HPP
