#pragma once

#include "request_base.h"
#include "coin_identity.h"
#include "intent.h"

using BytesVector = std::vector<uint8_t>;
using StringOrBytesVariant = std::variant<std::monostate, std::string, BytesVector>;
/**
 * @brief:  Implementation of the sign-request UR type decoder (CBOR tag #6.41411)
 *
 * Ref.: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2023-003-sign.md
 *
 *
 * sign-request = {
 *     ?request-id: uuid,                            ; Unique identifier for the signing request
 *     coin-id: #6.41401(coin-identity),             ; Defines elliptic curve and blockchain context
 *     ?derivation-path: derivation-path-selection,  ; Key path(s) for signature derivation
 *     sign-data: bytes,                             ; Serialized transaction data
 *     ?origin: text,                                ; Origin of the request (e.g., wallet identifier)
 *     ?tx-type: int .default 1,                     ; Blockchain-specific transaction type
 *     ?address: address-selection                   ; Address(es) associated to the signer's account (must correspond to the derived address from the derivation path)
 *     ?intent: #6.41415(intent)                ; Metadata instructing signer to apply intent logic before signing
 * }
 *
 * ; If both derivation paths and addresses are provided as arrays, they must contain the same number of elements.
 * ; When multiple elements are present, the offline signer must select one account for signing.
 *
 * derivation-path-selection = keypath-type / [2* keypath-type]
 * address-selection = address-type / [2* address-type]
 *
 * keypath-type = #6.40304(keypath)
 * address-type = string / bytes
 *
 * request-id = 1
 * coin-id = 2
 * derivation-path = 3
 * sign-data = 4
 * origin = 5
 * tx-type = 6
 * address = 7
 * intent = 8
 *
 */

class SignRequest : public RequestBase
{
public:
    SignRequest();
    explicit SignRequest(const CoinIdentity &coinId, const BytesVector &sign_data);
    explicit SignRequest(const Uuid &requestId, const CoinIdentity &coinId, const Keypath &derivationPath, const BytesVector &sign_data, const std::string &origin, const StringOrBytesVariant &address, const Intent &intent = {});

    ~SignRequest() override = default;

    enum class Key
    {
        REQUEST_ID = 1,
        COIN_IDENTITY,
        DERIVATION_PATH,
        SIGN_DATA,
        ORIGIN,
        TXTYPE,
        ADDRESS,
        INTENT
    };

    size_t getMinMapLength() const override { return MIN_MAP_LENGTH; }
    size_t getMaxMapLength() const override { return MAX_MAP_LENGTH; }

    size_t getMapSize() const override;

    CoinIdentity getCoinIdentity() const { return m_coin_identity; }
    void setCoinIdentity(const CoinIdentity &coin_identity) { m_coin_identity = coin_identity; }

    std::optional<uint> getTxtype() const { return m_txtype; }
    void setTxtype(const std::optional<uint> &txtype) { m_txtype = txtype; }

    std::variant<std::monostate, std::string, std::vector<uint8_t>> getAddress() const { return m_address; }
    void setAddress(const std::variant<std::monostate, std::string, std::vector<uint8_t>> &address) { m_address = address; }

    std::string getAddressStr() const override;

    std::optional<Intent> getIntent() const { return m_intent; }
    void setIntent(const std::optional<Intent> &value) { m_intent = value; }

    void toMap(CborEncoder *) const override
    {
        throw CborException("Encoder not supported", CborErrorUnimplementedValidation);
    }
    void fromMap(CborValue *cborMap) override;
    void clearAllItems();

private:
    static constexpr size_t MIN_MAP_LENGTH = 1;
    static constexpr size_t MAX_MAP_LENGTH = 8;

    CoinIdentity m_coin_identity;
    std::optional<uint> m_txtype;
    std::variant<std::monostate, std::string, std::vector<uint8_t>> m_address;
    std::optional<Intent> m_intent;
};
