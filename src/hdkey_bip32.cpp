#include <wally_core.h>
#include "cbor_exception.h"

#include "hdkey_bip32.h"

/**
 * @brief Base58 encoding results in a Base58-encoded string of up to 112 characters.
 *
 */
constexpr size_t MAX_KEY_STR_LEN = 112;

/**
 * @brief Decode 78 key bytes + 4 checksum bytes
 *
 */
constexpr size_t MAX_KEY_BYTES_LEN = 82;

/**
 * @brief A BIP32 key string representation consists in 78 bytes:
 * - 4 version bytes
 * - 1 depth byte
 * - 4 fingerprint bytes
 * - 4 child number bytes
 * - 32 chain code bytes
 * - 33 key data bytes
 *
 */
constexpr size_t EXPECTED_KEY_LEN = 78;
constexpr size_t VERSION_SIZE = 4;
constexpr size_t DEPTH_SIZE = 1;
constexpr size_t FINGERPRINT_SIZE = 4;
constexpr size_t CHILD_NUMBER_SIZE = 4;

/**
 * @brief Possible values for the version bytes
 *
 */
constexpr uint32_t MAINNET_XPUB = 0x0488B21E;
constexpr uint32_t MAINNET_XPRIV = 0x0488ADE4;
constexpr uint32_t TESTNET_XPUB = 0x043587CF;
constexpr uint32_t TESTNET_XPRIV = 0x04358394;
constexpr uint32_t MAINNET_ZPUB = 0x04B24746;
constexpr uint32_t MAINNET_ZPRIV = 0x04B2430C;
constexpr uint32_t MAINNET_LTUB = 0x019DA462;
constexpr uint32_t MAINNET_LTPV = 0x019D9CFE;

BIP32Deserializer::BIP32Key BIP32Deserializer::deserialize(const std::string &key)
{
    BIP32Key outKey;
    uint8_t decodedData[MAX_KEY_BYTES_LEN] = {0};
    size_t decodedLen = 0;

    // Preliminary check: rough length validation
    if (key.length() > MAX_KEY_STR_LEN)
    {
        throw CborException("Key length incorrect", CborErrorImproperValue);
    }

    int result = wally_base58_to_bytes(key.c_str(), BASE58_FLAG_CHECKSUM, decodedData, MAX_KEY_BYTES_LEN, &decodedLen);
    if (result != WALLY_OK)
    {
        throw CborException("Failed to decode key string", CborErrorImproperValue);
    }

    // Validate size of decoded data
    if (decodedLen != EXPECTED_KEY_LEN)
    {
        throw CborException("Decoded key size incorrect", CborErrorImproperValue);
    }

    // Parse fields
    size_t offset = 0;

    // 4 bytes version
    uint32_t version = (decodedData[offset] << 24) | (decodedData[offset + 1] << 16) |
                       (decodedData[offset + 2] << 8) | decodedData[offset + 3];
    offset += VERSION_SIZE;

    bool isPrivate = (version == MAINNET_XPRIV || version == TESTNET_XPRIV || version == MAINNET_ZPRIV || version == MAINNET_LTPV);
    bool isPublic = (version == MAINNET_XPUB || version == TESTNET_XPUB || version == MAINNET_ZPUB || version == MAINNET_LTUB);
    if (!isPrivate && !isPublic)
    {
        throw CborException("Unsupported key version", CborErrorImproperValue);
    }
    outKey.is_private = isPrivate;
    outKey.is_testnet = (version == TESTNET_XPRIV || version == TESTNET_XPUB);

    // 1 byte depth
    uint8_t depth = decodedData[offset];
    offset += DEPTH_SIZE;
    outKey.is_master = (depth == 0);

    // 4 bytes parent fingerprint
    outKey.parent_fingerprint = (decodedData[offset] << 24) | (decodedData[offset + 1] << 16) |
                                (decodedData[offset + 2] << 8) | decodedData[offset + 3];
    offset += FINGERPRINT_SIZE;
    if (outKey.is_master && (outKey.parent_fingerprint != 0))
    {
        throw CborException("Invalid master key with non-zero parent fingerprint", CborErrorImproperValue);
    }

    // 4 bytes child number
    outKey.child_number = (decodedData[offset] << 24) | (decodedData[offset + 1] << 16) |
                          (decodedData[offset + 2] << 8) | decodedData[offset + 3];
    offset += CHILD_NUMBER_SIZE;

    // 32 bytes chain code
    std::copy(decodedData + offset, decodedData + offset + CHAIN_CODE_SIZE, outKey.chain_code.begin());
    offset += CHAIN_CODE_SIZE;

    // 33 bytes: key data
    std::copy(decodedData + offset, decodedData + offset + KEY_DATA_SIZE, outKey.key_data.begin());
    // If private key, validate that it starts with 0x00
    if (isPrivate && outKey.key_data[0] != 0x00)
    {
        throw CborException("Invalid private key data", CborErrorImproperValue);
    }

    return outKey;
}

KeyData BIP32Deserializer::hexStringToKeyData(const std::string &pubkey)
{
    std::string compressed_pubkey = pubkey;
    if (pubkey.length() != 2 * KEY_DATA_SIZE)
    {
        if (pubkey.length() == 2 * (KEY_DATA_SIZE - 1))
        {
            // This is an uncompressed public key, we need to prepend 0x00 to the 32 bytes to fit KeyData type
            compressed_pubkey.insert(0, "00");
        }
        else
        {
            throw CborException("Pubkey length must be 32 or 33 bytes", CborErrorImproperValue);
        }
    }
    KeyData arr;
    for (size_t i = 0; i < arr.size(); i++)
    {
        auto substr = compressed_pubkey.substr(i * 2, 2);
        try
        {
            arr[i] = std::stoi(substr, nullptr, 16);
        }
        catch (const std::exception &e)
        {
            throw CborException("Pubkey should be hexadecimal", CborErrorImproperValue);
        }
    }
    return arr;
}
