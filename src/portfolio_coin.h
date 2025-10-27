#pragma once

#include <iostream>
#include <optional>
#include <vector>
#include <variant>

#include "registry_item.h"
#include "coin_identity.h"
#include "detailed_account.h"

/**
 * @brief Implementation of the portfolio-coin (CBOR tag #6.41403) UR type
 *
 * Source: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2023-002-multi-layer-sync.md
 * CDDL specification:
 *
 * ; Associate a coin identity to its accounts
 *
 * detailed_accounts = [+ #6.41402(detailed-account)]
 *
 * ; The accounts are listed using #6.41402(detailed-account) to share the maximum of information related to the accounts
 *
 * coin = {
 *   coin-id: #6.41401(coin-identity),
 *   accounts: accounts_exp,
 *   ? master-fingerprint: uint32 ; Master fingerprint (fingerprint for the master public key as per BIP32)
 * }
 *
 * ; master-fingerprint must match the potential other fingerprints included in the other sub-UR types
 *
 * coin-id = 1
 * accounts = 2
 * master-fingerprint = 3
 */

class PortfolioCoin : public RegistryItem
{
public:
    PortfolioCoin();
    ~PortfolioCoin() override = default;
    explicit PortfolioCoin(const CoinIdentity &coin_id,
                           const std::vector<DetailedAccount> &accounts,
                           const std::optional<uint32_t> &master_fingerprint = std::nullopt);

    PortfolioCoin(const std::string &uai, const std::string &key);

    void clear();

    enum class Key
    {
        COIN_ID = 1,
        ACCOUNTS,
        MASTER_FINGERPRINT
    };

    size_t getMinMapLength() const override
    {
        return MIN_MAP_LENGTH;
    }

    size_t getMaxMapLength() const override
    {
        return MAX_MAP_LENGTH;
    }

    CoinIdentity getCoinId() const { return m_coin_id; }
    std::vector<DetailedAccount> getAccounts() const { return m_accounts; }
    std::optional<uint32_t> getMasterFingerprint() const { return m_master_fingerprint; }

    void setCoinId(const CoinIdentity &coin_id) { m_coin_id = coin_id; }
    void setAccounts(const std::vector<DetailedAccount> &accounts) { m_accounts = accounts; }
    void addAccount(const DetailedAccount &account) { m_accounts.push_back(account); };
    std::vector<DetailedAccount>::const_iterator findAccount(const DetailedAccount &account) const { return std::find(m_accounts.begin(), m_accounts.end(), account); };
    void setMasterFingerprint(const uint32_t master_fingerprint) { m_master_fingerprint = master_fingerprint; }

    bool operator==(const PortfolioCoin &coin) const
    {
        // Verify first if the master fingerprint is identical
        auto coin_masterFingerprint = coin.getMasterFingerprint();
        if (m_master_fingerprint.has_value() && coin_masterFingerprint.has_value())
        {
            if (*coin_masterFingerprint != *m_master_fingerprint)
            {
                return false;
            }
        }
        return (m_coin_id == coin.getCoinId());
    };

    size_t getMapSize() const override;
    void toMap(CborEncoder *parentEncoder) const override;
    void fromMap(CborValue *) override
    {
        throw CborException("Decoder not supported", CborErrorUnimplementedValidation);
    }

    /**
     * @brief Create a Portfolio Coin from the UAI and the public key
     *
     * @param uai is the Unique Asset ID format as defined in NBCR-2024-01
     * @param key is the public key
     *
     * Source: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2024-001-unique-asset-id.md
     */
    void createPortfolioCoin(const std::string &uai, const std::string &key);

    /**
     * @brief Add an account to the accounts array based on the UAI and the public key
     *
     * @param uai: - should indicate the same coin identity and the same master fingerprint (if specified)
     *             - can contain a new token to add to an existing account
     * @param key specifies the key of the added account
     */
    void addAccount(const std::string &uai, const std::string &key);

private:
    static constexpr size_t MIN_MAP_LENGTH = 2;
    static constexpr size_t MAX_MAP_LENGTH = 3;

    CoinIdentity m_coin_id;
    std::vector<DetailedAccount> m_accounts;
    std::optional<uint32_t> m_master_fingerprint;
};