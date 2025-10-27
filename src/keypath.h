#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "keypath_component.h"

#include "registry_item.h"

/**
 * @brief Implementation of the keypath (CBOR tag #6.40304) UR type and its legacy version crypto-keypath (CBOR tag #6.304)
 *
 * Source: https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-2020-007-hdkey.md
 * CDDL specification:
 * ; Metadata for the complete or partial derivation path of a key.
 * ;
 * ; `source-fingerprint`, if present, is the fingerprint of the
 * ; ancestor key from which the associated key was derived.
 * ;
 * ; If `components` is empty, then `source-fingerprint` MUST be a fingerprint of
 * ; a master key.
 * ;
 * ; `depth`, if present, represents the number of derivation steps in
 * ; the path of the associated key, regardless of whether steps are present in the `components` element
 * ; of this structure.
 *
 * tagged-keypath = #6.40304(keypath)
 *
 * keypath = {
 *     components: [path-component], ; If empty, source-fingerprint MUST be present
 *     ? source-fingerprint: uint32 .ne 0 ; fingerprint of ancestor key, or master key if components is empty
 *     ? depth: uint8 ; 0 if this is a public key derived directly from a master key
 * }
 *
 * path-component = (
 *     child-index-component /     ; A single child, possibly hardened
 *     child-range-component /		; A specific range of children, all possibly hardened
 *     child-wildcard-component /  ; An inspecific range of children, all possibly hardened
 *     child-pair-component        ; Used in output descriptors,
 *                                 ; see https://github.com/bitcoin/bitcoin/pull/22838
 * )
 *
 * uint32 = uint .size 4
 * uint31 = uint32 .lt 0x80000000
 * child-index-component = (child-index, is-hardened)
 * child-range-component = ([child-index, child-index], is-hardened) ; [low, high] where low < high
 * child-wildcard-component = ([], is-hardened)
 * child-pair-component = [
 *     child-index-component,	; Child to use for external addresses, possibly hardened
 *     child-index-component	; Child to use for internal addresses, possibly hardened
 * ]
 *
 * child-index = uint31
 * is-hardened = bool
 *
 * components = 1
 * source-fingerprint = 2
 * depth = 3
 */

class Keypath : public RegistryItem
{
public:
    Keypath();
    ~Keypath() override = default;
    explicit Keypath(const std::string &derivationPath);
    Keypath(const std::string &derivationPath, uint32_t sourceFingerprint);
    Keypath(const std::string &derivationPath, uint32_t sourceFingerprint, bool withDepth);

    enum class Key
    {
        COMPONENTS = 1,
        SOURCE_FINGERPRINT,
        DEPTH
    };

    size_t getMinMapLength() const override
    {
        return MIN_MAP_LENGTH;
    }

    size_t getMaxMapLength() const override
    {
        return MAX_MAP_LENGTH;
    }

    size_t getMapSize() const override;
    void toMap(CborEncoder *parentEncoder) const override;
    void fromMap(CborValue *map) override;

    void addKeyPathComponent(const KeyPathComponent &keypathcomponent);
    std::vector<KeyPathComponent> getKeyPathComponent() const { return m_components; }
    std::optional<uint32_t> getSourceFingerprint() const { return m_source_fingerprint; }
    std::optional<uint8_t> getDepth() const { return m_depth; }
    void setSourceFingerprint(const uint32_t source_fingerprint) { m_source_fingerprint = source_fingerprint; }

    /**
     * @brief Set the Depth object to the components size representing the number of derivation steps
     *
     */
    void setDepth()
    {
        if (m_components.empty())
        {
            m_depth = 0;
        }
        else
        {
            m_depth = m_components.size();
        }
    }

    /**
     * @brief Get the Derivation Path object following the format
     * m/1'/2/[3,4]/[5,6]'/[]/[]'/[7,8']/[9',0]
     * Where each component can be either
     *  - a child index possibly hardened (e.g. 1' and 2)
     *  - a child range with a range between two indexes possibly hardened (e.g. [3,4] and [5,6]')
     *  - a child wildcard possibly hardened (e.g. [] and []')
     *  - a child pair with an external and internal addresses, both addresses possibly hardened independently (e.g. [7, 8'] and [9', 0])
     *
     * @return std::string containing the derivation path in the specified format
     */
    std::string getDerivationPath() const;

    /**
     * @brief Set the Keypath components from the derivation path
     * The derivation path should follow the same format as in getDerivationPath
     *
     */
    void setDerivationPath(const std::string &derivationPath);

    /**
     * @brief Indicate if the components include a hardened path
     *
     * @return true in case of one or more hardened path
     * @return false in case of none hardened path
     */
    bool hasHardenedPath() const;

private:
    static constexpr size_t MIN_MAP_LENGTH = 1;
    static constexpr size_t MAX_MAP_LENGTH = 10;

    std::vector<KeyPathComponent> m_components;
    std::optional<uint32_t> m_source_fingerprint;
    std::optional<uint8_t> m_depth;
};
