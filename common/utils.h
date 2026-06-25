#pragma once

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../src/hdkey_bip32.h"

inline KeyData convertToKeyData(const std::vector<uint8_t>& vec) {
    if (vec.size() != KEY_DATA_SIZE) {
        throw std::invalid_argument("ByteArray size does not match KEY_DATA_SIZE.");
    }

    KeyData result;
    std::copy(vec.begin(), vec.end(), result.begin());
    return result;
}

inline ChainCode convertToChainCode(const std::vector<uint8_t>& vec) {
    if (vec.size() != CHAIN_CODE_SIZE) {
        throw std::invalid_argument("Vector size does not match CHAIN_CODE_SIZE.");
    }

    ChainCode result;
    std::copy(vec.begin(), vec.end(), result.begin());
    return result;
}

inline std::string bytesToHex(const std::vector<uint8_t>& bytes)
{
    static const char hex_chars[] = "0123456789abcdef";
    std::string hex;
    hex.reserve(bytes.size() * 2);
    for (uint8_t b : bytes) {
        hex.push_back(hex_chars[(b >> 4) & 0x0F]);
        hex.push_back(hex_chars[b & 0x0F]);
    }
    return hex;
}

inline std::vector<uint8_t> fromHex(const std::string &hex) {
    std::vector<uint8_t> bytes;
    bytes.reserve(hex.size() / 2);

    for (std::size_t i = 0; i < hex.size(); i += 2) {
        auto byte = static_cast<uint8_t>(
            std::stoul(hex.substr(i, 2), nullptr, 16)
        );
        bytes.push_back(byte);
    }

    return bytes;
}

inline std::string vectorToHex(const std::vector<uint8_t>& data) {
    std::ostringstream oss;
    for (uint8_t byte : data) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return oss.str();
}

inline bool isHexString(std::string_view hexStr) {
    if (hexStr.empty())
        return false;

    return std::all_of(hexStr.begin(), hexStr.end(), 
                       [](unsigned char c) {
                           return std::isxdigit(c);
                       });
}

template <std::size_t N>
std::string arrayToHex(const std::array<uint8_t, N>& data) {
    std::ostringstream oss;
    for (uint8_t byte : data) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return oss.str();
}