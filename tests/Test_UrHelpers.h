#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <cppunit/extensions/HelperMacros.h>

#include "../src/registry_item.h"
#include "../common/utils.h"

/**
 * @brief Helper to compare and validate the CBOR data
 */
template <typename T>
void ValidateCborResults(T &ur_type, const std::vector<uint8_t> &expectedBytes)
{
    ur::ByteVector cborDataVector;
    CPPUNIT_ASSERT_NO_THROW(cborDataVector = ur_type.toCbor());

    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "CBOR output does not match the expected value",
        bytesToHex(expectedBytes),
        bytesToHex(cborDataVector));
}

/**
 * @brief Helper to compare and validate the UR type encoding
 */
template <typename T>
void ValidateUrEncoding(T &ur_type, const std::string &expectedUR)
{
    UrType urStr;

    CPPUNIT_ASSERT_NO_THROW(urStr = ur_type.toSinglePartUr());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "UR does not match the expected value",
        expectedUR,
        urStr);
}

/**
 * @brief Helper to validate Cbor exception when encoding the UR type
 */
template <typename T>
void ValidateUrEncodingException(T &ur_type, const CborError error_code)
{
    CPPUNIT_ASSERT_THROW(ur_type.toSinglePartUr(), CborException);
    try
    {
        ur_type.toSinglePartUr();
    }
    catch (const CborException &e)
    {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Error code different than expected",
            error_code,
            e.errorCode());
    }
}

template <typename T>
void ValidateUrEncodingException(T &ur_type, const CborError error_code, const std::string &message)
{
    CPPUNIT_ASSERT_THROW_MESSAGE(
        message,
        ur_type.toSinglePartUr(),
        CborException);
    try
    {
        ur_type.toSinglePartUr();
    }
    catch (const CborException &e)
    {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Error code different than expected for " + message,
            error_code,
            e.errorCode());
    }
}

/**
 * @brief Helper to validate Cbor exception when decoding the UR type
 */
template <typename T>
void ValidateUrDecodingException(T &ur_type, const std::string &urError, const CborError error_code)
{
    CPPUNIT_ASSERT_THROW(ur_type.fromUr(urError), CborException);
    try
    {
        ur_type.fromUr(urError);
    }
    catch (const CborException &e)
    {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Error code different than expected",
            error_code,
            e.errorCode());
    }
}

template <typename T>
void ValidateUrDecodingException(T &ur_type, const std::string &urError, const CborError error_code, const std::string &message)
{
    CPPUNIT_ASSERT_THROW_MESSAGE(
        message,
        ur_type.fromUr(urError),
        CborException);
    try
    {
        ur_type.fromUr(urError);
    }
    catch (const CborException &e)
    {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Error code different than expected for " + message,
            error_code,
            e.errorCode());
    }
}
