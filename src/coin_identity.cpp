#include <sstream>

#include "coin_identity.h"
#include "uai.h"
#include <bc-ur/utils.hpp>

const std::unordered_map<std::string, EllipticCurve> CoinIdentity::CurveMap = {
    {"p256", EllipticCurve::P256},
    {"p384", EllipticCurve::P384},
    {"p521", EllipticCurve::P521},
    {"p521", EllipticCurve::P521},
    {"x25519", EllipticCurve::X25519},
    {"x448", EllipticCurve::X448},
    {"ed25519", EllipticCurve::Ed25519},
    {"ed448", EllipticCurve::Ed448},
    {"secp256k1", EllipticCurve::Secp256k1}};

CoinIdentity::CoinIdentity()
{
    setRegistryType(COIN_IDENTITY);
}

CoinIdentity::CoinIdentity(EllipticCurve curve, uint32_t type) : CoinIdentity()
{
    setCurve(curve);
    setType(type);
}

CoinIdentity::CoinIdentity(EllipticCurve curve, uint32_t type, const std::vector<SubTypeExp> &subtypes) : CoinIdentity(curve, type)
{
    setSubTypes(subtypes);
}

CoinIdentity::CoinIdentity(const std::string &uai) : CoinIdentity()
{
    setCoinIdentity(uai);
}

void CoinIdentity::clear()
{
    m_curve = EllipticCurve::P256;
    m_type = 0;
    m_subtypes = std::nullopt;
}

void CoinIdentity::setType(uint32_t type)
{
    checkMaxUint31(type);
    m_type = type;
}

size_t CoinIdentity::getMapSize() const
{
    size_t size = MIN_MAP_LENGTH; // curve and type are mandatory
    if (m_subtypes.has_value() && !m_subtypes->empty())
    {
        size++;
    }
    return size;
}

void CoinIdentity::toMap(CborEncoder *parentEncoder) const
{
    CborError err = CborNoError;
    CborEncoder container;
    size_t mapSize = getMapSize();

    err = cbor_encoder_create_map(parentEncoder, &container, mapSize);
    checkCborError(err, "Failed to create map");

    // Encode curve
    err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::CURVE));
    checkCborError(err, "Failed to encode curve key");
    err = cbor_encode_uint(&container, static_cast<uint8_t>(m_curve));
    checkCborError(err, "Failed to encode curve value");

    // Encode type
    err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::TYPE));
    checkCborError(err, "Failed to encode type key");
    err = cbor_encode_uint(&container, m_type);
    checkCborError(err, "Failed to encode type value");

    // Encode subtypes if present
    if (m_subtypes.has_value() && !m_subtypes->empty())
    {
        err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::SUBTYPE));
        checkCborError(err, "Failed to encode subtype key");

        // Create array for subtypes
        CborEncoder array;
        err = cbor_encoder_create_array(&container, &array, m_subtypes->size());
        checkCborError(err, "Failed to create subtype array");

        // Encode each subtype
        for (const auto &subtype : *m_subtypes)
        {
            switch (subtype.getType())
            {
            case SubTypeExp::Type::UINT32:
            {
                err = cbor_encode_uint(&array, subtype.getUint32Value());
                checkCborError(err, "Failed to encode uint32 subtype");
                break;
            }
            case SubTypeExp::Type::STRING:
            {
                err = cbor_encode_text_string(&array, subtype.getStringValue().c_str(), subtype.getStringValue().length());
                checkCborError(err, "Failed to encode string subtype");
                break;
            }
            case SubTypeExp::Type::HEX_STRING:
            {
                // Tag 263 for hex string
                err = cbor_encode_tag(&array, HEX_STRING.tag());
                checkCborError(err, "Failed to encode hex string tag");

                const auto &hexValue = subtype.getHexValue();
                err = cbor_encode_byte_string(&array, hexValue.data(), hexValue.size());
                checkCborError(err, "Failed to encode hex string value");
                break;
            }
            }
        }

        err = cbor_encoder_close_container(&container, &array);
        checkCborError(err, "Failed to close subtype array");
    }

    err = cbor_encoder_close_container(parentEncoder, &container);
    checkCborError(err, "Failed to close container");
}

