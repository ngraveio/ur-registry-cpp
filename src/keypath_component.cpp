#include "keypath_component.h"

constexpr size_t WILDCARD_ARRAY_SIZE = 0;
constexpr size_t CHILDRANGE_ARRAY_SIZE = 2;
constexpr size_t CHILDPAIR_ARRAY_SIZE = 4;

KeyPathComponent::KeyPathComponent()
{
}

void KeyPathComponent::toMap(CborEncoder *parentEncoder) const
{
    CborError err = CborNoError;

    switch (m_type)
    {
    case Type::ChildIndex:
    {
        checkMaxUint31(m_childIndexComponent->childIndex);

        err = cbor_encode_uint(parentEncoder, m_childIndexComponent->childIndex);
        checkCborError(err, "Failed to encode uint");

        err = cbor_encode_boolean(parentEncoder, m_childIndexComponent->isHardened);
        checkCborError(err, "Failed to encode boolean");
        break;
    }

    case Type::ChildRange:
    {
        checkMaxUint31(m_childRangeComponent->lowIndex);
        checkMaxUint31(m_childRangeComponent->highIndex);
        if (m_childRangeComponent->lowIndex >= m_childRangeComponent->highIndex)
        {
            throw CborException("Low index superior than high index", CborErrorImproperValue);
        }

        CborEncoder rangeEncoder;
        err = cbor_encoder_create_array(parentEncoder, &rangeEncoder, CHILDRANGE_ARRAY_SIZE);
        checkCborError(err, "Failed to create array");

        err = cbor_encode_uint(&rangeEncoder, m_childRangeComponent->lowIndex);
        checkCborError(err, "Failed to encode uint");

        err = cbor_encode_uint(&rangeEncoder, m_childRangeComponent->highIndex);
        checkCborError(err, "Failed to encode uint");

        err = cbor_encoder_close_container(parentEncoder, &rangeEncoder);
        checkCborError(err, "Failed to close container");

        err = cbor_encode_boolean(parentEncoder, m_childRangeComponent->isHardened);
        checkCborError(err, "Failed to encode boolean");
        break;
    }

    case Type::ChildWildcard:
    {
        CborEncoder emptyArrayEncoder;
        err = cbor_encoder_create_array(parentEncoder, &emptyArrayEncoder, WILDCARD_ARRAY_SIZE);
        checkCborError(err, "Failed to create array");

        err = cbor_encoder_close_container(parentEncoder, &emptyArrayEncoder);
        checkCborError(err, "Failed to close container");

        err = cbor_encode_boolean(parentEncoder, m_childWildcardComponent->isHardened);
        checkCborError(err, "Failed to encode boolean");
        break;
    }

    case Type::ChildPair:
    {
        checkMaxUint31(m_childPairComponent->externalAddressIndex);
        checkMaxUint31(m_childPairComponent->internalAddressIndex);

        CborEncoder rangeEncoder;
        err = cbor_encoder_create_array(parentEncoder, &rangeEncoder, CHILDPAIR_ARRAY_SIZE);
        checkCborError(err, "Failed to create array");

        err = cbor_encode_uint(&rangeEncoder, m_childPairComponent->externalAddressIndex);
        checkCborError(err, "Failed to encode uint");

        err = cbor_encode_boolean(&rangeEncoder, m_childPairComponent->externalAddressIsHardened);
        checkCborError(err, "Failed to encode boolean");

        err = cbor_encode_uint(&rangeEncoder, m_childPairComponent->internalAddressIndex);
        checkCborError(err, "Failed to encode uint");

        err = cbor_encode_boolean(&rangeEncoder, m_childPairComponent->internalAddressIsHardened);
        checkCborError(err, "Failed to encode boolean");

        err = cbor_encoder_close_container(parentEncoder, &rangeEncoder);
        checkCborError(err, "Failed to close container");
        break;
    }

    default:
        throw CborException("Unknown KeyPathComponent type", CborErrorExcludedType);
    }
}

