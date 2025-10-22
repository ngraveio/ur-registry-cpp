#pragma once

#include <stdint.h>

#include "registry_item.h"

/**
 * @brief Implementation of path-component in keypath (CBOR tag #6.40304) UR type
 *
 * Source: https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-2020-007-hdkey.md
 * CDDL specification:
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
 */
class KeyPathComponent
{
public:
    KeyPathComponent();

    enum class Type
    {
        ChildIndex,
        ChildRange,
        ChildWildcard,
        ChildPair
    };

    struct ChildIndexComponent
    {
        uint32_t childIndex;
        bool isHardened;
    };

    struct ChildRangeComponent
    {
        uint32_t lowIndex;
        uint32_t highIndex;
        bool isHardened;
    };

    struct ChildWildcardComponent
    {
        bool isHardened;
    };

    struct ChildPairComponent
    {
        uint32_t externalAddressIndex;
        bool externalAddressIsHardened;
        uint32_t internalAddressIndex;
        bool internalAddressIsHardened;
    };

    /**
     * @brief Constructors for the different path component types
     *
     */
    KeyPathComponent(uint32_t childIndex, bool isHardened)
        : m_type(Type::ChildIndex), m_childIndexComponent({childIndex, isHardened}) {}

    KeyPathComponent(uint32_t lowIndex, uint32_t highIndex, bool isHardened)
        : m_type(Type::ChildRange), m_childRangeComponent({lowIndex, highIndex, isHardened}) {}

    explicit KeyPathComponent(bool isHardened)
        : m_type(Type::ChildWildcard), m_childWildcardComponent({isHardened}) {}

    KeyPathComponent(uint32_t externalAddressIndex, bool externalAddressIsHardened, uint32_t internalAddressIndex, bool internalAddressIsHardened)
        : m_type(Type::ChildPair), m_childPairComponent({externalAddressIndex, externalAddressIsHardened, internalAddressIndex, internalAddressIsHardened}) {}

    void toMap(CborEncoder *parentEncoder) const;
    void fromMap(CborValue *map);

    Type getType() const { return m_type; }
    void setType(const Type type) { m_type = type; }

    uint32_t getChildIndexValue() const { return m_childIndexComponent->childIndex; }
    bool getChildIndexIsHardened() const { return m_childIndexComponent->isHardened; }
    void setChildIndexValue(const uint32_t childIndex) { m_childIndexComponent->childIndex = childIndex; }
    void setChildIndexHardened(const bool isHardened) { m_childIndexComponent->isHardened = isHardened; }

    uint32_t getLowIndexValue() const { return m_childRangeComponent->lowIndex; }
    uint32_t getHighIndexValue() const { return m_childRangeComponent->highIndex; }
    bool getChildRangeIsHardened() const { return m_childRangeComponent->isHardened; }
    void setLowIndexValue(const uint32_t lowIndex) { m_childRangeComponent->lowIndex = lowIndex; }
    void setHighIndexValue(const uint32_t highIndex) { m_childRangeComponent->highIndex = highIndex; }
    void setRangeHardened(const bool isHardened) { m_childRangeComponent->isHardened = isHardened; }

    bool getChildWildcardIsHardened() const { return m_childWildcardComponent->isHardened; }
    void setChildWildcardHardened(const bool isHardened) { m_childWildcardComponent->isHardened = isHardened; }

    uint32_t getExternalAddressIndexValue() const { return m_childPairComponent->externalAddressIndex; }
    bool getExternalIndexIsHardened() const { return m_childPairComponent->externalAddressIsHardened; }
    uint32_t getInternalAddressIndexValue() const { return m_childPairComponent->internalAddressIndex; }
    bool getInternalIndexIsHardened() const { return m_childPairComponent->internalAddressIsHardened; }
    void setExternalIndexValue(const uint32_t externalAddressIndex) { m_childPairComponent->externalAddressIndex = externalAddressIndex; }
    void setExternalIndexHardened(const bool externalAddressIsHardened) { m_childPairComponent->externalAddressIsHardened = externalAddressIsHardened; }
    void setInternalIndexValue(const uint32_t internalAddressIndex) { m_childPairComponent->internalAddressIndex = internalAddressIndex; }
    void setInternalIndexHardened(const bool internalAddressIsHardened) { m_childPairComponent->internalAddressIsHardened = internalAddressIsHardened; }

private:
    Type m_type;
    std::optional<ChildIndexComponent> m_childIndexComponent;
    std::optional<ChildRangeComponent> m_childRangeComponent;
    std::optional<ChildWildcardComponent> m_childWildcardComponent;
    std::optional<ChildPairComponent> m_childPairComponent;
};
