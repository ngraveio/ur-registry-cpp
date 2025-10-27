#include "request_base.h"
#include "cbor_exception.h"

void RequestBase::cbor_decode_requestid(CborValue *container)
{
    validateTag(container, UUID);
    CborError err = cbor_value_advance(container);
    checkCborError(err, "Failed to advance");

    std::vector<uint8_t> request_id_data(UUID_SIZE);
    size_t decoded_uuid_size = 0;
    checkIsByteStr(container);
    err = cbor_value_get_string_length(container, &decoded_uuid_size);
    checkCborError(err, "Failed to get string length");

    if (decoded_uuid_size != UUID_SIZE)
        throw CborException("UUID size is incorrect", CborErrorImproperValue);

    err = cbor_value_copy_byte_string(container, request_id_data.data(), &decoded_uuid_size, nullptr);
    checkCborError(err, "Failed to copy byte string");

    m_requestid.emplace(request_id_data);
}

void RequestBase::cbor_decode_derivation_path(CborValue *container)
{
    CborError err = cbor_value_advance(container);
    checkCborError(err, "Failed to advance");

    checkIsMap(container);
    Keypath derivPath;
    derivPath.fromMap(container);
    m_derivation_path.emplace(std::move(derivPath));
}

void RequestBase::cbor_decode_sign_data(CborValue *container)
{
    std::vector<uint8_t> signData;
    size_t signData_size = 0;
    checkIsByteStr(container);

    CborError err = cbor_value_calculate_string_length(container, &signData_size);
    checkCborError(err, "Failed to get string length");

    signData.resize(signData_size);
    err = cbor_value_copy_byte_string(container, signData.data(), &signData_size, nullptr);
    checkCborError(err, "Failed to copy byte string");

    m_sign_data = std::move(signData);
}

void RequestBase::cbor_decode_origin(CborValue *container)
{
    size_t origin_size = 0;
    checkIsTextStr(container);

    CborError err = cbor_value_get_string_length(container, &origin_size);
    checkCborError(err, "Failed to get string length");

    std::string origin(origin_size, '\0');
    err = cbor_value_copy_text_string(container, origin.data(), &origin_size, nullptr);
    checkCborError(err, "Failed to copy text string");

    m_origin.emplace(std::move(origin));
}

void RequestBase::clearBaseItems()
{
    m_requestid = std::nullopt;
    m_origin = std::nullopt;
    m_derivation_path = std::nullopt;
    m_sign_data.clear();
}

void RequestBase::validateTag(const CborValue *encoder, const RegistryType &tagUrType)
{
    checkIsTag(encoder);
    CborTag tag{};
    CborError err = cbor_value_get_tag(encoder, &tag);
    checkCborError(err, "Failed to get " + tagUrType.type() + " tag");

    if (tag != tagUrType.tag())
        throw CborException("Decoded " + tagUrType.type() + " tag is incorrect", CborErrorInappropriateTagForType);
}
