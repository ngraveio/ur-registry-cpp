#pragma once

#include <iostream>
#include <vector>
#include <optional>

#include "registry_item.h"


/**
 * @brief Implementation of the coin-identity (CBOR tag #6.41401) UR type
 *
 * Source: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2023-001-coin-identity.md
 * CDDL specification:
 *
 * coin-identity = {
 *     curve: elliptic_curve,
 *     type: uint31, ; values from [SLIP44] with high bit turned off,
 *     ? subtype: [ sub_type_exp + ]  ; Compatible with the definition of several subtypes if necessary
 * }
 *
 * curve = 1
 * type = 2
 * subtype = 3
 *
 * elliptic_curve = P256 / P384 / P521 / X25519 / X448 / Ed25519 / Ed448 / secp256k1
 *
 * P256=1	            ; NIST P-256 also known as secp256r1
 * P384=2	            ; NIST P-384 also known as secp384r1
 * P521=3	            ; EC2	NIST P-521 also known as secp521r1
 * X25519=4            ; X25519 for use w/ ECDH only
 * X448=5              ; X448 for use w/ ECDH only
 * Ed25519=6           ; Ed25519 for use w/ EdDSA only
 * Ed448=7             ; Ed448 for use w/ EdDSA only
 * secp256k1=8         ; SECG secp256k1 curve	IESG
 *
 * hex_string = #6.263(bstr) ; byte string is a hexadecimal string no need for decoding
 * sub_type_exp = uint32 / str / hex_string
 */

enum class EllipticCurve : uint8_t {
    P256 = 1,
    P384 = 2,
    P521 = 3,
    X25519 = 4,
    X448 = 5,
    Ed25519 = 6,
    Ed448 = 7,
    Secp256k1 = 8
};

// Forward declaration for variant type
class SubTypeExp;

class CoinIdentity : public RegistryItem {
public:
    CoinIdentity();  // Default constructor needed for decoding
    virtual ~CoinIdentity() override = default;
    explicit CoinIdentity(EllipticCurve curve, uint32_t type);
    CoinIdentity(EllipticCurve curve, uint32_t type, const std::vector<SubTypeExp>& subtypes);
    explicit CoinIdentity(const std::string& uai);

    void clear();
    
    static const std::unordered_map<std::string, EllipticCurve> CurveMap;

    enum class Key {
        CURVE = 1,
        TYPE,
        SUBTYPE
    };

    size_t getMinMapLength() const override {
        return MIN_MAP_LENGTH;
    }

    size_t getMaxMapLength() const override {
        return MAX_MAP_LENGTH;
    }

    EllipticCurve getCurve() const { return m_curve; }
    uint32_t getType() const { return m_type; }
    const std::optional<std::vector<SubTypeExp>>& getSubTypes() const { return m_subtypes; }

    void setCurve(EllipticCurve curve) { m_curve = curve; }
    void setType(uint32_t type);
    void setSubTypes(const std::vector<SubTypeExp>& subtypes) { m_subtypes = subtypes; }

    size_t getMapSize() const override;
    void toMap(CborEncoder* parentEncoder) const override;
    void fromMap(CborValue* map) override;
    
    std::string toUaiStr() const;

    bool operator == (const CoinIdentity& coin_id) const {
        if (m_curve != coin_id.getCurve() || m_type != coin_id.getType()){
            return false;
        }
        if (m_subtypes.has_value() != coin_id.getSubTypes().has_value()) {
            return false;
        }
        if (m_subtypes.has_value()) {
            std::vector<SubTypeExp> subtypes = *m_subtypes;
            std::vector<SubTypeExp> subtypes_coinid = *coin_id.getSubTypes();
            if (subtypes != subtypes_coinid) {
                return false;
            }
        }
        return true;
    };

    /**
     * @brief Encode UAI string (curve, type and subtypes) to coin identity 
     * 
     * @param uai is the Unique Asset ID format as defined in NBCR-2024-01
     * 
     * Source: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2024-001-unique-asset-id.md
     */
    void setCoinIdentity(const std::string& uai);

private:
    static constexpr size_t MIN_MAP_LENGTH = 2;
    static constexpr size_t MAX_MAP_LENGTH = 3;

    EllipticCurve m_curve{EllipticCurve::P256};  // Default to P256
    uint32_t m_type{0};  // Default to Bitcoin (SLIP44 = 0)
    std::optional<std::vector<SubTypeExp>> m_subtypes;
};

// Variant type for subtype expressions
class SubTypeExp {
public:
    enum class Type {
        UINT32,
        STRING,
        HEX_STRING
    };

    // Constructors for different types
    explicit SubTypeExp(uint32_t value) : m_type(Type::UINT32), m_uint32_value(value) {}
    explicit SubTypeExp(const std::string& value) : m_type(Type::STRING), m_string_value(value) {}
    explicit SubTypeExp(const std::vector<uint8_t>& value) : m_type(Type::HEX_STRING), m_hex_value(value) {}

    Type getType() const { return m_type; }
    uint32_t getUint32Value() const { return m_uint32_value; }
    const std::string& getStringValue() const { return m_string_value; }
    const std::vector<uint8_t>& getHexValue() const { return m_hex_value; }

    bool operator == (const SubTypeExp& subtype) const {
        if (m_type != subtype.getType()) {
            return false;
        }
        switch (m_type) {
            case Type::UINT32: {
                if (m_uint32_value == subtype.getUint32Value()) {
                    return true;
                }
                break;
            }
            case Type::STRING: {
                if (m_string_value == subtype.getStringValue()) {
                    return true;
                }
                break;
            }
            case Type::HEX_STRING: {
                if (m_hex_value == subtype.getHexValue()) {
                    return true;
                }
                break;
            }
        }
        return false;
    };
        
private:
    Type m_type;
    uint32_t m_uint32_value{0};
    std::string m_string_value;
    std::vector<uint8_t> m_hex_value;
};
