#pragma once

#include <iostream>

#include "uuid.h"
#include "signature_base.h"

/**
 * @brief  Implementation of the sign-response (CBOR tag #6.41412) UR type
 *
 * Source: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2023-003-sign.md
 * CDDL specification:
 *
 * sign-response = {
 *     ?request-id: uuid,   ; Echoes the request-id if present in the corresponding sign-request
 *     signature: bytes,    ; Resulting signature
 *     ?origin: text,       ; Device or signer name
 *     ?public-key: bytes   ; Public key used to produce the signature (required only in multi-account selection)
 * }
 *
 * request-id = 1
 * signature = 2
 * origin = 3
 * public-key = 4
 *
 **/

class SignResponse : public SignatureBase
{
public:
    SignResponse();
    SignResponse(const std::optional<Uuid> &requestId, const std::vector<uint8_t> &signature, const std::optional<std::string> &origin = std::nullopt, const std::optional<std::vector<uint8_t>> &publicKey = std::nullopt);

    enum class Key
    {
        REQUEST_ID = 1,
        SIGNATURE,
        ORIGIN,
        PUBLIC_KEY
    };

    size_t getMinMapLength() const override
    {
        return MIN_MAP_LENGTH;
    }

    size_t getMaxMapLength() const override
    {
        return MAX_MAP_LENGTH;
    }

    size_t getMapSize() const override;
    void toMap(CborEncoder *parentEncoder) const override;
    void fromMap(CborValue *) override
    {
        throw CborException("Decoder not supported", CborErrorUnimplementedValidation);
    }

private:
    static constexpr size_t MIN_MAP_LENGTH = 1;
    static constexpr size_t MAX_MAP_LENGTH = 4;
};
