#include "signature_base.h"

bool SignatureBase::cbor_encode_request_id(CborEncoder* container, const uint8_t map_index) const {
    if (m_requestID.has_value()) {
        CborError err = cbor_encode_uint(container, map_index);
        checkCborError(err, "Failed to encode uint");

        err = cbor_encode_tag(container, UUID.tag());
        checkCborError(err, "Failed to encode tag");

        if (m_requestID.value().bytevector().size() != UUID_SIZE) {
            throw CborException("UUID size incorrect", CborErrorImproperValue);
        }
        err = cbor_encode_byte_string(container,  m_requestID.value().bytevector().data(), m_requestID.value().bytevector().size());
        checkCborError(err, "Failed to encode byte string");

        return true;
    }
    return false;
}

void SignatureBase::cbor_encode_signature(CborEncoder* container, const uint8_t map_index) const {
    CborError err = cbor_encode_uint(container, map_index);
    checkCborError(err, "Failed to encode uint");

    err = cbor_encode_byte_string(container, getSignature().data(), m_signature.size());
    checkCborError(err, "Failed to encode byte string");
}

bool SignatureBase::cbor_encode_origin(CborEncoder* container, const uint8_t map_index) const {
    if (m_origin.has_value()) {
        CborError err = cbor_encode_uint(container, map_index);
        checkCborError(err, "Failed to encode uint");

        err = cbor_encode_text_string(container, m_origin.value().c_str(), m_origin.value().size());
        checkCborError(err, "Failed to encode text string");
        return true;
    }
    return false;
}

bool SignatureBase::cbor_encode_public_key(CborEncoder *container, const uint8_t map_index) const
{
    if (m_publickey.has_value()) {
        CborError err = cbor_encode_uint(container, map_index);
        checkCborError(err, "Failed to encode uint");
        
        err = cbor_encode_byte_string(container, m_publickey.value().data(), m_publickey.value().size());
        checkCborError(err, "Failed to encode byte string");
        return true;
    }
    return false;
}
