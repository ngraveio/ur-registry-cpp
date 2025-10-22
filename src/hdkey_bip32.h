#pragma once

#include <string>
#include <array>
#include <tinycbor/cbor.h>

constexpr size_t KEY_DATA_SIZE = 33;
constexpr size_t CHAIN_CODE_SIZE = 32;

using KeyData = std::array<uint8_t, KEY_DATA_SIZE>;
using ChainCode = std::array<uint8_t, CHAIN_CODE_SIZE>;

/**
 * @brief Implementation of utility classes related to BIP32
 *
 * Source: https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki
 */

/**
 * @brief Deserializer of BIP32 key string format
 *
 */
class BIP32Deserializer
{
public:
    struct BIP32Key
    {
        bool is_master{false};
        bool is_private{false};
        bool is_testnet{false};
        uint32_t version{0};
        uint8_t depth{0};
        uint32_t parent_fingerprint{0};
        uint32_t child_number{0};
        ChainCode chain_code;
        KeyData key_data;
    };

    // Parses a Base58 BIP32 key string into the structured BIP32Key
    static BIP32Deserializer::BIP32Key deserialize(const std::string &key);

    static KeyData hexStringToKeyData(const std::string &pubkey);
};
