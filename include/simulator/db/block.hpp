//
// Created by 14270 on 2024-04-08.
//

#ifndef MUSE_SIMULATOR_BLOCK_HPP
#define MUSE_SIMULATOR_BLOCK_HPP
#include "serializer/IbinarySerializable.h"
#include "simulator/encryption/uint256.hpp"
#include "simulator/encryption/encryption.hpp"
#include "utils/toolkits.hpp"

using namespace muse::chain;

namespace muse::simulator {
    /* 模拟一个区块 */
    class SIMULATOR_CPP_WIN_API block : public muse::serializer::IBinarySerializable{
    public:
        uint256 prev_block_hash;              // 前区块hash值
        uint64_t height;         // 区块高度
        uint64_t create_time;    // 创建时间
        uint64_t transaction_count; //区块内交易数量
        uint64_t block_size;        //区块大小
        block();

        block(const block& other);

        block(block&& other) noexcept;

        auto operator=(const block& other) -> block&;

        auto operator=(block&& other) noexcept -> block&;

        [[nodiscard]] auto get_block_hash() const -> uint256;

        MUSE_IBinarySerializable(
            prev_block_hash,
            height,
            create_time,
            transaction_count,
            block_size
        )

        virtual ~block() = default;
    };
}

#endif //MUSE_SIMULATOR_BLOCK_HPP
