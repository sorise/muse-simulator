#include "simulator/db/block.hpp"

namespace muse::simulator{
    [[maybe_unused]] auto block::get_block_hash() const -> uint256 {
        uint256 result = prev_block_hash;
        uint256 temp(0);
        encryption::get_hash_base()->get_hash(&height, sizeof(height), temp.get_data());
        result = result + temp;
        encryption::get_hash_base()->get_hash(&create_time, sizeof(create_time), temp.get_data());
        result = result + temp;
        result = result.get_sha3_256();
        return result;
    }

    block::block()
        :prev_block_hash(0),
         height(0),
         create_time(0) {

    }

    block::block(const block &other)
    :prev_block_hash(other.prev_block_hash)
    ,height(other.height)
    ,create_time(other.create_time){

    }

    block::block(block &&other) noexcept
    :prev_block_hash(std::move(other.prev_block_hash))
    ,height(other.height)
    ,create_time(other.create_time){

    }

    auto block::operator=(const block &other) -> block & {
        if (this != &other){
            this->height = other.height;
            this->create_time = other.create_time;
            this->prev_block_hash = other.prev_block_hash;
        }
        return *this;
    }

    auto block::operator=(block &&other) noexcept -> block & {
        if (this != &other){
            this->height = other.height;
            this->create_time = other.create_time;
            this->prev_block_hash = std::move(other.prev_block_hash);
        }
        return *this;
    }
}