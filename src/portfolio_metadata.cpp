#include "portfolio_metadata.h"
#include "cbor_exception.h"
#include <regex>

PortfolioMetadata::PortfolioMetadata() {
    setRegistryType(PORTFOLIO_METADATA);
}

PortfolioMetadata::PortfolioMetadata(const std::vector<uint8_t>& sync_id,
                                   const std::string& language,
                                   const std::string& fw_version,
                                   const std::string& device) {
    setRegistryType(PORTFOLIO_METADATA);
    setSyncId(sync_id);
    setLanguage(language);
    setFwVersion(fw_version);
    setDevice(device);
}

void PortfolioMetadata::setSyncId(const std::vector<uint8_t>& sync_id) {
    if (sync_id.size() != sync_id_size) {
        throw CborException("Invalid sync_id size", CborErrorImproperValue);
    }
    m_sync_id = sync_id;
}

void PortfolioMetadata::setLanguage(const std::string& language) {
    if (language.empty()) {
        throw CborException("Language code cannot be empty", CborErrorImproperValue);
    }

    // ISO 639-1 language codes are exactly 2 characters
    if (language.length() != 2) {
        throw CborException("Invalid language code length", CborErrorImproperValue);
    }

    // ISO 639-1 language codes are lowercase letters only
    std::regex languagePattern("^[a-z]{2}$");
    if (!std::regex_match(language, languagePattern)) {
        throw CborException("Invalid language code format", CborErrorImproperValue);
    }

    m_language = language;
}

void PortfolioMetadata::setFwVersion(const std::string& fw_version) {
    if (fw_version.empty()) {
        throw CborException("Firmware version cannot be empty", CborErrorImproperValue);
    }
    m_fw_version = fw_version;
}

void PortfolioMetadata::setDevice(const std::string& device) {
    if (device.empty()) {
        throw CborException("Device name cannot be empty", CborErrorImproperValue);
    }
    m_device = device;
}

size_t PortfolioMetadata::getMapSize() const {
    size_t size = 0;
    if (m_sync_id) size++;
    if (m_language) size++;
    if (m_fw_version) size++;
    if (m_device) size++;
    return size;
}

void PortfolioMetadata::toMap(CborEncoder* parentEncoder) const {
    CborEncoder mapEncoder;
    size_t size = getMapSize();
    CborError err = cbor_encoder_create_map(parentEncoder, &mapEncoder, size);
    checkCborError(err, "Failed to create map");

    if (m_sync_id) {
        err = cbor_encode_uint(&mapEncoder, static_cast<uint8_t>(Key::SYNC_ID));
        checkCborError(err, "Failed to encode sync_id key");
        err = cbor_encode_byte_string(&mapEncoder, m_sync_id->data(), m_sync_id->size());
        checkCborError(err, "Failed to encode sync_id value");
    }

    if (m_language) {
        err = cbor_encode_uint(&mapEncoder, static_cast<uint8_t>(Key::LANGUAGE));
        checkCborError(err, "Failed to encode language key");
        err = cbor_encode_text_string(&mapEncoder, m_language->c_str(), m_language->length());
        checkCborError(err, "Failed to encode language value");
    }

    if (m_fw_version) {
        err = cbor_encode_uint(&mapEncoder, static_cast<uint8_t>(Key::FW_VERSION));
        checkCborError(err, "Failed to encode fw_version key");
        err = cbor_encode_text_string(&mapEncoder, m_fw_version->c_str(), m_fw_version->length());
        checkCborError(err, "Failed to encode fw_version value");
    }

    if (m_device) {
        err = cbor_encode_uint(&mapEncoder, static_cast<uint8_t>(Key::DEVICE));
        checkCborError(err, "Failed to encode device key");
        err = cbor_encode_text_string(&mapEncoder, m_device->c_str(), m_device->length());
        checkCborError(err, "Failed to encode device value");
    }

    err = cbor_encoder_close_container(parentEncoder, &mapEncoder);
    checkCborError(err, "Failed to close container");
}
