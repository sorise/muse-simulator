#include "simulator/machines/ipv4_address.hpp"

namespace muse::simulator{
    uint32_t ipv4_address::string_to_ipv4(const std::string &ip) {
        uint32_t result = 0; //主机字节序
        auto ip_len = ip.size();
        if (ip_len < 7){
            throw std::logic_error("the argument ip is empty.");
        }
        if (ip_len > 15){
            throw std::logic_error("the argument ip is too long.");
        }
        uint32_t last_8byte = 0xFF; //截取后面
        uint32_t segment_value = 0;
        int gap = 0;

        for (int i = 0; i < ip_len; ++i) {
            if (ip[i] == '.'){
                if (gap > 3){
                    throw std::logic_error("error the argument is too long.");
                }
                if (segment_value > 0xFF) {
                    throw std::logic_error("error the argument is too long.");
                }
                gap++;
                result = result | segment_value;
                result = result << 8;
                segment_value = 0;
            }else if (ip[i] >= '0' && ip[i] <= '9') {
                segment_value = segment_value * 10 + (ip[i] - '0');
            } else {
                throw std::logic_error(std::string("illegal characters."));
            }
        }
        if (gap != 3) {
            throw std::logic_error("content error.");
        }
        result = result | segment_value;
        return result;
    }

    std::string ipv4_address::ipv4_to_string(uint32_t _ip_value) {
        std::string result;
        for(int i = 3; i >= 0; --i) {
            uint32_t segment = (_ip_value >> (i * 8)) & 0xFF;
            if(!result.empty()) {
                result += ".";
            }
            result += std::to_string(segment);
        }
        return result;
    }

    bool ipv4_address::is_private_net_address() const {
        // IPv4专用内网地址范围：
        // 10.0.0.0 - 10.255.255.255
        auto A = ((ip_value_ & 0xFF000000) == 0x0A000000); // 10.x.x.x
        // 172.16.0.0 - 172.31.255.255
        auto B = ((ip_value_ & 0xFFF00000) == 0xAC100000); // 172.16.x.x
        // 192.168.0.0 - 192.168.255.255
        auto C = ((ip_value_ & 0xFFFF0000) == 0xC0A80000); // 192.168.x.x
        return A || B || C;
    }

    bool ipv4_address::test_ip_address_legal(const std::string &ip) noexcept {
        try{
            auto _ip =string_to_ipv4(ip);
        } catch (...) {
            return false;
        }
        return true;
    }

}