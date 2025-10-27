#pragma once

#include <stdexcept>
#include <string>
#include <limits>
#include <tinycbor/cbor.h>

class CborException : public std::runtime_error
{
public:
    explicit CborException(const std::string &message, CborError errorCode)
        : std::runtime_error(message + " (Error Code: " + std::to_string(errorCode) + ")"),
          m_errorCode(errorCode) {}

    CborError errorCode() const noexcept { return m_errorCode; }

private:
    CborError m_errorCode;
};

namespace CborErrorUtils
{
    inline bool g_cborOutOfMemoryFlag = false;

    inline void clearOutOfMemoryFlag()
    {
        g_cborOutOfMemoryFlag = false;
    }

    inline void markOutOfMemory()
    {
        g_cborOutOfMemoryFlag = true;
    }

    inline bool outOfMemoryOccurred()
    {
        return g_cborOutOfMemoryFlag;
    }
}

inline void checkCborError(CborError err, const std::string &errorMessage)
{
    if (err == CborErrorOutOfMemory)
    {
        CborErrorUtils::markOutOfMemory();
        return; // allow continuing encoding to get the needed bytes for dynamic buffer allocation
    }
    if (err != CborNoError)
    {
        throw CborException(errorMessage, err);
    }
}

inline void checkIsMap(const CborValue *value)
{
    if (!cbor_value_is_map(value))
    {
        throw CborException("Expected CBOR map", CborErrorIllegalType);
    }
}

inline void checkIsArray(const CborValue *value)
{
    if (!cbor_value_is_array(value))
    {
        throw CborException("Expected CBOR array", CborErrorIllegalType);
    }
}

inline void checkIsUint(const CborValue *value)
{
    if (!cbor_value_is_unsigned_integer(value))
    {
        throw CborException("Expected CBOR unsigned integer", CborErrorIllegalType);
    }
}

inline void checkIsInt(const CborValue *value)
{
    if (!cbor_value_is_integer(value))
    {
        throw CborException("Expected CBOR integer", CborErrorIllegalType);
    }
}

inline void checkIsBool(const CborValue *value)
{
    if (!cbor_value_is_boolean(value))
    {
        throw CborException("Expected CBOR boolean", CborErrorIllegalType);
    }
}

inline void checkIsTag(const CborValue *value)
{
    if (!cbor_value_is_tag(value))
    {
        throw CborException("Expected CBOR tag", CborErrorIllegalType);
    }
}

inline void checkIsByteStr(const CborValue *value)
{
    if (!cbor_value_is_byte_string(value))
    {
        throw CborException("Expected CBOR byte string", CborErrorIllegalType);
    }
}

inline void checkIsTextStr(const CborValue *value)
{
    if (!cbor_value_is_text_string(value))
    {
        throw CborException("Expected CBOR text string", CborErrorIllegalType);
    }
}

inline void checkMaxUint8(uint64_t value)
{
    if (value > std::numeric_limits<uint8_t>::max())
    {
        throw CborException("Value exceeds maximum uint8_t", CborErrorImproperValue);
    }
}

inline void checkMaxUint32(uint64_t value)
{
    if (value > std::numeric_limits<uint32_t>::max())
    {
        throw CborException("Value exceeds maximum uint32_t", CborErrorImproperValue);
    }
}

inline void checkMaxUint31(uint64_t value)
{
    if (value >= 0x80000000)
    {
        throw CborException("Value exceeds maximum uint31", CborErrorImproperValue);
    }
}