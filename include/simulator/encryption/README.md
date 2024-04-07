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

### 创建秘钥

```cpp
unsigned char private_key[32];
//生成一个随机私钥
RAND_bytes(private_key, sizeof(private_key));
```

通过私钥创建公钥,**secp256k1_ec_pubkey_create** 和 **secp256k1_keypair_create** 函数都是用于生成 **secp256k1** 椭圆曲线加密中的公钥的函数。
只需要公钥使用前者，两个都要使用后者。
```cpp
// 生成公钥
secp256k1_pubkey pubkey;
if (secp256k1_ec_pubkey_create(ctx, &pubkey, private_key) != 1) {
    std::cerr << "无法生成公钥" << std::endl;
    secp256k1_context_destroy(ctx);
    return 1;
}
```
生成密钥对
```cpp
//生成密钥对
secp256k1_keypair pair;
secp256k1_keypair_create(ctx, &pair, private_key);
```

签名例子
```cpp
static void print_hex(unsigned char* data, size_t size) {
    size_t i;
    printf("0x");
    for (i = 0; i < size; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

unsigned char randomize[32];
secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);

if (!RAND_bytes(randomize, sizeof(randomize))) {
    printf("Failed to generate randomness\n");
    return 1;
}
/* Randomizing the context is recommended to protect against side-channel
 * leakage See `secp256k1_context_randomize` in secp256k1.h for more
 * information about it. This should never fail. */
auto return_val = secp256k1_context_randomize(ctx, randomize);
assert(return_val);
unsigned char private_key[32];
secp256k1_keypair keypair;
//创建密钥对
while (true) {
    if (!RAND_bytes(private_key, sizeof(private_key))) {
        printf("Failed to generate randomness\n");
        return 1;
    }
    /* Try to create a keypair with a valid context, it should only fail if
     * the secret key is zero or out of range. */
    if (secp256k1_keypair_create(ctx, &keypair, private_key)) {
        break;
    }
}

printf("private_key: ");
print_hex(private_key, sizeof(private_key));

secp256k1_xonly_pubkey pubkey; //公钥部分
return_val = secp256k1_keypair_xonly_pub(ctx, &pubkey, NULL, &keypair);
assert(return_val);

unsigned char serialized_pubkey[32]; //序列化后的公钥
return_val = secp256k1_xonly_pubkey_serialize(ctx, serialized_pubkey, &pubkey);
assert(return_val);

unsigned char msg[13] = "Hello World!";
unsigned char msg_hash[32];
//muse::chain::hash_handler::sha3_256(message.data(), message.size(), msgHash);

unsigned char auxiliary_rand[32];
/* Generate 32 bytes of randomness to use with BIP-340 schnorr signing. */
if (!RAND_bytes(auxiliary_rand, sizeof(auxiliary_rand))) {
    printf("Failed to generate randomness\n");
    return 1;
}

unsigned char tag[18] = "my_fancy_protocol";
return_val = secp256k1_tagged_sha256(ctx, msg_hash, tag, sizeof(tag), msg, sizeof(msg));
assert(return_val);

printf("Hash Key: ");
print_hex(msg_hash, sizeof(msg_hash));

unsigned char signature[64];
return_val = secp256k1_schnorrsig_sign32(ctx, signature, msg_hash, &keypair, auxiliary_rand);
assert(return_val);

if (!secp256k1_xonly_pubkey_parse(ctx, &pubkey, serialized_pubkey)) {
    printf("Failed parsing the public key\n");
    return 1;
}

auto is_signature_valid = secp256k1_schnorrsig_verify(ctx, signature, msg_hash, 32, &pubkey);


printf("Is the signature valid? %s\n", is_signature_valid ? "true" : "false");
printf("Secret Key: ");
print_hex(private_key, sizeof(private_key));
printf("Public Key: ");
print_hex(serialized_pubkey, sizeof(serialized_pubkey));
printf("Signature: ");
print_hex(signature, sizeof(signature));

/* This will clear everything from the context and free the memory */
secp256k1_context_destroy(ctx);

auto is_signature_valid2 = secp256k1_schnorrsig_verify(secp256k1_context_static,
                                                  signature, msg_hash, 32, &pubkey);
assert(is_signature_valid2 == is_signature_valid);
```

```cpp
secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
secp256k1_keypair keypair;
unsigned char private_key[32];
std::shared_ptr<EVP_PKEY> key_pair = nullptr;
//创建密钥对
while (true) {
    key_pair = muse::chain::ecc_secp256k1::get_key();
    auto pri32 = muse::chain::ecc_secp256k1::convert_private_key_32B(key_pair.get());

    for (int i = 0; i < 32; ++i) {
        private_key[i] = pri32[i];
    }
    /* Try to create a keypair with a valid context, it should only fail if
     * the secret key is zero or out of range. */
    if (secp256k1_keypair_create(ctx, &keypair, private_key)) {
        break;
    }
}

auto pub_str = muse::chain::ecc_secp256k1::convert_public_key_no_compressed_64B(key_pair.get());

fmt::print("pub {} {}\n", pub_str.size(), hash_handler::convert_to_hex(reinterpret_cast<const unsigned char *>(pub_str.data()), pub_str.size()));

secp256k1_xonly_pubkey pubkey; //公钥部分
auto return_val = secp256k1_keypair_xonly_pub(ctx, &pubkey, NULL, &keypair);
assert(return_val);

printf("Secret Key: ");
fmt::print("{}\n", hash_handler::convert_to_hex(private_key, sizeof(private_key)));
printf("Public Key: ");
fmt::print("{}\n",hash_handler::convert_to_hex(pubkey.data, sizeof(pubkey.data)));
```

