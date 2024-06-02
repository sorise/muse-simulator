#ifndef MUSE_SIMULATOR_IPV4_ADDRESS_HPP
#define MUSE_SIMULATOR_IPV4_ADDRESS_HPP 1

#include <cstdint>
#include <iostream>
#include <string>
#include <bitset>
#include <stdexcept>

#include "../utils/toolkits.hpp"
#include "../serializer/IbinarySerializable.h"

namespace muse::simulator{
    /* 表示一个IPV4 地址 */
    class SIMULATOR_CPP_WIN_API ipv4_address: public muse::serializer::IBinarySerializable{
    private:
        uint32_t ip_value_; // 用于存储IPv4地址
    public:

        ipv4_address();

        MUSE_IBinarySerializable(ip_value_);

        explicit ipv4_address(const uint32_t & _address): ip_value_(_address) {

        }

        ipv4_address(const uint64_t & _address) = delete;
        ipv4_address(const uint16_t & _address) = delete;
        ipv4_address(const uint8_t & _address) = delete;
        ipv4_address(const int & _address) = delete;

        explicit ipv4_address(const std::string& _address): ip_value_(0) {
            ip_value_ = string_to_ipv4(_address);
        }

        [[nodiscard]] uint32_t get_ip_value() const {
            return this->ip_value_;
        }

        [[nodiscard]] std::string to_string() const {
            return ipv4_to_string(this->ip_value_);
        }

        // 检查是否是专用内网地址
        [[nodiscard]] bool is_private_net_address() const;

        static bool test_ip_address_legal(const std::string& ip) noexcept;

        static uint32_t string_to_ipv4(const std::string& ip);

        static std::string ipv4_to_string(uint32_t _ip_value);
    };
}

#endif //MUSE_SIMULATOR_IPV4_ADDRESS_HPP
