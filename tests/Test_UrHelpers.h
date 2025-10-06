#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <cppunit/extensions/HelperMacros.h>

#include "../src/registry_item.h"
#include "../src/hdkey_bip32.h"

inline KeyData convertToKeyData(const std::vector<uint8_t>& vec) {
    if (vec.size() != KEY_DATA_SIZE) {
        throw std::invalid_argument("ByteArray size does not match KEY_DATA_SIZE.");
    }

    KeyData result;
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

/**
 * @brief Helper to compare and validate the CBOR data
 */
template <typename T>
void ValidateCborResults(T& ur_type, const std::vector<uint8_t>& expectedBytes)
{
    ur::ByteVector cborDataVector;
    CPPUNIT_ASSERT_NO_THROW(cborDataVector = ur_type.toCbor());

    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "CBOR output does not match the expected value",
        bytesToHex(expectedBytes),
        bytesToHex(cborDataVector)
    );
}

/**
 * @brief Helper to compare and validate the UR type encoding
 */
template <typename T>
void ValidateUrEncoding(T& ur_type, const std::string& expectedUR)
{
    UrType urStr;

    CPPUNIT_ASSERT_NO_THROW(urStr = ur_type.toSinglePartUr());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "UR does not match the expected value",
        expectedUR,
        urStr
    );
}

/**
 * @brief Helper to validate Cbor exception when encoding the UR type
 */
template <typename T>
void ValidateUrEncodingException(T& ur_type, const CborError error_code)
{
    CPPUNIT_ASSERT_THROW(ur_type.toSinglePartUr(), CborException);
    try {
        ur_type.toSinglePartUr();
    } catch (const CborException& e) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Error code different than expected",
            error_code,
            e.errorCode()
        );
    }
}

template <typename T>
void ValidateUrEncodingException(T& ur_type, const CborError error_code, const std::string& message)
{
    CPPUNIT_ASSERT_THROW_MESSAGE(
        message,
        ur_type.toSinglePartUr(),
        CborException);
    try {
        ur_type.toSinglePartUr();
    } catch (const CborException& e) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Error code different than expected for " + message,
            error_code,
            e.errorCode()
        );
    }
}

/**
 * @brief Helper to validate Cbor exception when decoding the UR type
 */
template <typename T>
void ValidateUrDecodingException(T& ur_type, const std::string& urError, const CborError error_code)
{
    CPPUNIT_ASSERT_THROW(ur_type.fromUr(urError), CborException);
    try {
        ur_type.fromUr(urError);
    } catch (const CborException& e) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Error code different than expected",
            error_code,
            e.errorCode()
        );
    }
}

template <typename T>
void ValidateUrDecodingException(T& ur_type, const std::string& urError, const CborError error_code, const std::string& message)
{
    CPPUNIT_ASSERT_THROW_MESSAGE(
        message,
        ur_type.fromUr(urError),
        CborException);
    try {
        ur_type.fromUr(urError);
    } catch (const CborException& e) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Error code different than expected for " + message,
            error_code,
            e.errorCode()
        );
    }
}
