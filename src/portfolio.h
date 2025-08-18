#pragma once

#include <iostream>
#include <optional>
#include <vector>

#include "registry_item.h"
#include "portfolio_coin.h"
#include "portfolio_metadata.h"

/**
 * @brief Implementation of the portfolio (CBOR tag #6.41405) UR type
 *
 * Source: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2023-002-multi-layer-sync.md
 * CDDL specification:
 *
 * sync = {
 *     coins: [+ #6.41403(portfolio-coin)],           ; Multiple coins with their respective accounts and coin identities
 *     ? metadata: #6.41404(portfolio-metadata) ; Optional wallet metadata
 * }
 *
 * coins = 1
 * metadata = 2
 */

class Portfolio : public RegistryItem {
public:
    Portfolio();
    ~Portfolio() override = default;
    explicit Portfolio(const std::vector<PortfolioCoin>& coins,
                      const std::optional<PortfolioMetadata>& metadata = std::nullopt);
    Portfolio(const std::string& uai, const std::string& key);

    void clear();

    enum class Key {
        COINS = 1,
        METADATA
    };

    static constexpr size_t min_map_length = 1;
    static constexpr size_t max_map_length = 2;

    size_t getMinMapLength() const override {
        return min_map_length;
    }

    size_t getMaxMapLength() const override {
        return max_map_length;
    }

    std::optional<PortfolioMetadata> getMetadata() const { return m_metadata; }
    void setMetadata(const PortfolioMetadata& metadata) { m_metadata = metadata; }

    std::vector<PortfolioCoin> getCoins() const { return m_coins; }
    void setCoins(const std::vector<PortfolioCoin>& coins) { m_coins = coins; }
    void addCoin(const PortfolioCoin& coin) { m_coins.push_back(coin); }
    std::vector<PortfolioCoin>::const_iterator findCoin(const PortfolioCoin& coin) const { return std::find(m_coins.begin(), m_coins.end(), coin); };

    size_t getMapSize() const override;
    void toMap(CborEncoder* parentEncoder) const override;
    void fromMap(CborValue*) override {
        throw CborException("Decoder not supported", CborErrorUnimplementedValidation);
    }

    /**
     * @brief Create a Portfolio from the UAI and the public key
     * 
     * @param uai is the Unique Asset ID format as defined in NBCR-2024-01
     * @param key is the public key 
     * 
     * Source: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2024-001-unique-asset-id.md
     */
    void createPortfolio(const std::string& uai, const std::string& key);

    /**
     * @brief Add a coin to the Portfolio coin array
     * 
     * @param uai is the Unique Asset ID format
     * @param key is the public key 
     */
    void addCoin(const std::string& uai, const std::string& key);

private:
    std::vector<PortfolioCoin> m_coins;
    std::optional<PortfolioMetadata> m_metadata;
};