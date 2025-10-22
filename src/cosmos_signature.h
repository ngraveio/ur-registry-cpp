#pragma once

#include <iostream>

#include "uuid.h"
#include "signature_base.h"

/**
 * @brief  Implementation of the cosmos-signature (CBOR tag #6.4102) UR type
 * 
 * Source: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2023-003-sign.md
 * UR Type Tag: #6.4102
 * 
 * cosmos-signature  = (
 *     request-id: uuid,
 *     signature: bytes,
 *     public-key: bytes
 * )
 * 
 * request-id = 1
 * signature = 2
 * public-key = 3
 * 
 **/

class CosmosSignature : public SignatureBase
{
public:
    CosmosSignature();
    CosmosSignature(const std::optional<Uuid>& requestId, const std::vector<uint8_t>& signature, const std::optional<std::vector<uint8_t>>& publicKey = std::nullopt);

    enum class Key {
      REQUEST_ID = 1,
      SIGNATURE,
      PUBLIC_KEY
    };

    size_t getMinMapLength() const override {
        return MIN_MAP_LENGTH;
    }

    size_t getMaxMapLength() const override {
        return MAX_MAP_LENGTH;
    }

    size_t getMapSize() const override;
    void toMap(CborEncoder* parentEncoder) const override;
    void fromMap(CborValue*) override {
        throw CborException("Decoder not supported", CborErrorUnimplementedValidation);
    }

private:
    static constexpr size_t MIN_MAP_LENGTH = 1;
    static constexpr size_t MAX_MAP_LENGTH = 3;
};
