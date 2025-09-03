#pragma once

#include <QByteArray>
#include <QString>
#include <cppunit/extensions/HelperMacros.h>

#include "../src/registry_item.h"
#include "../src/hdkey_bip32.h"

inline std::vector<uint8_t> convertQByteArrayToVector(const QByteArray& byteArray)
{
    return std::vector<uint8_t>(byteArray.begin(), byteArray.end());
}

inline KeyData convertToStdArrayKeyData(const QByteArray& byteArray) {
    if (byteArray.size() != KEY_DATA_SIZE) {
        throw std::invalid_argument("ByteArray size does not match KEY_DATA_SIZE.");
    }

    KeyData result;
    std::copy(byteArray.begin(), byteArray.end(), result.begin());
    return result;
}

/**
 * @brief Helper to compare and validate the CBOR data
 *
 * @tparam T should have as parent the abstract class RegistryItem
 * @param ur_type contains the UR class to test
 * @param expectedBytes contains the CBOR vector
 */
template <typename T>
void ValidateCborResults(T& ur_type, const QByteArray& expectedBytes)
{
    ur::ByteVector cborDataVector;
    CPPUNIT_ASSERT_NO_THROW(cborDataVector = ur_type.toCbor());

    QByteArray cborData = QByteArray::fromRawData(reinterpret_cast<const char*>(cborDataVector.data()),
                                                  static_cast<int>(cborDataVector.size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "CBOR output does not match the expected value",
        expectedBytes.toHex().toStdString(),
        cborData.toHex().toStdString()
    );
}

/**
 * @brief Helper to compare and validate the UR type encoding
 *
 * @tparam T should have as parent the abstract class RegistryItem
 * @param ur_type contains the UR class to test
 * @param expectedUR contains the expected UR string
 */
template <typename T>
void ValidateUrEncoding(T& ur_type, const QString& expectedUR)
{
    UrType urStr;

    CPPUNIT_ASSERT_NO_THROW(urStr = ur_type.toSinglePartUr());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "UR does not match the expected value",
        expectedUR.toStdString(),
        urStr
    );
}

/**
 * @brief Helper to validate Cbor exception when encoding the UR type
 *
 * @tparam T should have as parent the abstract class RegistryItem
 * @param ur_type contains the UR to encode with an error
 * @param error_code contains the returned error code during the CborException
 * @param message adds an optional message if the assertion fails
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
void ValidateUrEncodingException(T& ur_type, const CborError error_code, const QString& message)
{
    CPPUNIT_ASSERT_THROW_MESSAGE(
        message.toStdString(),
        ur_type.toSinglePartUr(),
        CborException);
    try {
        ur_type.toSinglePartUr();
    } catch (const CborException& e) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Error code different than expected for " + message.toStdString(),
            error_code,
            e.errorCode()
        );
    }
}

/**
 * @brief Helper to validate Cbor exception when decoding the UR type
 *
 * @tparam T should have as parent the abstract class RegistryItem
 * @param ur_type contains the results of the decoded UR type
 * @param urError contains the UR QString with an error
 * @param error_code contains the returned error code during the CborException
 * @param message adds an optional message if the assertion fails
 */
template <typename T>
void ValidateUrDecodingException(T& ur_type, const QString& urError, const CborError error_code)
{
    CPPUNIT_ASSERT_THROW(ur_type.fromUr(urError.toStdString()), CborException);
    try {
        ur_type.fromUr(urError.toStdString());
    } catch (const CborException& e) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Error code different than expected",
            error_code,
            e.errorCode()
        );
    }
}

template <typename T>
void ValidateUrDecodingException(T& ur_type, const QString& urError, const CborError error_code, const QString& message)
{
    CPPUNIT_ASSERT_THROW_MESSAGE(
        message.toStdString(),
        ur_type.fromUr(urError.toStdString()),
        CborException);
    try {
        ur_type.fromUr(urError.toStdString());
    } catch (const CborException& e) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Error code different than expected for " + message.toStdString(),
            error_code,
            e.errorCode()
        );
    }
}
