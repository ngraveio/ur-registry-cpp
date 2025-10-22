#pragma once

#include <tinycbor/cbor.h>
#include <cstdint>
#include <string>
#include <optional>
#include <limits>

#include <bc-ur/utils.hpp>
#include <bc-ur/bc-ur.hpp>
#include "cbor_exception.h"
#include "registry_type.h"

/**
 * @brief Maximum buffer size for CBOR encoding
 * 
 */
constexpr size_t MAX_CBOR_SIZE = 16384;

/**
 * @brief Max fragment length to encode UR type
 */
constexpr size_t DEFAULT_UR_MAXFRAGMENT = 90;

/**
 *
 * @brief BC-UR registry item. Abstract base class for all UR types in the registry
 **/
class RegistryItem
{

public:
     virtual ~RegistryItem() = default;

    /// @brief Methods to be implemented in derived UR type classes
    /**
     * @brief Get CBOR map size to encode.
     * 
     * @return Map size
     */
    virtual size_t getMapSize() const = 0;

    /**
     * @brief Get minimum CBOR map size of the UR type.
     * 
     * @return Map size
     */
    virtual size_t getMinMapLength() const = 0;

    /**
     * @brief Get maximum CBOR map size of the UR type.
     * 
     * @return Map size
     */
    virtual size_t getMaxMapLength() const = 0;

    /**
     * @brief Encode UR registry object as a CBOR map.
     * 
     */
    virtual void toMap(CborEncoder* parentEncoder) const = 0;

    /**
     * @brief Decode CBOR map into UR registry object.
     * 
     * @param map contains the encoded CBOR value as CborValue.
     */
    virtual void fromMap(CborValue* map) = 0;




    /// @brief Common implementations.
    /**
     * @brief Encodes UR registry object as CBOR bytes.
     * 
     * @param cbor contains the CBOR bytes as Bytevector.
     */
    ur::ByteVector toCbor() const;

    /**
     * @brief Encode as a single part UR type.
     * 
     * @param ur contains the encoded UR type as string in a single fragment.
     */
    std::string toSinglePartUr() const;

    /**
     * @brief Encode as UR type with possibly several fragments.
     * 
     * @param max_fragment_length defines the max fragment length for each QR code.
     * @return ur::UREncoder 
     */
    ur::UREncoder toUr(uint32_t max_fragment_length = DEFAULT_UR_MAXFRAGMENT) const;

    /**
     * @brief Decode UR with different methods
     * 
     * @param ur contains the UR object to decode
     * or
     * @param urStr contains the UR string to decode
     * or
     * @param urDecoder contains the UR decoder object with the decoded UR
     */
    void fromUr(const ur::UR& ur);
    void fromUr(const std::string& urStr);
    void fromUr(const ur::URDecoder& urDecoder);

    /**
     * @brief Get the Registry Type including both string type and CBOR tag
     * 
     * @return RegistryType 
     */
    RegistryType getRegistryType() const { return m_registryType; };

    /**
     * @brief Get the Legacy Registry Type (only if supported)
     * 
     * @return RegistryType 
     */
    std::optional<RegistryType> getLegacyRegistryType() const { return m_legacyRegistryType; }

    /**
     * @brief Set the Registry Type including both string type and CBOR tag
     * 
     * @param RegistryType 
     */
    void setRegistryType(const RegistryType& registryType) { m_registryType = registryType; }

    /**
     * @brief Set the Legacy Registry Type including both string type and CBOR tag (only if supported)
     * 
     * @param RegistryType 
     */
    void setLegacyRegistryType(const RegistryType& legacyRegistryType) { m_legacyRegistryType = legacyRegistryType; }

    /**
     * @brief Get the Use Legacy Type flag
     * 
     * @return True if the legacy type is used, false otherwise.
     */
    std::optional<bool> getUseLegacyType() const { return m_useLegacy; }

    /**
     * @brief Set the flag to use the Legacy Type 
     * 
     * @param useLegacy to true if the legacy type is set and to false otherwise.
     */
    void setUseLegacyType(bool useLegacy) { m_useLegacy = useLegacy; }

private:
    RegistryType m_registryType;
    std::optional<RegistryType> m_legacyRegistryType;
    std::optional<bool> m_useLegacy;

    ur::UR encodeUr() const;
};