void CoinIdentity::fromMap(CborValue *map)
{
    CborError err = CborNoError;
    CborValue container;
    m_subtypes.reset(); // Clear any existing subtypes

    err = cbor_value_enter_container(map, &container);
    checkCborError(err, "Failed to enter container");

    bool curveFound = false;
    bool typeFound = false;
    bool subtypeFound = false;

    while (!cbor_value_at_end(&container))
    {
        uint64_t key;
        checkIsUint(&container);
        err = cbor_value_get_uint64(&container, &key);
        checkCborError(err, "Failed to get uint");
        checkMaxUint8(key);

        err = cbor_value_advance(&container);
        checkCborError(err, "Failed to advance");

        switch (static_cast<Key>(key))
        {
        case Key::CURVE:
        {
            if (curveFound)
            {
                throw CborException("Curve map key duplicated", CborErrorMapKeysNotUnique);
            }
            curveFound = true;

            uint64_t curve;
            checkIsUint(&container);
            err = cbor_value_get_uint64(&container, &curve);
            checkCborError(err, "Failed to get uint");
            checkMaxUint8(curve);

            // Validate curve value
            if (curve < static_cast<uint8_t>(EllipticCurve::P256) ||
                curve > static_cast<uint8_t>(EllipticCurve::Secp256k1))
            {
                throw CborException("Invalid curve value", CborErrorImproperValue);
            }

            m_curve = static_cast<EllipticCurve>(curve);
            break;
        }
        case Key::TYPE:
        {
            if (typeFound)
            {
                throw CborException("Type map key duplicated", CborErrorMapKeysNotUnique);
            }
            typeFound = true;

            uint64_t type;
            checkIsUint(&container);
            err = cbor_value_get_uint64(&container, &type);
            checkCborError(err, "Failed to get uint");
            checkMaxUint31(type);

            m_type = static_cast<uint32_t>(type);
            break;
        }
        case Key::SUBTYPE:
        {
            if (subtypeFound)
            {
                throw CborException("Subtype map key duplicated", CborErrorMapKeysNotUnique);
            }
            subtypeFound = true;

            checkIsArray(&container);

            // Get array size
            size_t arraySize = 0;
            err = cbor_value_get_array_length(&container, &arraySize);
            checkCborError(err, "Failed to get array length");

            if (arraySize == 0)
            {
                throw CborException("Subtype array cannot be empty", CborErrorImproperValue);
            }

            // Enter array container
            CborValue array;
            err = cbor_value_enter_container(&container, &array);
            checkCborError(err, "Failed to enter array container");

            // Parse array elements
            std::vector<SubTypeExp> subtypes;
            while (!cbor_value_at_end(&array))
            {
                if (cbor_value_is_unsigned_integer(&array))
                {
                    uint64_t value;
                    err = cbor_value_get_uint64(&array, &value);
                    checkCborError(err, "Failed to get uint");
                    checkMaxUint32(value);
                    subtypes.emplace_back(static_cast<uint32_t>(value));
                }
                else if (cbor_value_is_text_string(&array))
                {
                    size_t length;
                    err = cbor_value_get_string_length(&array, &length);
                    checkCborError(err, "Failed to get string length");

                    std::string value(length, '\0');
                    err = cbor_value_copy_text_string(&array, value.data(), &length, nullptr);
                    checkCborError(err, "Failed to copy text string");

                    subtypes.emplace_back(value);
                }
                else if (cbor_value_is_tag(&array))
                {
                    CborTag tag;
                    err = cbor_value_get_tag(&array, &tag);
                    checkCborError(err, "Failed to get tag");

                    if (tag != HEX_STRING.tag())
                    {
                        throw CborException("Invalid hex string tag", CborErrorInappropriateTagForType);
                    }

                    // Advance the iterator to reach the byte string
                    err = cbor_value_advance(&array);
                    checkCborError(err, "Failed to skip tag");

                    size_t length;
                    checkIsByteStr(&array);
                    err = cbor_value_get_string_length(&array, &length);
                    checkCborError(err, "Failed to get string length");

                    std::vector<uint8_t> value(length);
                    err = cbor_value_copy_byte_string(&array, value.data(), &length, &array);
                    checkCborError(err, "Failed to copy byte string");

                    subtypes.emplace_back(value);

                    // Skip the advance since copy_byte_string already advanced
                    continue;
                }
                else
                {
                    throw CborException("Invalid subtype value", CborErrorIllegalType);
                }

                err = cbor_value_advance(&array);
                checkCborError(err, "Failed to advance");
            }

            err = cbor_value_leave_container(&container, &array);
            checkCborError(err, "Failed to leave array container");

            m_subtypes = std::move(subtypes);
            break;
        }
        default:
        {
            throw CborException("Unknown map key", CborErrorUnknownType);
        }
        }

        if (key != static_cast<uint64_t>(Key::SUBTYPE))
        {
            err = cbor_value_advance(&container);
            checkCborError(err, "Failed to advance");
        }
    }

    // Verify that mandatory keys are present
    if (!curveFound)
    {
        throw CborException("Mandatory curve key missing", CborErrorTooFewItems);
    }
    if (!typeFound)
    {
        throw CborException("Mandatory type key missing", CborErrorTooFewItems);
    }

    err = cbor_value_leave_container(map, &container);
    checkCborError(err, "Failed to leave container");
}

