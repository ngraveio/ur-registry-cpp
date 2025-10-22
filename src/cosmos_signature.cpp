#include "cosmos_signature.h"

CosmosSignature::CosmosSignature()
{
    setRegistryType(COSMOS_SIGNATURE);
    clearSignature();
}

CosmosSignature::CosmosSignature(const std::optional<Uuid> &requestId,
                                 const std::vector<uint8_t> &signature,
                                 const std::optional<std::vector<uint8_t>> &publicKey)
    : SignatureBase::SignatureBase(requestId, signature, std::nullopt, publicKey)
{
    setRegistryType(COSMOS_SIGNATURE);
}

size_t CosmosSignature::getMapSize() const
{
    size_t mapSize = MIN_MAP_LENGTH;
    if (getRequestId().has_value())
        ++mapSize;
    if (getPublickey().has_value())
        ++mapSize;
    return mapSize;
}

void CosmosSignature::toMap(CborEncoder *parentEncoder) const
{
    CborEncoder container;
    size_t mapSize = getMapSize();

    auto err = cbor_encoder_create_map(parentEncoder, &container, mapSize);
    checkCborError(err, "Failed to create map");

    this->cbor_encode_request_id(&container, static_cast<uint8_t>(Key::REQUEST_ID));
    this->cbor_encode_signature(&container, static_cast<uint8_t>(Key::SIGNATURE));

    // the pubkey is optiona for the base/sign-response types but not for cosmos-signature
    const bool pubkeAdded = this->cbor_encode_public_key(&container, static_cast<uint8_t>(Key::PUBLIC_KEY));
    if (!pubkeAdded)
    {
        err = cbor_encoder_close_container(parentEncoder, &container);
        checkCborError(err, "Failed to close container");

        throw CborException("Cosmos signature encoder missing the pubkey", CborErrorTooFewItems);
    }

    err = cbor_encoder_close_container(parentEncoder, &container);
    checkCborError(err, "Failed to close container");
}
