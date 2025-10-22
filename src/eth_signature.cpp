#include "eth_signature.h"

EthSignature::EthSignature() : SignatureBase::SignatureBase()
{
    setRegistryType(ETH_SIGNATURE);
    setRequestID(Uuid());
    clearSignature();
}

EthSignature::EthSignature(const Uuid &requestId,
                           const std::vector<uint8_t> &signature,
                           const std::optional<std::string> &origin)
    : SignatureBase::SignatureBase(requestId, signature, origin)
{
    setRegistryType(ETH_SIGNATURE);
}

size_t EthSignature::getMapSize() const
{
    if (getOrigin().has_value())
        return MIN_MAP_LENGTH + 1;
    return MIN_MAP_LENGTH;
}

void EthSignature::toMap(CborEncoder *parentEncoder) const
{
    CborEncoder container;
    size_t mapSize = getMapSize();

    auto err = cbor_encoder_create_map(parentEncoder, &container, mapSize);
    checkCborError(err, "Failed to create map");

    auto isRequestIdEncoded = this->cbor_encode_request_id(&container, static_cast<uint8_t>(Key::REQUEST_ID));
    if (!isRequestIdEncoded)
    {
        throw CborException("Request ID is a mandatory field", CborErrorImproperValue);
    }

    if (getSignatureSize() < MIN_ETH_SIGNATURE_SIZE)
    {
        throw CborException("Signature size incorrect", CborErrorImproperValue);
    }
    this->cbor_encode_signature(&container, static_cast<uint8_t>(Key::SIGNATURE));

    this->cbor_encode_origin(&container, static_cast<uint8_t>(Key::ORIGIN));

    err = cbor_encoder_close_container(parentEncoder, &container);
    checkCborError(err, "Failed to close container");
}