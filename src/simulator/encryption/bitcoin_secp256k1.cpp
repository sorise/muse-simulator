#include "simulator/encryption/bitcoin_secp256k1.hpp"

namespace muse::simulator{
    extern EVP_PKEY*  convert_serialized_pubkey_no_compressed(unsigned char *serialized_pubkey, size_t serialized_pubkey_len) {
        EVP_PKEY *pkey = NULL;
        EC_KEY *ec_key = NULL;

        ec_key = EC_KEY_new_by_curve_name(NID_secp256k1);
        if (!ec_key) {
            printf("Error creating EC_KEY\n");
            return NULL;
        }

        EC_GROUP *group = EC_GROUP_new_by_curve_name(NID_secp256k1);
        if (!group) {
            printf("Error creating EC_GROUP\n");
            EC_KEY_free(ec_key);
            return NULL;
        }

        EC_POINT *point = EC_POINT_new(group);
        if (!point) {
            printf("Error creating EC_POINT\n");
            EC_KEY_free(ec_key);
            EC_GROUP_free(group);
            return NULL;
        }

        if (!EC_POINT_oct2point(group, point, serialized_pubkey, serialized_pubkey_len, NULL)) {
            printf("Error converting octet string to EC_POINT\n");
            EC_KEY_free(ec_key);
            EC_GROUP_free(group);
            EC_POINT_free(point);
            return NULL;
        }

        if (!EC_KEY_set_public_key(ec_key, point)) {
            printf("Error setting EC_KEY public key\n");
            EC_KEY_free(ec_key);
            EC_GROUP_free(group);
            EC_POINT_free(point);
            return NULL;
        }

        pkey = EVP_PKEY_new();
        if (!pkey) {
            printf("Error creating EVP_PKEY\n");
            EC_KEY_free(ec_key);
            EC_GROUP_free(group);
            EC_POINT_free(point);
            return NULL;
        }

        if (!EVP_PKEY_assign_EC_KEY(pkey, ec_key)) {
            printf("Error assigning EC_KEY to EVP_PKEY\n");
            EVP_PKEY_free(pkey);
            EC_GROUP_free(group);
            EC_POINT_free(point);
            return NULL;
        }

        EC_GROUP_free(group);
        EC_POINT_free(point);

        return pkey;
    }

    extern EVP_PKEY* convert_private_key(unsigned char *private_key) {
        EVP_PKEY *pkey = EVP_PKEY_new();
        if (!pkey) {
            printf("Error creating EVP_PKEY\n");
            return NULL;
        }

        EC_KEY *ec_key = EC_KEY_new_by_curve_name(NID_secp256k1);
        if (!ec_key) {
            printf("Error creating EC_KEY\n");
            EVP_PKEY_free(pkey);
            return NULL;
        }

        BIGNUM *priv_bn = BN_new();
        if (!priv_bn) {
            printf("Error creating BIGNUM\n");
            EC_KEY_free(ec_key);
            EVP_PKEY_free(pkey);
            return NULL;
        }

        if (!BN_bin2bn(private_key, 32, priv_bn)) {
            printf("Error converting binary private key to BIGNUM\n");
            BN_free(priv_bn);
            EC_KEY_free(ec_key);
            EVP_PKEY_free(pkey);
            return NULL;
        }

        if (!EC_KEY_set_private_key(ec_key, priv_bn)) {
            printf("Error setting EC_KEY private key\n");
            BN_free(priv_bn);
            EC_KEY_free(ec_key);
            EVP_PKEY_free(pkey);
            return NULL;
        }

        if (!EVP_PKEY_assign_EC_KEY(pkey, ec_key)) {
            printf("Error assigning EC_KEY to EVP_PKEY\n");
            BN_free(priv_bn);
            EC_KEY_free(ec_key);
            EVP_PKEY_free(pkey);
            return NULL;
        }

        BN_free(priv_bn);

        return pkey;
    }

}

