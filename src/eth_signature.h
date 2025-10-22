#pragma once

#include <iostream>

#include "uuid.h"
#include "signature_base.h"

/**
 * @brief  Implementation of the eth-signature (CBOR tag #6.402) UR type
 * 
 * Source: https://eips.ethereum.org/EIPS/eip-4527
 * CDDL specification:
 * 
 * eth-signature  = (
 *     request-id: uuid,
 *     signature: eth-signature-bytes,
 *     ? origin: text, ; The device info for providing this signature
 * )
 *
 * request-id = 1
 * signature = 2
 * origin = 3
 *
 * eth-signature-bytes = bytes .size 65; the signature of the signing request (r,s,v)
 * 
 * Important note: 
 * The CDDL specification is incorrect regarding eth-signature-bytes.
 * In some cases, the signature can be above the 65 bytes. For example, with EIP-155, the v value can be replaced by more than 1 byte.
 * The proposed implementation used the 65 bytes as the minimum signature size instead.
 */

class EthSignature : public SignatureBase
{
public:
    EthSignature();
    EthSignature(const Uuid& requestId, const std::vector<uint8_t>& signature, const std::optional<std::string>& origin = std::nullopt);

    enum class Key {
        REQUEST_ID = 1,
        SIGNATURE,
        ORIGIN,
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
    static constexpr size_t MIN_MAP_LENGTH = 2;
    static constexpr size_t MAX_MAP_LENGTH = 3;
    static constexpr size_t MIN_ETH_SIGNATURE_SIZE = 65;
};
