#include "coininfo.h"

CoinInfo::CoinInfo()
{
    setRegistryType(COIN_INFO);
    setLegacyRegistryType(CRYPTO_COIN_INFO);
    setUseLegacyType(false);
}

CoinInfo::CoinInfo(const CoinType type, const NetworkType network) : CoinInfo()
{
    this->setType(type);
    this->setNetwork(network);
}

CoinInfo::CoinInfo(const CoinType type) : CoinInfo()
{
    this->setType(type);
}

CoinInfo::CoinInfo(const NetworkType network) : CoinInfo()
{
    this->setNetwork(network);
}

size_t CoinInfo::getMapSize() const
{
    size_t mapSize = MIN_MAP_LENGTH;
    if (m_type.has_value())
        ++mapSize;
    if (m_network.has_value())
        ++mapSize;
    return mapSize;
}

void CoinInfo::toMap(CborEncoder *parentEncoder) const
{
    constexpr uint32_t MASK_TYPE_UINT31 = 0x80000000;

    CborError err = CborNoError;
    CborEncoder container;
    size_t mapSize = getMapSize();

    err = cbor_encoder_create_map(parentEncoder, &container, mapSize);
    checkCborError(err, "Failed to create map");

    if (m_type.has_value())
    {
        if (static_cast<uint32_t>(m_type.value()) & MASK_TYPE_UINT31)
        {
            throw CborException("Last bit of type should be turned off", CborErrorImproperValue);
        }
        err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::TYPE));
        checkCborError(err, "Failed to encode uint");
        err = cbor_encode_uint(&container, static_cast<uint32_t>(m_type.value()));
        checkCborError(err, "Failed to encode uint");
    }

    if (m_network.has_value())
    {
        err = cbor_encode_uint(&container, static_cast<uint8_t>(Key::NETWORK));
        checkCborError(err, "Failed to encode uint");
        err = cbor_encode_int(&container, static_cast<int16_t>(m_network.value()));
        checkCborError(err, "Failed to encode int");
    }

    err = cbor_encoder_close_container(parentEncoder, &container);
    checkCborError(err, "Failed to close container");
}