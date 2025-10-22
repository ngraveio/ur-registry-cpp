#include "sign_response.h"

SignResponse::SignResponse()
{
    setRegistryType(SIGN_RESPONSE);
    clearSignature();
}

SignResponse::SignResponse(const std::optional<Uuid> &requestId,
                           const std::vector<uint8_t> &signature,
                           const std::optional<std::string> &origin,
                           const std::optional<std::vector<uint8_t>> &publicKey)
    : SignatureBase::SignatureBase(requestId, signature, origin, publicKey)
{
    setRegistryType(SIGN_RESPONSE);
}

size_t SignResponse::getMapSize() const
{
    size_t mapSize = MIN_MAP_LENGTH;
    if (getRequestId().has_value())
        ++mapSize;
    if (getOrigin().has_value())
        ++mapSize;
    if (getPublickey().has_value())
        ++mapSize;
    return mapSize;
}

void SignResponse::toMap(CborEncoder *parentEncoder) const
{
    CborEncoder container;
    size_t mapSize = getMapSize();

    auto err = cbor_encoder_create_map(parentEncoder, &container, mapSize);
    checkCborError(err, "Failed to create map");

    this->cbor_encode_request_id(&container, static_cast<uint8_t>(Key::REQUEST_ID));
    this->cbor_encode_signature(&container, static_cast<uint8_t>(Key::SIGNATURE));
    this->cbor_encode_origin(&container, static_cast<uint8_t>(Key::ORIGIN));
    this->cbor_encode_public_key(&container, static_cast<uint8_t>(Key::PUBLIC_KEY));

    err = cbor_encoder_close_container(parentEncoder, &container);
    checkCborError(err, "Failed to close container");
}
