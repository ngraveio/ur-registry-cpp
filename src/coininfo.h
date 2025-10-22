#pragma once

#include <iostream>

#include "registry_item.h"

/**
 * @brief Implementation of the coin-info (CBOR tag #6.40305) UR type and its legacy version crypto-coin-info (CBOR tag #6.305)
 *
 * Source: https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-2020-007-hdkey.md
 * CDDL specification:
 * ; Metadata for the type and use of a cryptocurrency
 *
 * tagged-coininfo = #6.40305(coininfo)
 *
 * coininfo = {
 *    ? type: uint31 .default cointype-btc, ; values from [SLIP44](https://github.com/satoshilabs/slips/blob/master/slip-0044.md) with high bit turned off
 *    ? network: int .default mainnet ; coin-specific identifier for testnet
 * }
 *
 * type = 1
 * network = 2
 *
 * cointype-btc = 0
 * cointype-eth = 0x3c
 *
 * mainnet = 0;
 * testnet-btc = 1;
 *
 * ; from [ETH-TEST-NETWORKS]
 * testnet-eth-ropsten = 1;
 * testnet-eth-kovan = 2;
 * testnet-eth-rinkeby = 3;
 * testnet-eth-gorli = 4;
 */

enum class CoinType : uint32_t
{
    Bitcoin = 0,
    Ethereum = 60
};

enum class NetworkType : int16_t
{
    MainNet = 0,
    BtcTestNet = 1,
    EthTestNetRopsten = 1,
    EthTestNetKovan = 2,
    EthTestNetRinkeby = 3,
    EthTestNetGorli = 4
};

class CoinInfo : public RegistryItem
{
public:
    CoinInfo();
    ~CoinInfo() override = default;
    explicit CoinInfo(const CoinType type, const NetworkType network);
    explicit CoinInfo(const CoinType type);
    explicit CoinInfo(const NetworkType network);

    enum class Key
    {
        TYPE = 1,
        NETWORK,
    };

    size_t getMinMapLength() const override
    {
        return MIN_MAP_LENGTH;
    }

    size_t getMaxMapLength() const override
    {
        return MAX_MAP_LENGTH;
    }

    std::optional<CoinType> getType() const { return m_type; }
    std::optional<NetworkType> getNetwork() const { return m_network; }
    void setType(const CoinType type) { m_type = type; }
    void setNetwork(const NetworkType network) { m_network = network; }

    size_t getMapSize() const override;
    void toMap(CborEncoder *parentEncoder) const override;
    void fromMap(CborValue *) override
    {
        throw CborException("Decoder not supported", CborErrorUnimplementedValidation);
    }

private:
    static constexpr size_t MIN_MAP_LENGTH = 0;
    static constexpr size_t MAX_MAP_LENGTH = 2;

    std::optional<CoinType> m_type;
    std::optional<NetworkType> m_network;
};
