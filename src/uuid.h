#pragma once

#include <string>

#include <bc-ur/utils.hpp>

constexpr size_t UUID_SIZE = 16;

/**
 * @brief  Implementation of the uuid (CBOR tag #6.37) UR type
 *
 * Source: CBOR Tags Registry
 * - https://www.iana.org/assignments/cbor-tags/cbor-tags.xhtml
 * UUID specification:
 * - https://www.rfc-editor.org/rfc/rfc9562.html
 *
 * UUID Format = 4hexOctet "-" 2hexOctet "-" 2hexOctet "-" 2hexOctet "-" 6hexOctet
 *
 * Ref implementation: https://github.com/BlockchainCommons/keytool-cli/blob/master/src/uuid.hpp
 */

class Uuid final
{
public:
    Uuid();
    explicit Uuid(const std::string &uuid_str);
    explicit Uuid(const ur::ByteVector &data);

    const ur::ByteVector &bytevector() const { return _data; }
    const std::string str() const;

private:
    ur::ByteVector _data;
};
