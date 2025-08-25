#include "output_descriptor.h"

#include <regex>
#include <charconv>

OutputDescriptor::OutputDescriptor(const std::string &source,
                                   const std::vector<HDKey> &keys,
                                   const std::optional<std::string> &name,
                                   const std::optional<std::string> &note) 
    : m_source(source), m_keys(keys), m_name(name), m_note(note) {
    setRegistryType(OUTPUT_DESCRIPTOR);
    setLegacyRegistryType(CRYPTO_OUTPUT);
    setUseLegacyType(false);
}

size_t OutputDescriptor::getMapSize() const
{
    size_t size = MIN_MAP_LENGTH;
    if(!m_keys.empty())
        size++;
    if(m_name.has_value())
        size++;
    if(m_note.has_value())
        size++;
    return size;
}


void OutputDescriptor::toMap(CborEncoder* parentEncoder) const {
    
    if (this->getUseLegacyType().has_value() && this->getUseLegacyType().value())
        throw CborException("Legacy type is not supported.", CborErrorIllegalType);
    
    CborError err = CborNoError;
    CborEncoder localEncoder;

    err = cbor_encoder_create_map(parentEncoder, &localEncoder, getMapSize());
    checkCborError(err, "Failed to create output-descriptor map");    

    if(m_source.empty())
        throw CborException("Source field should not be empty.", CborErrorImproperValue);

    err = cbor_encode_uint(&localEncoder, static_cast<uint8_t>(Key::SOURCE));
    checkCborError(err, "Failed to encode uint");
    err = cbor_encode_text_string(&localEncoder, m_source.c_str(), m_source.size());
    checkCborError(err, "Failed to encode byte string");

    /* Checking if there are placeholders inside parentheses that starts with '@' and separated with a comma. */
    const std::regex regex{R"(@([^,\)]+))"};
    const std::vector<std::string> matches {
        std::sregex_token_iterator{m_source.cbegin(), m_source.cend(), regex, 1},
        std::sregex_token_iterator{}
    };
    
    // cppcheck-suppress knownConditionTrueFalse
    if(matches.empty() && !m_keys.empty())
        throw CborException("Keys field should be empty since the source is in textual format (no placeholder).", CborErrorImproperValue);

    // cppcheck-suppress knownConditionTrueFalse
    if(!matches.empty()) {
        if(m_keys.empty())
            throw CborException("Keys field cannot be empty since the source has placeholders.", CborErrorImproperValue);
        if(matches.size() != m_keys.size())
            throw CborException("Source placeholders length and keys length mismatch.", CborErrorImproperValue);

        err = cbor_encode_uint(&localEncoder, static_cast<uint8_t>(Key::KEYS));
        checkCborError(err, "Failed to encode uint");

        CborEncoder arrayEncoder;
        err = cbor_encoder_create_array(&localEncoder, &arrayEncoder, matches.size());
        checkCborError(err, "Failed to create cbor array localEncoder");

        uint64_t index{};
        for(const std::string_view match: matches) {
            const auto endptr = match.data() + match.size();
            const auto [ptr, err_code] = std::from_chars(match.data(), endptr, index);

            if (ptr != endptr || err_code != std::errc())
                throw CborException("Key placeholder substr in the source str is not a number.", CborErrorImproperValue);

            if(index >= m_keys.size())
                throw CborException("Placeholder index is out of Keys range.", CborErrorImproperValue);
            
            if (this->getUseLegacyType().has_value() && this->getUseLegacyType().value()) {
                if (!m_keys[index].getLegacyRegistryType().has_value()) {
                    throw CborException("Key legacy type not defined", CborErrorUnknownTag);
                }
                err = cbor_encode_tag(&arrayEncoder, m_keys[index].getLegacyRegistryType().value().tag());
            } else {
                err = cbor_encode_tag(&arrayEncoder, m_keys[index].getRegistryType().tag());
            }
            checkCborError(err, "Failed to encode tag");

            m_keys[index].toMap(&arrayEncoder);
        }

        err = cbor_encoder_close_container(&localEncoder, &arrayEncoder);
        checkCborError(err, "Failed to close array localEncoder.");
    }

    if(m_name.has_value()) {
        err = cbor_encode_uint(&localEncoder, static_cast<uint8_t>(Key::NAME));
        checkCborError(err, "Failed to encode uint");
        err = cbor_encode_text_string(&localEncoder, m_name.value().c_str(), m_name.value().size());
        checkCborError(err, "Failed to encode byte string");
    }

    if(m_note.has_value()) {
        err = cbor_encode_uint(&localEncoder, static_cast<uint8_t>(Key::NOTE));
        checkCborError(err, "Failed to encode uint");
        err = cbor_encode_text_string(&localEncoder, m_note.value().c_str(), m_note.value().size());
        checkCborError(err, "Failed to encode byte string");
    }

    err = cbor_encoder_close_container(parentEncoder, &localEncoder);
    checkCborError(err, "Failed to close localEncoder");
}