std::string CoinIdentity::toUaiStr() const
{
    std::string uai{};

    // cppcheck-suppress unassignedVariable
    for (const auto &[curve_name, curve_int] : CurveMap)
    {
        if (curve_int == m_curve)
        {
            uai = "uai://" + curve_name + "." + std::to_string(m_type);
            break;
        }
    }

    if (m_subtypes.has_value() && !m_subtypes.value().empty())
    {
        for (const auto &subtype : m_subtypes.value())
        {
            std::string subtypeStr = "";

            switch (subtype.getType())
            {
            case SubTypeExp::Type::HEX_STRING:
                subtypeStr = "0x" + ur::data_to_hex(subtype.getHexValue());
                break;
            case SubTypeExp::Type::STRING:
                subtypeStr = subtype.getStringValue();
                break;
            case SubTypeExp::Type::UINT32:
                subtypeStr = std::to_string(subtype.getUint32Value());
                break;
            default:
                break;
            }

            uai += "." + subtypeStr;
        }
    }

    return uai;
}

static EllipticCurve parseCurveName(const std::string &curveName)
{

    std::string lowerCurveName = curveName;
    std::transform(lowerCurveName.begin(), lowerCurveName.end(), lowerCurveName.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });

    auto it = CoinIdentity::CurveMap.find(lowerCurveName);
    if (it != CoinIdentity::CurveMap.end())
    {
        return it->second;
    }
    throw CborException("Unknown curve name: " + curveName, CborErrorImproperValue);
}

static SubTypeExp parseSubType(const std::string &subtypeStr)
{
    // Check for hex number
    if (subtypeStr.length() > 2 && (subtypeStr.substr(0, 2) == "0x" || subtypeStr.substr(0, 2) == "0X"))
    {
        try
        {
            std::vector<uint8_t> hexValue;
            for (size_t i = 2; i < subtypeStr.length(); i += 2)
            {
                std::string byteString = subtypeStr.substr(i, 2);
                uint8_t byte = std::stoul(byteString, nullptr, 16);
                hexValue.push_back(byte);
            }
            return SubTypeExp{hexValue};
        }
        catch (const std::invalid_argument &)
        {
            // Not a valid hexadecimal number, fall through to number parsing
        }
    }

    try
    {
        std::size_t pos{0};
        uint32_t decimalValue = std::stoul(subtypeStr, &pos, 10);
        if (pos == subtypeStr.size())
        {
            return SubTypeExp{decimalValue};
        }
    }
    catch (const std::invalid_argument &)
    {
        // Not a valid hexadecimal number, fall through to string parsing
    }

    return SubTypeExp{subtypeStr};
}

void CoinIdentity::setCoinIdentity(const std::string &uai)
{
    UniqueAssetId UaiObject(uai);

    auto curveName = UaiObject.getCurveName();
    m_curve = parseCurveName(curveName);

    auto typeStr = UaiObject.getType();
    m_type = std::stoul(typeStr);

    auto subtypesStr = UaiObject.getSubtypes();
    if (!subtypesStr.empty())
    {
        std::vector<SubTypeExp> subtypes;
        for (auto subtype : subtypesStr)
        {
            SubTypeExp subtypeExp = parseSubType(subtype);
            subtypes.push_back(subtypeExp);
        }
        m_subtypes = subtypes;
    }
    else
    {
        m_subtypes = std::nullopt;
    }
}
