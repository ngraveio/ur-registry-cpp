#pragma once

#include <tinycbor/cbor.h>
#include <string>

typedef std::string UrType;

class RegistryType {

public:
    RegistryType() = default;
    ~RegistryType() {};
    RegistryType(const UrType& type, CborTag tag): m_type(type), m_tag(tag) {};

    UrType type() const {return m_type;}
    CborTag tag() const {return m_tag;}

private:
    UrType           m_type;
    CborTag          m_tag;
};

/**
 * @brief IANA CBOR Tags Registry
 * Source: https://www.iana.org/assignments/cbor-tags/cbor-tags.xhtml
 *
 */
inline const RegistryType UUID("uuid", 37);
inline const RegistryType HEX_STRING("hex-string", 263);

/**
 * @brief BC UR registry types
 * Source: https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-2020-006-urtypes.md
 *
 */
inline const RegistryType CRYPTO_HDKEY("crypto-hdkey", 303); // Legacy version
inline const RegistryType HDKEY("hdkey", 40303);
inline const RegistryType CRYPTO_KEYPATH("crypto-keypath", 304); // Legacy version
inline const RegistryType KEYPATH("keypath", 40304);
inline const RegistryType CRYPTO_COIN_INFO("crypto-coin-info", 305); // Legacy version
inline const RegistryType COIN_INFO("coin-info", 40305);
inline const RegistryType CRYPTO_ECKEY("crypto-eckey", 306);
inline const RegistryType CRYPTO_OUTPUT("crypto-output", 308); // Legacy version
inline const RegistryType OUTPUT_DESCRIPTOR("output-descriptor", 40308);
inline const RegistryType CRYPTO_PSBT("crypto-psbt", 310);
inline const RegistryType CRYPTO_ACCOUNT("crypto-account", 311);
inline const RegistryType CRYPTO_MULTI_ACCOUNTS("crypto-multi-accounts", 1103);

/**
 * @brief EIP-4517
 * Source: https://eips.ethereum.org/EIPS/eip-4527
 */
inline const RegistryType ETH_SIGN_REQUEST("eth-sign-request", 401);
inline const RegistryType ETH_SIGNATURE("eth-signature", 402);

/**
 * @brief Keystone registry
 * Source: https://github.com/KeystoneHQ/Keystone-developer-hub/blob/main/research/solana-qr-data-protocol.md#sending-the-unsigned-data-from-wallet-only-wallet-to-offline-signer
 */
inline const RegistryType SOL_SIGN_REQUEST("sol-sign-request", 1101);
inline const RegistryType SOL_SIGNATURE("sol-signature", 1102);
inline const RegistryType COSMOS_SIGNATURE("cosmos-signature", 4102);

/**
 * @brief NGRAVE registry
 * Source: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2023-002-multi-layer-sync.md
 * Source: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2023-003-sign.md
 */
inline const RegistryType DETAILED_ACCOUNT("detailed-account", 41402);
inline const RegistryType PORTFOLIO_METADATA("portfolio-metadata", 41404);
inline const RegistryType COIN_IDENTITY("coin-identity", 41401);
inline const RegistryType PORTFOLIO_COIN("portfolio-coin", 41403);
inline const RegistryType PORTFOLIO("portfolio", 41405);
inline const RegistryType SIGN_RESPONSE("sign-response", 41412);
inline const RegistryType SIGN_REQUEST("sign-request", 41411);
inline const RegistryType INTENT("intent", 41415);
inline const RegistryType BATCH_SIGN_REQUEST("batch-sign-request", 41413);
inline const RegistryType BATCH_SIGN_RESPONE("batch-sign-response", 41414);