序列化
```cpp
unsigned char pub_key_bytes[65];
size_t pub_key_len = sizeof(pub_key_bytes);
if (!secp256k1_ec_pubkey_serialize(ctx, pub_key_bytes, &pub_key_len, &py, SECP256K1_EC_UNCOMPRESSED)) {
    std::cerr << "Error: Failed to serialize secp256k1 public key" << std::endl;
    secp256k1_context_destroy(ctx);
    return 1;
}
printf("bitcoin serialize public: ");
fmt::print("{}\n",hash_handler::convert_to_hex(pub_key_bytes, pub_key_len));
```

在libsecp256k1库中，如果你想将序列化的公钥（通常是压缩或未压缩格式的字节串）还原为其原始的secp256k1_pubkey结构体以便进一步进行椭圆曲线相关的操作，你需要使用secp256k1_ec_pubkey_parse函数。

```cpp
secp256k1_pubkey parsed_pubkey;
size_t input_len = ...; // 此处应替换为实际序列化公钥的长度
const unsigned char* serialized_pubkey = ...; // 此处应替换为实际的序列化公钥字节数组

// 解析公钥
int ret = secp256k1_ec_pubkey_parse(context, &parsed_pubkey, serialized_pubkey, input_len);

if (ret == 1) {
    // 公钥已成功解析
} else {
    // 解析失败，可能是因为输入数据无效
}
```

### 测试代码
```cpp
int test(){
    // 使用 OpenSSL 生成密钥对
    EC_KEY *ec_key = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (!EC_KEY_generate_key(ec_key)) {
        std::cerr << "Error: Failed to generate OpenSSL key pair" << std::endl;
        return 1;
    }

    // 提取私钥
    const BIGNUM *priv_key_bn = EC_KEY_get0_private_key(ec_key);
    if (!priv_key_bn) {
        std::cerr << "Error: Failed to get OpenSSL private key" << std::endl;
        EC_KEY_free(ec_key);
        return 1;
    }

    unsigned char private_key[32];
    BN_bn2bin(priv_key_bn, private_key);

    // 使用 Bitcoin 的 secp256k1 库创建密钥对
    secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
    if (!ctx) {
        std::cerr << "Error: Failed to create secp256k1 context" << std::endl;
        EC_KEY_free(ec_key);
        return 1;
    }

    secp256k1_keypair keypair;
    if (!secp256k1_keypair_create(ctx, &keypair, private_key)) {
        std::cerr << "Error: Failed to create secp256k1 key pair" << std::endl;
        secp256k1_context_destroy(ctx);
        EC_KEY_free(ec_key);
        return 1;
    }

    secp256k1_pubkey pubkey_;
    secp256k1_keypair_pub(ctx, &pubkey_, &keypair);

    // 获取 Bitcoin 的公钥
    unsigned char pub_key_bytes[65];
    size_t pub_key_len = sizeof(pub_key_bytes);
    if (!secp256k1_ec_pubkey_serialize(ctx, pub_key_bytes, &pub_key_len, &pubkey_, SECP256K1_EC_UNCOMPRESSED)) {
        std::cerr << "Error: Failed to serialize secp256k1 public key" << std::endl;
        secp256k1_context_destroy(ctx);
        EC_KEY_free(ec_key);
        return 1;
    }

    // 获取 OpenSSL 的公钥
    const EC_POINT *pub_key_point = EC_KEY_get0_public_key(ec_key);
    if (!pub_key_point) {
        std::cerr << "Error: Failed to get OpenSSL public key" << std::endl;
        secp256k1_context_destroy(ctx);
        EC_KEY_free(ec_key);
        return 1;
    }

    unsigned char openssl_pub_key_bytes[65];
    size_t openssl_pub_key_len = EC_POINT_point2oct(EC_KEY_get0_group(ec_key), pub_key_point, POINT_CONVERSION_UNCOMPRESSED, openssl_pub_key_bytes, sizeof(openssl_pub_key_bytes), NULL);
    if (openssl_pub_key_len == 0) {
        std::cerr << "Error: Failed to serialize OpenSSL public key" << std::endl;
        secp256k1_context_destroy(ctx);
        EC_KEY_free(ec_key);
        return 1;
    }

    // 检查公钥是否相同
    if (pub_key_len == openssl_pub_key_len && memcmp(pub_key_bytes, openssl_pub_key_bytes, pub_key_len) == 0) {
        std::cout << "Public keys match!" << std::endl;
    } else {
        std::cout << "Public keys do not match!" << std::endl;
    }

    // 释放资源
    secp256k1_context_destroy(ctx);
    EC_KEY_free(ec_key);
    return 0;
}

int mytest(){
    //环境
    secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
    secp256k1_keypair keypair;

    //私钥
    unsigned char private_key[32];
    std::shared_ptr<EVP_PKEY> key_pair = nullptr;
    //创建密钥对
    while (true) {
        key_pair = muse::chain::ecc_secp256k1::get_key();
        auto pri32 = muse::chain::ecc_secp256k1::convert_private_key_32B(key_pair.get());

        for (int i = 0; i < 32; ++i) {
            private_key[i] = pri32[i];
        }

        if (secp256k1_keypair_create(ctx, &keypair, private_key)) {
            break;
        }
    }
    //输出私钥
    auto pri32 = muse::chain::ecc_secp256k1::convert_private_key_32B(key_pair.get());

    fmt::print("openssl private key: {}\n",hash_handler::convert_to_hex(private_key, sizeof(private_key)));

    //输出公钥
    auto pub_str = muse::chain::ecc_secp256k1::convert_public_key_no_compressed_64B(key_pair.get());

    fmt::print("openssl Public Key: {}\n", hash_handler::convert_to_hex(reinterpret_cast<const unsigned char *>(pub_str.data()), pub_str.size()));

    secp256k1_pubkey py;
    auto rv = secp256k1_keypair_pub(ctx, &py, &keypair);
    assert(rv);
    printf("bitcoin pub key: ");
    fmt::print("{}\n",hash_handler::convert_to_hex(py.data, sizeof(py.data)));

    secp256k1_xonly_pubkey xonly_pubkey {"\0"}; //公钥部分
    auto return_val = secp256k1_keypair_xonly_pub(ctx, &xonly_pubkey, NULL, &keypair);
    assert(return_val);

    printf("bitcoin xonly public: ");
    fmt::print("{}\n",hash_handler::convert_to_hex(xonly_pubkey.data, sizeof(xonly_pubkey.data)));


    unsigned char pub_key_bytes[65] {'\0'};
    size_t pub_key_len = sizeof(pub_key_bytes);
    if (!secp256k1_ec_pubkey_serialize(ctx, pub_key_bytes, &pub_key_len, &py, SECP256K1_EC_UNCOMPRESSED)) {
        std::cerr << "Error: Failed to serialize secp256k1 public key" << std::endl;
        secp256k1_context_destroy(ctx);
        return 1;
    }
    printf("bitcoin serialize public: ");
    fmt::print("{}\n",hash_handler::convert_to_hex(pub_key_bytes, pub_key_len));

    secp256k1_context_destroy(ctx);
    return 0;
}
```

