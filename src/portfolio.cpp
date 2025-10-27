#include "portfolio.h"
#include "cbor_exception.h"

Portfolio::Portfolio()
{
    setRegistryType(PORTFOLIO);
}

Portfolio::Portfolio(const std::vector<PortfolioCoin> &coins,
                     const std::optional<PortfolioMetadata> &metadata) : Portfolio::Portfolio()
{
    setCoins(coins);
    if (metadata.has_value())
    {
        setMetadata(metadata.value());
    }
}

Portfolio::Portfolio(const std::string &uai, const std::string &key) : Portfolio::Portfolio()
{
    createPortfolio(uai, key);
}

void Portfolio::clear()
{
    m_coins.clear();
    m_metadata = std::nullopt;
}

size_t Portfolio::getMapSize() const
{
    size_t size = MIN_MAP_LENGTH; // coins is mandatory
    if (m_metadata.has_value())
    {
        size++;
    }
    return size;
}

void Portfolio::toMap(CborEncoder *parentEncoder) const
{
    CborEncoder mapEncoder;
    size_t size = getMapSize();
    CborError err = cbor_encoder_create_map(parentEncoder, &mapEncoder, size);
    checkCborError(err, "Failed to create map");

    // Encode coins
    err = cbor_encode_uint(&mapEncoder, static_cast<uint8_t>(Key::COINS));
    checkCborError(err, "Failed to encode coins key");

    // Create array for coins
    CborEncoder coinsArray;
    err = cbor_encoder_create_array(&mapEncoder, &coinsArray, m_coins.size());
    checkCborError(err, "Failed to create coins array");

    // Encode each portfolio coin
    for (const auto &coin : m_coins)
    {
        // Tag each portfolio coin with its registry type
        err = cbor_encode_tag(&coinsArray, PORTFOLIO_COIN.tag());
        checkCborError(err, "Failed to encode portfolio coin tag");

        // Encode the portfolio coin
        coin.toMap(&coinsArray);
    }

    err = cbor_encoder_close_container(&mapEncoder, &coinsArray);
    checkCborError(err, "Failed to close coins array");

    // Encode metadata if present
    if (m_metadata.has_value())
    {
        err = cbor_encode_uint(&mapEncoder, static_cast<uint8_t>(Key::METADATA));
        checkCborError(err, "Failed to encode metadata key");

        // Tag the metadata with its registry type
        err = cbor_encode_tag(&mapEncoder, PORTFOLIO_METADATA.tag());
        checkCborError(err, "Failed to encode portfolio metadata tag");

        // Encode the metadata
        m_metadata.value().toMap(&mapEncoder);
    }

    err = cbor_encoder_close_container(parentEncoder, &mapEncoder);
    checkCborError(err, "Failed to close container");
}

void Portfolio::createPortfolio(const std::string &uai, const std::string &key)
{
    m_coins.clear();
    PortfolioCoin uai_coin(uai, key);
    addCoin(uai_coin);
}

void Portfolio::addCoin(const std::string &uai, const std::string &key)
{
    PortfolioCoin uai_coin(uai, key);
    if (m_coins.empty())
    {
        createPortfolio(uai, key);
        return;
    }

    auto it_coin = findCoin(uai_coin);

    if (it_coin != m_coins.end())
    {
        // Add account to existing coin
        auto found_coin = *it_coin;
        found_coin.addAccount(uai, key);
        std::replace(m_coins.begin(), m_coins.end(), *it_coin, found_coin);
    }
    else
    {
        // Add new coin to portfolio
        addCoin(uai_coin);
    }
}
