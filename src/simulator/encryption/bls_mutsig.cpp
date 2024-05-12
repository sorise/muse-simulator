#include "simulator/encryption/bls_mutsig.hpp"

namespace muse::chain{

    auto bls_multiple_signatures::get_bls_key() -> std::shared_ptr<EVP_PKEY> {
        return nullptr;
    }
}