### 例子

```cpp
auto ecc_key_pair = muse::chain::ecc_secp256k1::get_key();

secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);

if (!ctx) {
    throw std::runtime_error("Error: Failed to create secp256k1 context.\n");
}

secp256k1_pubkey pubkey;

auto pub_str = muse::chain::ecc_secp256k1::convert_public_key_no_compressed_64B(ecc_key_pair.get());
if (pub_str.empty()){
    throw std::runtime_error("Error: Failed to get convert_public_key_no_compressed_64B result.\n");
}
secp256k1_ec_pubkey_parse(ctx, &pubkey, reinterpret_cast<unsigned char*>(pub_str.data()), pub_str.size());


auto pri_str = muse::chain::ecc_secp256k1::convert_private_key_32B(ecc_key_pair.get());

secp256k1_keypair keypair;

secp256k1_keypair_create(ctx, &keypair, reinterpret_cast<unsigned char *>(pri_str.data()));

// 要签名的消息
const char *message = "Hello, Schnorr!";

// 计算消息的哈希值
unsigned char hash[32];
muse::chain::hash_handler::sha3_256(message, strlen(message), hash);

// 计算签名
unsigned char signature[64];

//随机数使用
unsigned char aux_rand[32] {"rand number"}; //随机数

if (!secp256k1_schnorrsig_sign32(ctx, signature, hash, &keypair, aux_rand)) {
    printf("Error: Failed to sign message\n");
    secp256k1_context_destroy(ctx);
    return 1;
}

// 打印签名结果
printf("Signature: ");
for (int i = 0; i < 64; ++i) {
    printf("%02x", signature[i]);
}
printf("\n");

secp256k1_xonly_pubkey xo_pub;
auto return_val = secp256k1_keypair_xonly_pub(ctx, &xo_pub, NULL, &keypair);
assert(return_val);

auto is_signature_valid = secp256k1_schnorrsig_verify(ctx,signature, hash, 32, &xo_pub);

printf("Is the signature valid? %s\n", is_signature_valid ? "true" : "false");
secp256k1_context_destroy(ctx);
```


### 参考文献
* [Schnorr协议：非交互零知识身份证明和数字签名](https://zhuanlan.zhihu.com/p/107752440)
* [MuSig2：Schnorr 的多签名](https://www.btcstudy.org/2022/01/18/schnorr-musig2-by-popeller/)
* [数字化契约如何守护？解析聚合签名的妙用](https://zhuanlan.zhihu.com/p/165375205)