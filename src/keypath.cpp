#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <stdexcept>

#include "keypath.h"

Keypath::Keypath(){
    setRegistryType(KEYPATH);
    setLegacyRegistryType(CRYPTO_KEYPATH);
    setUseLegacyType(false);
}

Keypath::Keypath(const std::string& derivationPath) : Keypath() {
    this->setDerivationPath(derivationPath);
}

Keypath::Keypath(const std::string& derivationPath, uint32_t sourceFingerprint) : Keypath() {
    this->setDerivationPath(derivationPath);
    this->setSourceFingerprint(sourceFingerprint);
}

Keypath::Keypath(const std::string& derivationPath, uint32_t sourceFingerprint, bool withDepth) : Keypath() {
    this->setDerivationPath(derivationPath);
    this->setSourceFingerprint(sourceFingerprint);
    if (withDepth) this->setDepth();
}

void encodeComponents(CborEncoder* encoder, const std::vector<KeyPathComponent>& components) {
    CborError err = CborNoError;
    CborEncoder arrayEncoder;
    uint32_t arraySize = 0;

    // Determine the size of the components to be encoded in the CBOR map
    for (const auto& component : components) {
        // ChildPair contains only one array
        if (component.getType() == KeyPathComponent::Type::ChildPair) {
            arraySize++; 
        // Other types contain two elements: an index or an array with a boolean
        } else {
            arraySize+=2;
        }
    }
    err = cbor_encoder_create_array(encoder, &arrayEncoder, arraySize);
    checkCborError(err, "Failed to create array");

    for (const auto& component : components) {
        component.toMap(&arrayEncoder); 
    }

    err = cbor_encoder_close_container(encoder, &arrayEncoder);
    checkCborError(err, "Failed to close container");
}

std::vector<KeyPathComponent> decodeComponents(CborValue* array) {
    checkIsArray(array);
    std::vector<KeyPathComponent> components;

    CborValue it;
    CborError err = cbor_value_enter_container(array, &it);
    checkCborError(err, "Failed to enter container");

    while (!cbor_value_at_end(&it)) {
        KeyPathComponent component;
        component.fromMap(&it);
        components.push_back(component);

        // Update to the next CBOR element only if the last element was not an array, i.e. not a Child pair component
        if (component.getType() != KeyPathComponent::Type::ChildPair){
            err = cbor_value_advance(&it);
            checkCborError(err, "Failed to advance");
        }
    }

    return components;
}

size_t Keypath::getMapSize() const {
    size_t mapSize = MIN_MAP_LENGTH;
    if (m_source_fingerprint.has_value()) ++mapSize;
    if (m_depth.has_value()) ++mapSize;
    return mapSize;
}

void Keypath::toMap(CborEncoder* parentEncoder) const {
    CborError err = CborNoError;
    CborEncoder container;
    size_t mapSize = getMapSize();

    err = cbor_encoder_create_map(parentEncoder, &container, mapSize);
    checkCborError(err, "Failed to create map");

    err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::COMPONENTS));
    checkCborError(err, "Failed to encode uint");
    
    encodeComponents(&container, m_components);

    if (m_source_fingerprint.has_value()) {
        err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::SOURCE_FINGERPRINT));
        checkCborError(err, "Failed to encode uint");
        if (m_source_fingerprint.value() == 0) {
            throw CborException("Fingerprint cannot be equal to 0", CborErrorExcludedValue);
        }
        err = cbor_encode_uint(&container, m_source_fingerprint.value());
        checkCborError(err, "Failed to encode uint");
    }

    if (m_depth.has_value()) {
        err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::DEPTH));
        checkCborError(err, "Failed to encode uint");
        err = cbor_encode_uint(&container, m_depth.value());
        checkCborError(err, "Failed to encode uint");
    }

    err = cbor_encoder_close_container(parentEncoder, &container);
    checkCborError(err, "Failed to close container");
}

void Keypath::fromMap(CborValue* map) {
    CborError err = CborNoError;
    CborValue container;
    m_components.clear();

    err = cbor_value_enter_container(map, &container);
    checkCborError(err, "Failed to enter container");

    bool componentsFound = false;
    bool fingerprintFound = false;
    bool depthFound = false;
    while (!cbor_value_at_end(&container)) {
        uint64_t key;
        checkIsUint(&container);
        err = cbor_value_get_uint64(&container, &key);
        checkCborError(err, "Failed to get uint");
        checkMaxUint8(key);

        err = cbor_value_advance(&container);
        checkCborError(err, "Failed to advance");

        switch (static_cast<Key>(key)) {
            case Key::COMPONENTS: {
                if (componentsFound) {
                    throw CborException("Components map key duplicated", CborErrorMapKeysNotUnique);
                }
                componentsFound = true;

                checkIsArray(&container);
                m_components = decodeComponents(&container);
                break;
            }
            case Key::SOURCE_FINGERPRINT: {
                if (fingerprintFound) {
                    throw CborException("Fingerprint map key duplicated", CborErrorMapKeysNotUnique);
                }
                fingerprintFound = true;

                uint64_t fingerprint;
                checkIsUint(&container);
                err = cbor_value_get_uint64(&container, &fingerprint);
                checkCborError(err, "Failed to get uint");

                checkMaxUint32(fingerprint);
                if (fingerprint == 0) {
                    throw CborException("Fingerprint cannot be equal to 0", CborErrorExcludedValue);
                } 
                m_source_fingerprint = static_cast<uint32_t>(fingerprint);
                break;
            }
            case Key::DEPTH: {
                if (depthFound) {
                    throw CborException("Depth map key duplicated", CborErrorMapKeysNotUnique);
                }
                depthFound = true;

                uint64_t depth;
                checkIsUint(&container);
                err = cbor_value_get_uint64(&container, &depth);
                checkCborError(err, "Failed to get uint");

                checkMaxUint8(depth);
                m_depth = static_cast<uint8_t>(depth);
                break;
            }
            default: {
                throw CborException("Unknown map key", CborErrorUnknownType);
            }
        }

        err = cbor_value_advance(&container);
        checkCborError(err, "Failed to advance");
    }

    // Verify that mandatory keys are present
    if (!componentsFound) {
        throw CborException("Mandatory map key missing", CborErrorTooFewItems);
    }

    err = cbor_value_leave_container(map, &container);
    checkCborError(err, "Failed to leave container");
}

