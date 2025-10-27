#pragma once

#include <iostream>

#include "registry_item.h"
#include "output_descriptor.h"

/**
 * @brief  Implementation of the detailed-account(CBOR tag #6.41402) UR type
 * Reference: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2023-002-multi-layer-sync.md
 *
 * The following specification of detailed-account is written in CDDL. When used embedded in another CBOR structure, this structure should be tagged #6.41402.
 *
 * account_exp = #6.40303(hdkey) / #6.40308(output-descriptor)
 *
 * ; Accounts are specified using either '#6.40303(hdkey)' or
 * ; '#6.40308(output-descriptor)'.
 * ; By default, '#6.40303(hdkey)' should be used to share public keys and
 * ; extended public keys.
 * ; '#6.308(output-descriptor)' should be used to share an output descriptor,
 * ; e.g. for the different Bitcoin address formats (P2PKH, P2SH-P2WPKH, P2WPKH, P2TR).

 * ; Optional 'token-ids' to indicate the synchronization of a list of tokens with
 * ; the associated accounts
 * ; 'token-id' is defined differently depending on the blockchain:
 * ; - ERC20 tokens on EVM chains are identified by their contract addresses
 * ; (e.g. `0xA0b86991c6218b36c1d19D4a2e9Eb0cE3606eB48`)
 * ; - ERC1155 tokens are identifed with their contract addresses followed by their
 * ; ID with ':' as separator (e.g. `0xfaafdc07907ff5120a76b34b731b278c38d6043c:
 * ; 508851954656174721695133294256171964208`)
 * ; - ESDT tokens on MultiversX are by their name followed by their ID with `-` as
 * ; separator (e.g. `USDC-c76f1f`)
 * ; - SPL tokens on Solana are identified by their contract addresses
 * ; (e.g. `EPjFWdd5AufqSSqeM2qN1xzybapC8G4wEGGkZwyTDt1v`)
 *
 * detailed-account = {
 *  account: account_exp,
 *  ? token-ids: [+ string]  * ; Specify multiple tokens associated to one account
 * }
 *
 * account = 1
 * token-ids = 2
 */

class DetailedAccount : public RegistryItem
{
public:
    DetailedAccount();
    explicit DetailedAccount(const std::variant<std::monostate, HDKey, OutputDescriptor> &account, const std::vector<std::string> &tokenids = {});
    DetailedAccount(const std::string &uai, const std::string &key);

    ~DetailedAccount() override = default;

    enum class Key
    {
        ACCOUNT = 1,
        TOKENIDS,
    };

    size_t getMinMapLength() const override { return MIN_MAP_LENGTH; }
    size_t getMaxMapLength() const override { return MAX_MAP_LENGTH; }
    size_t getMapSize() const override;

    void toMap(CborEncoder *parentEncoder) const override;
    void fromMap(CborValue *) override
    {
        throw CborException("Decoder not supported", CborErrorUnimplementedValidation);
    }

    std::variant<std::monostate, HDKey, OutputDescriptor> getAccount() const { return m_account; };
    void setAccount(const std::variant<std::monostate, HDKey, OutputDescriptor> &account) { m_account = account; };

    std::vector<std::string> getTokenids() const { return m_tokenids; };
    void setTokenids(const std::vector<std::string> &tokenids) { m_tokenids = tokenids; };
    void addTokenId(const std::string &tokenid) { m_tokenids.push_back(tokenid); };
    bool isTokenAdded(const std::string &tokenid) const { return (std::find(m_tokenids.begin(), m_tokenids.end(), tokenid) != m_tokenids.end()); };

    bool operator==(const DetailedAccount &detailedAccount) const
    {
        return (m_account == detailedAccount.getAccount());
    };

    /**
     * @brief Encode UAI string and xpub to detailed account
     *
     * @param uai is the Unique Asset ID format as defined in NBCR-2024-01
     * @param key is an extended public key or a public key
     *
     * Ref: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2024-001-unique-asset-id.md
     */
    void setDetailedAccount(const std::string &uai, const std::string &key);

private:
    static constexpr size_t MIN_MAP_LENGTH = 1;
    static constexpr size_t MAX_MAP_LENGTH = 2;

    std::variant<std::monostate, HDKey, OutputDescriptor> m_account;
    std::vector<std::string> m_tokenids;
};
