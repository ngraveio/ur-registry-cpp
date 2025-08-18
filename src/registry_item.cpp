#include "registry_item.h"

ur::ByteVector RegistryItem::toCbor() const {
    uint8_t buffer[MAX_CBOR_SIZE] = {0};
    CborEncoder encoder;
    ur::ByteVector cbor;

    cbor_encoder_init(&encoder, buffer, sizeof(buffer), 0);

    size_t mapSize = getMapSize();
    if (mapSize != 0) {
        // Encode the CBOR map
        toMap(&encoder);

        if (!CborErrorUtils::outOfMemoryOccurred()) {
            // Convert encoded map to CBOR bytes
            size_t encodedSize = cbor_encoder_get_buffer_size(&encoder, buffer);
            cbor.assign(buffer, buffer + encodedSize);
        } else { 
            // Clear flag 
            CborErrorUtils::clearOutOfMemoryFlag();
            // Dynamically allocate more memory for CBOR encoding
            size_t total_needed = MAX_CBOR_SIZE + cbor_encoder_get_extra_bytes_needed(&encoder);
            uint8_t* resized_buffer = static_cast<uint8_t*>(malloc(total_needed));
            cbor_encoder_init(&encoder, resized_buffer, total_needed, 0);
            toMap(&encoder);
            if (CborErrorUtils::outOfMemoryOccurred()) {
                // OOM occurs again even with the resized buffer
                throw CborException("Cannot encode CBOR data even after adding extra needed bytes", CborErrorOutOfMemory);
            }
            
            cbor.assign(resized_buffer, resized_buffer + total_needed);
        }
    }
    return cbor;
}

ur::UR RegistryItem::encodeUr() const {
    UrType ur_type = m_registryType.type().c_str();

    // Using the legacy UR type if defined
    if (m_legacyRegistryType.has_value() && m_useLegacy.has_value()) {
        ur_type = (m_useLegacy.value()) ? m_legacyRegistryType.value().type().c_str() : m_registryType.type().c_str();
    }

    ur::ByteVector cbor = toCbor();
    return ur::UR(ur_type, cbor);
}

std::string RegistryItem::toSinglePartUr() const {
    ur::UR ur = encodeUr();

    // Encode single part UR without fountain encoder
    return ur::UREncoder::encode(ur);
}

ur::UREncoder RegistryItem::toUr(uint32_t max_fragment_length) const {
    constexpr uint32_t DEFAULT_FIRST_SEQ = 0;
    constexpr size_t DEFAULT_MIN_FRAQ_LEN = 10;

    ur::UR ur = encodeUr();

    if (ur.cbor().size() <= DEFAULT_MIN_FRAQ_LEN) {
        throw CborException("Message length is below the minimum fragment length ", CborErrorImproperValue);
    }

    return ur::UREncoder(ur, max_fragment_length, DEFAULT_FIRST_SEQ, DEFAULT_MIN_FRAQ_LEN);
}

static ur::UR decodeUr(const std::string& urStr) {
    // Parse the UR string into its components: type and CBOR data
    ur::URDecoder urdec = ur::URDecoder();

    // Ensure decoded UR is safe to use
    try {
        return urdec.decode(urStr);
    } catch (const std::exception& e) {
        throw CborException("Failed to decode UR", CborErrorIO);
    }
}

void RegistryItem::fromUr(const ur::UR& ur) {
    // Check if the decoded UR type matches the expected registry type or the legacy type
    if (ur.type() != m_registryType.type() &&
       (!m_legacyRegistryType.has_value() || ur.type() != m_legacyRegistryType.value().type())) {
        throw CborException("Unexpected UR type", CborErrorUnknownType);
    }

    // Decode the CBOR data
    const ur::ByteVector& cbor = ur.cbor();
    CborParser parser;
    CborValue value;

    CborError err = cbor_parser_init(cbor.data(), cbor.size(), 0, &parser, &value);
    checkCborError(err, "Failed to init parser");
    checkIsMap(&value);

    // Verify map length fitting UR type length
    size_t length = 0;
    err = cbor_value_get_map_length(&value, &length);
    checkCborError(err, "Failed to get map length");

    size_t minLength = getMinMapLength(); 
    size_t maxLength = getMaxMapLength(); 
    if (length < minLength || length > maxLength) {
        throw CborException("Map length is out of range for the UR type definition", CborErrorImproperValue);
    }

    fromMap(&value);
}

void RegistryItem::fromUr(const std::string& urStr) {
    fromUr(decodeUr(urStr));
}

void RegistryItem::fromUr(const ur::URDecoder& urDecoder) {
    if (!urDecoder.is_complete()) {
        throw CborException("UR decoder result is incomplete", CborErrorIO);
    }
    fromUr(urDecoder.result_ur());
}