void Keypath::addKeyPathComponent(const KeyPathComponent& keypathcomponent)
{
    m_components.push_back(keypathcomponent);
}

std::string Keypath::getDerivationPath() const
{
    std::stringstream ss;

    ss << "m";

    for (const auto& component : m_components) {
        ss << "/";
        switch (component.getType()) {
            case KeyPathComponent::Type::ChildIndex: {
                ss << component.getChildIndexValue();
                if (component.getChildIndexIsHardened()) {
                    ss << "'";
                }
                break;
            }
            case KeyPathComponent::Type::ChildRange: {
                ss << "["  << component.getLowIndexValue() << "," << component.getHighIndexValue() << "]";
                if (component.getChildRangeIsHardened()) {
                    ss << "'";
                }
                break;
            }
            case KeyPathComponent::Type::ChildWildcard: {
                ss << "[]";
                if (component.getChildWildcardIsHardened()) {
                    ss << "'";
                }
                break;
            }
            case KeyPathComponent::Type::ChildPair: {
                ss << "<" << component.getExternalAddressIndexValue();
                if (component.getExternalIndexIsHardened()) {
                    ss << "'";
                }
                ss << ";" << component.getInternalAddressIndexValue();
                if (component.getInternalIndexIsHardened()) {
                    ss << "'";
                }
                ss << ">";
                break;
            }
            default:
                ss << "err";
                break;
        }
    }

    return ss.str();
}

void Keypath::setDerivationPath(const std::string& derivationPath) {
    m_components.clear();

    // Validate the derivation path format
    if (derivationPath.empty() || derivationPath[0] != 'm') {
        throw CborException("Derivation path must start with 'm'", CborErrorIO);
    }

    std::stringstream ss(derivationPath);
    std::string segment;

    // Skip the 'm' prefix
    std::getline(ss, segment, '/');

    // Regex patterns for each type of component
    std::regex childIndexRegex(R"(^(\d+)(('|h)?)$)"); // e.g., 0 or 0'
    std::regex childRangeRegex(R"(^\[(\d+),(\d+)\](('|h)?)$)"); // e.g., [0,1] or [0,1]'
    std::regex wildcardRegex(R"(^\[\](('|h)?)$)"); // e.g., [] or []'
    std::regex childPairRegex(R"(^\<(\d+)(('|h)?);(\d+)(('|h)?)\>$)"); // e.g., <0;1'> or <0';1>

    while (std::getline(ss, segment, '/')) {
        std::smatch match;

        if (std::regex_match(segment, match, childIndexRegex)) {
            uint32_t index = std::stoul(match[1].str());
            bool isHardened = !match[2].str().empty();
            m_components.emplace_back(index, isHardened);
        } else if (std::regex_match(segment, match, childRangeRegex)) {
            uint32_t lowIndex = std::stoul(match[1].str());
            uint32_t highIndex = std::stoul(match[2].str());
            bool isHardened = !match[3].str().empty();
            m_components.emplace_back(lowIndex, highIndex, isHardened);
        } else if (std::regex_match(segment, match, wildcardRegex)) {
            bool isHardened = !match[1].str().empty();
            m_components.emplace_back(isHardened);
        } else if (std::regex_match(segment, match, childPairRegex)) {
            uint32_t externalIndex = std::stoul(match[1].str());
            bool externalHardened = !match[2].str().empty();
            uint32_t internalIndex = std::stoul(match[4].str());
            bool internalHardened = !match[5].str().empty();
            m_components.emplace_back(externalIndex, externalHardened, internalIndex, internalHardened);
        } else {
            throw CborException("Invalid derivation path segment: " + segment, CborErrorIO);
        }
    }
}

bool Keypath::hasHardenedPath() const {
    bool isPathHardened = false;
    for (const auto& component : m_components) {
        switch (component.getType()) {
            case KeyPathComponent::Type::ChildIndex: {
                isPathHardened |= component.getChildIndexIsHardened();
                break;
            }
            case KeyPathComponent::Type::ChildRange: {
                isPathHardened |= component.getChildRangeIsHardened();
                break;
            }
            case KeyPathComponent::Type::ChildWildcard: {
                isPathHardened |= component.getChildWildcardIsHardened();
                break;
            }
            case KeyPathComponent::Type::ChildPair: {
                isPathHardened |= component.getExternalIndexIsHardened();
                isPathHardened |= component.getInternalIndexIsHardened();
                break;
            }
            default: {
                std::cerr << "Unknown child type" << std::endl;
                break;
            }
        }
    }

    return isPathHardened;
}