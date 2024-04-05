```cpp
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
    if (secp256k1_ec_pubkey_serialize(ctx, serialized_pubkey, &pubkey_len, &pubkey, SECP256K1_EC_UNCOMPRESSED) != 1) {
        std::cerr << "无法序列化公钥" << std::endl;
        secp256k1_context_destroy(ctx);
        return 1;
    }

    fmt::print("public key len {}\n", pubkey_len);

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


    EVP_PKEY *pubkey_pkey = convert_serialized_pubkey(serialized_pubkey, sizeof(serialized_pubkey));
    if (!pubkey_pkey) {
        printf("Error converting serialized public key to EVP_PKEY\n");
        return 1;
    }
    EVP_PKEY *privatekey_pkey = convert_private_key(private_key);
    if (!privatekey_pkey) {
        printf("Error converting private key to EVP_PKEY\n");
        EVP_PKEY_free(pubkey_pkey);
        return 1;
    }

    // Free the EVP_PKEYs
    EVP_PKEY_free(pubkey_pkey);
    EVP_PKEY_free(privatekey_pkey);
```