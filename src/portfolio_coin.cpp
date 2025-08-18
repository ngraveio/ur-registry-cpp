#include "portfolio_coin.h"
#include "cbor_exception.h"
#include "uai.h"

PortfolioCoin::PortfolioCoin() {
    setRegistryType(PORTFOLIO_COIN);
}

PortfolioCoin::PortfolioCoin(const CoinIdentity& coin_id,
                           const std::vector<DetailedAccount>& accounts,
                           const std::optional<uint32_t>& master_fingerprint) : PortfolioCoin::PortfolioCoin() {
    setCoinId(coin_id);
    setAccounts(accounts);
    if (master_fingerprint.has_value()) {
        setMasterFingerprint(master_fingerprint.value());
    }
}

PortfolioCoin::PortfolioCoin(const std::string& uai, const std::string& key) : PortfolioCoin::PortfolioCoin() {
    createPortfolioCoin(uai, key);
}

void PortfolioCoin::clear() {
    m_coin_id.clear();
    m_accounts.clear();
    m_master_fingerprint = std::nullopt;
}

size_t PortfolioCoin::getMapSize() const {
    size_t size = min_map_length; // coin_id and accounts are mandatory
    if (m_master_fingerprint.has_value()) {
        size++;
    }
    return size;
}

void PortfolioCoin::toMap(CborEncoder* parentEncoder) const {
    CborEncoder mapEncoder;
    size_t size = getMapSize();
    CborError err = cbor_encoder_create_map(parentEncoder, &mapEncoder, size);
    checkCborError(err, "Failed to create map");

    // Encode coin_id
    err = cbor_encode_uint(&mapEncoder, static_cast<uint8_t>(Key::COIN_ID));
    checkCborError(err, "Failed to encode coin_id key");

    // Tag the coin identity with its registry type
    err = cbor_encode_tag(&mapEncoder, COIN_IDENTITY.tag());
    checkCborError(err, "Failed to encode coin identity tag");

    // Encode the coin identity
    m_coin_id.toMap(&mapEncoder);

    // Encode accounts
    err = cbor_encode_uint(&mapEncoder, static_cast<uint8_t>(Key::ACCOUNTS));
    checkCborError(err, "Failed to encode accounts key");

    // Create array for accounts
    CborEncoder accountsArray;
    err = cbor_encoder_create_array(&mapEncoder, &accountsArray, m_accounts.size());
    checkCborError(err, "Failed to create accounts array");

    // Encode each detailed account
    for (const auto& account : m_accounts) {
        // Tag each detailed account with its registry type
        err = cbor_encode_tag(&accountsArray, DETAILED_ACCOUNT.tag());
        checkCborError(err, "Failed to encode detailed account tag");

        // Encode the detailed account
        account.toMap(&accountsArray);
    }

    err = cbor_encoder_close_container(&mapEncoder, &accountsArray);
    checkCborError(err, "Failed to close accounts array");

    // Encode master_fingerprint if present
    if (m_master_fingerprint.has_value()) {
        err = cbor_encode_uint(&mapEncoder, static_cast<uint8_t>(Key::MASTER_FINGERPRINT));
        checkCborError(err, "Failed to encode master_fingerprint key");
        err = cbor_encode_uint(&mapEncoder, m_master_fingerprint.value());
        checkCborError(err, "Failed to encode master_fingerprint value");
    }

    err = cbor_encoder_close_container(parentEncoder, &mapEncoder);
    checkCborError(err, "Failed to close container");
}

void PortfolioCoin::createPortfolioCoin(const std::string& uai, const std::string& key) {
    clear();
    CoinIdentity coin_id(uai);
    setCoinId(coin_id);
    DetailedAccount account(uai, key);
    addAccount(account);
    UniqueAssetId UaiObject(uai);
    auto masterFingerprint = UaiObject.getMasterFingerprintValue();
    if (masterFingerprint != 0) setMasterFingerprint(masterFingerprint);
}

void PortfolioCoin::addAccount(const std::string& uai, const std::string& key) {
    UniqueAssetId UaiObject(uai);
    
    auto masterFingerprint = UaiObject.getMasterFingerprintValue();
    if (masterFingerprint != 0) {
        if (!m_master_fingerprint.has_value()) {
            setMasterFingerprint(masterFingerprint);
        } else {
            if (masterFingerprint != m_master_fingerprint){
                throw CborException("Master Fingerprint differs to add a new account to this portfolio coin", CborErrorImproperValue);
            }
        }
    }

    CoinIdentity uai_coin(uai);
    if (!(uai_coin == m_coin_id)){
        throw CborException("Coin identity differs to add a new account to this portfolio coin", CborErrorImproperValue);
    }

    DetailedAccount uai_account(uai, key);
    auto it_account = findAccount(uai_account);

    if (it_account != m_accounts.end()) {
        auto found_account = *it_account;
        if (UaiObject.isToken()) {
            // Add token ID if not already added
            auto uai_token = UaiObject.getTokenId();
            if (!found_account.isTokenAdded(uai_token)) {
                found_account.addTokenId(uai_token);
                std::replace(m_accounts.begin(), m_accounts.end(), *it_account, found_account);
            }
        }
        // Nothing to add
    } else {
        addAccount(uai_account);
    }
}