void KeyPathComponent::fromMap(CborValue *map)
{
    CborError err = CborNoError;

    if (cbor_value_is_unsigned_integer(map))
    {
        // ChildIndex type starts with unsigned, followed by a boolean
        uint64_t childIndex;
        bool isHardened;

        err = cbor_value_get_uint64(map, &childIndex);
        checkCborError(err, "Failed to get uint");
        checkMaxUint31(childIndex);
        err = cbor_value_advance(map);
        checkCborError(err, "Failed to advance");

        checkIsBool(map);
        err = cbor_value_get_boolean(map, &isHardened);
        checkCborError(err, "Failed to get boolean");

        *this = KeyPathComponent(static_cast<uint32_t>(childIndex), isHardened);
    }
    else
    {
        // ChildRange, ChildWildcard and ChildPair types start with an array
        checkIsArray(map);
        CborValue array;
        size_t length = 0;

        err = cbor_value_get_array_length(map, &length);
        checkCborError(err, "Failed to get array length");

        switch (length)
        {
        case CHILDRANGE_ARRAY_SIZE:
        {
            // ChildRange type is an array of 2 elements, followed by a boolean
            uint64_t lowIndex;
            uint64_t highIndex;
            bool isHardened;

            err = cbor_value_enter_container(map, &array);
            checkCborError(err, "Failed to enter container");

            checkIsUint(&array);
            err = cbor_value_get_uint64(&array, &lowIndex);
            checkCborError(err, "Failed to get uint");
            checkMaxUint31(lowIndex);

            err = cbor_value_advance(&array);
            checkCborError(err, "Failed to advance");

            checkIsUint(&array);
            err = cbor_value_get_uint64(&array, &highIndex);
            checkCborError(err, "Failed to get uint");
            checkMaxUint31(highIndex);

            err = cbor_value_advance(&array);
            checkCborError(err, "Failed to advance");

            err = cbor_value_leave_container(map, &array);
            checkCborError(err, "Failed to leave container");

            checkIsBool(map);
            err = cbor_value_get_boolean(map, &isHardened);
            checkCborError(err, "Failed to get boolean");

            *this = KeyPathComponent(static_cast<uint32_t>(lowIndex), static_cast<uint32_t>(highIndex), isHardened);

            break;
        }
        case WILDCARD_ARRAY_SIZE:
        {
            // ChildWildcard type is an empty array, followed by a boolean
            bool isHardened;

            err = cbor_value_advance(map);
            checkCborError(err, "Failed to advance");

            checkIsBool(map);
            err = cbor_value_get_boolean(map, &isHardened);
            checkCborError(err, "Failed to get boolean");

            *this = KeyPathComponent(isHardened);

            break;
        }
        case CHILDPAIR_ARRAY_SIZE:
        {
            // ChildPair type is an array of 4 elements
            uint64_t externalAddressIndex;
            bool externalAddressIsHardened;
            uint64_t internalAddressIndex;
            bool internalAddressIsHardened;

            err = cbor_value_enter_container(map, &array);
            checkCborError(err, "Failed to enter container");

            checkIsUint(&array);
            err = cbor_value_get_uint64(&array, &externalAddressIndex);
            checkCborError(err, "Failed to get uint");
            checkMaxUint31(externalAddressIndex);

            err = cbor_value_advance(&array);
            checkCborError(err, "Failed to advance");

            checkIsBool(&array);
            err = cbor_value_get_boolean(&array, &externalAddressIsHardened);
            checkCborError(err, "Failed to get boolean");

            err = cbor_value_advance(&array);
            checkCborError(err, "Failed to advance");

            checkIsUint(&array);
            err = cbor_value_get_uint64(&array, &internalAddressIndex);
            checkCborError(err, "Failed to get uint");
            checkMaxUint31(internalAddressIndex);

            err = cbor_value_advance(&array);
            checkCborError(err, "Failed to advance");

            checkIsBool(&array);
            err = cbor_value_get_boolean(&array, &internalAddressIsHardened);
            checkCborError(err, "Failed to get boolean");

            err = cbor_value_advance(&array);
            checkCborError(err, "Failed to advance");

            err = cbor_value_leave_container(map, &array);
            checkCborError(err, "Failed to leave container");

            *this = KeyPathComponent(static_cast<uint32_t>(externalAddressIndex), externalAddressIsHardened, static_cast<uint32_t>(internalAddressIndex), internalAddressIsHardened);

            break;
        }
        default:
        {
            throw CborException("Unsupported KeyPathComponent type", CborErrorIllegalType);
        }
        }
    }
}
