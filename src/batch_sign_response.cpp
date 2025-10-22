#include "batch_sign_response.h"

void BatchSignResponse::toMap(CborEncoder* parentEncoder) const
{
    if(m_signrespones.size() < 1)
        throw CborException("Sign responses array cannot be empty.", CborErrorImproperValue);
    
    CborError err = CborNoError;
    CborEncoder container;

    size_t mapSize = getMapSize();
    
    err = cbor_encoder_create_map(parentEncoder, &container, mapSize);
    checkCborError(err, "Failed to create map");
    
    err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::SIGN_RESPONSES));
    checkCborError(err, "Failed to encode uint");
    
    CborEncoder arrayEncoder;
    const uint32_t arraySize = m_signrespones.size();
    
    err = cbor_encoder_create_array(&container, &arrayEncoder, arraySize);
    checkCborError(err, "Failed to create array");

    for (const auto& sign_respone: m_signrespones) {
        if(!sign_respone.getRequestId().has_value() || sign_respone.getRequestId().value().str().empty())
            throw CborException("Item sign-response missing requestId", CborErrorImproperValue);

        err = cbor_encode_tag(&arrayEncoder, SIGN_RESPONSE.tag());
        checkCborError(err, "Failed to encode sign-response tag");
        
        sign_respone.toMap(&arrayEncoder);        
    }    

    err = cbor_encoder_close_container(&container, &arrayEncoder);
    checkCborError(err, "Failed to close arrayEncoder");
    
    err = cbor_encoder_close_container(parentEncoder, &container);
    checkCborError(err, "Failed to close container");
}
