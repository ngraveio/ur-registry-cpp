#include "hdkey.h"

HDKey::HDKey(){
    setRegistryType(HDKEY);
    setLegacyRegistryType(CRYPTO_HDKEY);
    setUseLegacyType(false);
    std::fill(m_keyData.begin(), m_keyData.end(), 0);
}

size_t HDKey::getMapSize() const {
    size_t mapSize = MIN_MAP_LENGTH;
    if (m_ismaster.has_value()) ++mapSize;
    if (m_isprivate.has_value()) ++mapSize;
    if (m_chainCode.has_value()) ++mapSize;
    if (m_useInfo.has_value()) ++mapSize;
    if (m_origin.has_value()) ++mapSize;
    if (m_children.has_value()) ++mapSize;
    if (m_parentFingerprint.has_value()) ++mapSize;
    if (m_name.has_value()) ++mapSize;
    if (m_note.has_value()) ++mapSize;
    return mapSize;
}

void HDKey::toMap(CborEncoder* parentEncoder) const {
    CborError err = CborNoError;
    CborEncoder container;
    size_t mapSize = getMapSize();

    err = cbor_encoder_create_map(parentEncoder, &container, mapSize);
    checkCborError(err, "Failed to create map");

    if (m_ismaster.has_value()) {
        err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::IS_MASTER));
        checkCborError(err, "Failed to encode uint");

        err = cbor_encode_boolean(&container, m_ismaster.value());
        checkCborError(err, "Failed to encode boolean");

        // Master key
        if (m_ismaster.value()) {
            if (m_isprivate.has_value() || m_origin.has_value() || m_children.has_value() || 
            m_parentFingerprint.has_value() || m_name.has_value() || m_note.has_value()) {
                throw CborException("Illegal map key to encode a master key", CborErrorIllegalType);
            }

            if (!m_chainCode.has_value()) {
                throw CborException("Missing chain code key to encode a master key", CborErrorTooFewItems);
            }

            err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::KEY_DATA));
            checkCborError(err, "Failed to encode uint");

            err = cbor_encode_byte_string(&container, m_keyData.data(), KEY_DATA_SIZE);
            checkCborError(err, "Failed to encode byte string");

            err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::CHAIN_CODE));
            checkCborError(err, "Failed to encode uint");

            err = cbor_encode_byte_string(&container, m_chainCode.value().data(), CHAIN_CODE_SIZE);
            checkCborError(err, "Failed to encode byte string");

            err = cbor_encoder_close_container(parentEncoder, &container);
            checkCborError(err, "Failed to close container");
            return;
        }
    } 

    // Derived key
    if (m_isprivate.has_value()) {
        err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::IS_PRIVATE));
        checkCborError(err, "Failed to encode uint");

        err = cbor_encode_boolean(&container, m_isprivate.value());
        checkCborError(err, "Failed to encode boolean");
    }

    err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::KEY_DATA));
    checkCborError(err, "Failed to encode uint");

    err = cbor_encode_byte_string(&container, m_keyData.data(), KEY_DATA_SIZE);
    checkCborError(err, "Failed to encode byte string");

    if (m_chainCode.has_value()) {
        err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::CHAIN_CODE));
        checkCborError(err, "Failed to encode uint");

        err = cbor_encode_byte_string(&container, m_chainCode.value().data(), CHAIN_CODE_SIZE);
        checkCborError(err, "Failed to encode byte string");
    }

    if (m_useInfo.has_value()) {
        err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::USE_INFO));
        checkCborError(err, "Failed to encode uint");

        if (this->getUseLegacyType().has_value() && this->getUseLegacyType().value()) {
            if (!m_useInfo.value().getLegacyRegistryType().has_value()) {
                throw CborException("Use info legacy type not defined", CborErrorUnknownTag);
            }
            err = cbor_encode_tag(&container, m_useInfo.value().getLegacyRegistryType().value().tag());
        } else {
            err = cbor_encode_tag(&container, m_useInfo.value().getRegistryType().tag());
        }
        checkCborError(err, "Failed to encode tag");

        m_useInfo.value().toMap(&container);
    }

    if (m_origin.has_value()) {
        // Verify coin type and second path of origin are identical as defined by BIP44
        if (m_useInfo.has_value() && m_useInfo.value().getType().has_value()) {
            if ((m_origin.value().getKeyPathComponent().size() < 2) || 
            (m_origin.value().getKeyPathComponent()[1].getType() != KeyPathComponent::Type::ChildIndex) ||
            (m_origin.value().getKeyPathComponent()[1].getChildIndexValue() != static_cast<uint32_t>(m_useInfo.value().getType().value()))) {
                throw CborException("As per BIP44, the second path in `origin` MUST match the coin type of `use-info`", CborErrorImproperValue);
            }
        }

        err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::ORIGIN));
        checkCborError(err, "Failed to encode uint");

        if (this->getUseLegacyType().has_value() && this->getUseLegacyType().value()) {
            if (!m_origin.value().getLegacyRegistryType().has_value()) {
                throw CborException("Origin legacy type not defined", CborErrorUnknownTag);
            }
            err = cbor_encode_tag(&container, m_origin.value().getLegacyRegistryType().value().tag());
        } else {
            err = cbor_encode_tag(&container, m_origin.value().getRegistryType().tag());
        }
        checkCborError(err, "Failed to encode tag");

        m_origin.value().toMap(&container);
    }

    if (m_children.has_value()) {
        if (m_children.value().hasHardenedPath() && !m_isprivate) {
            throw CborException("To derive hardened children, the derived key MUST be private", CborErrorImproperValue);
        }

        err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::CHILDREN));
        checkCborError(err, "Failed to encode tag");

        if (this->getUseLegacyType().has_value() && this->getUseLegacyType().value()) {
            if (!m_children.value().getLegacyRegistryType().has_value()) {
                throw CborException("Children legacy type not defined", CborErrorUnknownTag);
            }
            err = cbor_encode_tag(&container, m_children.value().getLegacyRegistryType().value().tag());
        } else {
            err = cbor_encode_tag(&container, m_children.value().getRegistryType().tag());
        }
        checkCborError(err, "Failed to encode tag");

        m_children.value().toMap(&container);
    }

    if (m_parentFingerprint.has_value()) {
        // Verify parent fingerprint is identical to source fingerprint in case of single derivation path
        if (m_origin.has_value() && m_origin.value().getSourceFingerprint().has_value() && m_origin.value().getKeyPathComponent().size() == 1) {
            if (m_origin.value().getSourceFingerprint().value() != m_parentFingerprint) {
                throw CborException("In case of single derivation path, `parent-fingerprint` MUST be identical to `source-fingerprint`", CborErrorImproperValue);
            }
        }

        err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::PARENT_FINGERPRINT));
        checkCborError(err, "Failed to encode uint");

        err = cbor_encode_uint(&container, m_parentFingerprint.value());
        checkCborError(err, "Failed to encode uint");
    }

    if (m_name.has_value()) {
        err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::NAME));
        checkCborError(err, "Failed to encode uint");

        err = cbor_encode_text_string(&container, m_name.value().c_str(), m_name.value().size());
        checkCborError(err, "Failed to encode uint");
    }

    if (m_note.has_value()) {
        err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::NOTE));
        checkCborError(err, "Failed to encode uint");

        err = cbor_encode_text_string(&container, m_note.value().c_str(), m_note.value().size());
        checkCborError(err, "Failed to encode text string");
    }
    
    err = cbor_encoder_close_container(parentEncoder, &container);
    checkCborError(err, "Failed to close container");
}

void HDKey::deserializeFromBIP32(const std::string& key) {
    BIP32Deserializer::BIP32Key bip32Key = BIP32Deserializer::deserialize(key);

    // Populate HDKey fields using the deserialized BIP32Key
    if (bip32Key.is_master) {
        this->setIsMaster(true);
    } else {
        if (bip32Key.is_private) this->setIsPrivate(true);
        this->setParentFingerprint(bip32Key.parent_fingerprint);
    }
    this->setKeyData(bip32Key.key_data);
    this->setChainCode(bip32Key.chain_code);
}

uint32_t HDKey::extractParentFingerprint(const std::string& key) {
    try {
        BIP32Deserializer::BIP32Key bip32Key = BIP32Deserializer::deserialize(key);
        return bip32Key.parent_fingerprint;
    } catch (const CborException& e) {
        return 0;
    }
}
