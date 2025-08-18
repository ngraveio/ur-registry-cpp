#pragma once

#include <cstdint>
#include <string>

#include "coininfo.h"
#include "keypath.h"
#include "hdkey_bip32.h"
#include "registry_item.h"

/**
 * @brief  Implementation of the hdkey (CBOR tag #6.40303) UR type and its legacy version crypto-hdkey (CBOR tag #6.303)
 * 
 * Source: https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-2020-007-hdkey.md
 * CDDL specification:
 * tagged-hdkey = #6.40303(hdkey)
 *
 * ; An HD key is either a master key or a derived key.
 * 
 * hdkey = {
 *     master-key / derived-key
 * }
 * 
 * ; A master key is always private, has no use or derivation information,
 * ; and always includes a chain code.
 * master-key = (
 *     is-master: true,
 *     key-data: key-data-bytes,
 *     chain-code: chain-code-bytes
 * )
 * 
 * ; A derived key may be private or public, has an optional chain code, and
 * ; may carry additional metadata about its use and derivation.
 * ; To maintain isomorphism with [BIP32] and allow keys to be derived from
 * ; this key `chain-code`, `origin`, and `parent-fingerprint` must be present.
 * ; If `origin` contains only a single derivation step and also contains `source-fingerprint`,
 * ; then `parent-fingerprint` MUST be identical to `source-fingerprint` or may be omitted.
 * derived-key = (
 *     ? is-private: bool .default false,     ; true if key is private, false if public
 *     key-data: key-data-bytes,
 *     ? chain-code: chain-code-bytes         ; omit if no further keys may be derived from this key
 *     ? use-info: tagged-coininfo, ; How the key is to be used
 *     ? origin: tagged-keypath,    ; How the key was derived
 *     ? children: tagged-keypath,  ; What children should/can be derived from this
 *     ? parent-fingerprint: uint32 .ne 0,    ; The fingerprint of this key's direct ancestor, per [BIP32]
 *     ? name: text,                          ; A short name for this key.
 *     ? note: text                           ; An arbitrary amount of text describing the key.
 * )
 * 
 * ; If the `use-info` field is omitted, defaults (mainnet BTC key) are assumed.
 * ; If `cointype` and `origin` are both present, then per [BIP44], the second path
 * ; component's `child-index` must match `cointype`.
 * 
 * ; The `children` field may be used to specify what set of child keys should or can be derived from this key. This may include `child-index-range` or `child-index-wildcard` as its last component. Any components that specify hardened derivation will require the key be private.
 * 
 * is-master = 1
 * is-private = 2
 * key-data = 3
 * chain-code = 4
 * use-info = 5
 * origin = 6
 * children = 7
 * parent-fingerprint = 8
 * name = 9
 * note = 10
 * 
 * uint8 = uint .size 1
 * key-data-bytes = bytes .size 33
 * chain-code-bytes = bytes .size 32
 */

class HDKey : public RegistryItem
{
public:
    HDKey();
    ~HDKey() override = default;

    static constexpr size_t MIN_MAP_LENGTH = 1;
    static constexpr size_t MAX_MAP_LENGTH = 10;

    enum class Key {
      IS_MASTER = 1,
      IS_PRIVATE,
      KEY_DATA,
      CHAIN_CODE,
      USE_INFO,
      ORIGIN,
      CHILDREN,
      PARENT_FINGERPRINT,
      NAME,
      NOTE,
    };

    size_t getMinMapLength() const override {
        return MIN_MAP_LENGTH;
    }

    size_t getMaxMapLength() const override {
        return MAX_MAP_LENGTH;
    }

    std::optional<bool> isMaster() const { return m_ismaster; };
    void setIsMaster(const bool ismaster) { m_ismaster = ismaster; };
    std::optional<bool> isPrivate() const { return m_isprivate; };
    void setIsPrivate(const bool isprivate) { m_isprivate = isprivate; };
    const KeyData& getKeyData() const { return m_keyData; };
    void setKeyData(const KeyData& keyData) { m_keyData = keyData; };
    const std::optional<ChainCode>& getChaincode() const { return m_chainCode; };
    void setChainCode(const ChainCode& chainCode) { m_chainCode = chainCode; };
    const std::optional<CoinInfo> &getUseInfo() const { return m_useInfo; };
    void setUseInfo(const CoinInfo &useInfo) { m_useInfo = useInfo; };
    const std::optional<Keypath> &getOrigin() const { return m_origin; };
    void setOrigin(const Keypath &origin) { m_origin = origin; };
    const std::optional<Keypath> &getChildren() const { return m_children; };
    void setChildren(const Keypath &children) { m_children = children; };
    std::optional<uint32_t> getParentFingerprint() const { return m_parentFingerprint; };
    void setParentFingerprint(const uint32_t parentFingerprint) { m_parentFingerprint = parentFingerprint; };
    const std::optional<std::string> &getName() const { return m_name; };
    void setName(const std::string &name) { m_name = name; };
    const std::optional<std::string> &getNote() const { return m_note; };
    void setNote(const std::string &note) { m_note = note; };

    size_t getMapSize() const override;
    void toMap(CborEncoder* parentEncoder) const override;
    void fromMap(CborValue*) override {
        throw CborException("Decoder not supported", CborErrorUnimplementedValidation);
    }

    bool operator == (const HDKey& hdkey) const {
        return (m_keyData == hdkey.getKeyData());
    };

    /**
     * @brief Deserialize the BIP32 key representation into the BIP32Key structure
     * - 4 bytes: version bytes (mainnet: 0x0488B21E public, 0x0488ADE4 private; testnet: 0x043587CF public, 0x04358394 private)
     * - 1 byte: depth: 0x00 for master nodes, 0x01 for level-1 derived keys, ....
     * - 4 bytes: the fingerprint of the parent's key (0x00000000 if master key)
     * - 4 bytes: child number. This is ser32(i) for i in xi = xpar/i, with xi the key being serialized. (0x00000000 if master key)
     * - 32 bytes: the chain code
     * - 33 bytes: the public key or private key data (serP(K) for public keys, 0x00 || ser256(k) for private keys)
     * 
     * @param key is the text string representation with xpub or xprv prefix
     */
    void deserializeFromBIP32(const std::string& key);

    /**
     * @brief Extract the parent fingerprint from the BIP32 string key
     * 
     * @param key is the text string representation with xpub or xprv prefix
     * @return uint32_t fingerprint, 0 in case of error
     */
    static uint32_t extractParentFingerprint(const std::string& key);

    void setOrigin(const std::string& derivationPath) { m_origin = Keypath(derivationPath); };
    void setOrigin(const std::string& derivationPath, const uint32_t sourceFingerprint) { m_origin = Keypath(derivationPath, sourceFingerprint); };
    void setOrigin(const std::string& derivationPath, const uint32_t sourceFingerprint, const bool withDepth) { m_origin = Keypath(derivationPath, sourceFingerprint, withDepth); };
    void setChildren(const std::string& derivationPath) { m_children = Keypath(derivationPath); };
    void setChildren(const std::string& derivationPath, const uint32_t sourceFingerprint) { m_children = Keypath(derivationPath, sourceFingerprint); };
    void setChildren(const std::string& derivationPath, const uint32_t sourceFingerprint, const bool withDepth) { m_children = Keypath(derivationPath, sourceFingerprint, withDepth); };

    void setUseInfo(const CoinType type, const NetworkType network) { m_useInfo = CoinInfo(type, network); };
    void setUseInfo(const CoinType type) { m_useInfo = CoinInfo(type); };
    void setUseInfo(const NetworkType network) { m_useInfo = CoinInfo(network); };

private:
    std::optional<bool> m_ismaster;
    std::optional<bool> m_isprivate;
    KeyData m_keyData;
    std::optional<ChainCode> m_chainCode;
    std::optional<CoinInfo> m_useInfo;
    std::optional<Keypath> m_origin;
    std::optional<Keypath> m_children;
    std::optional<uint32_t> m_parentFingerprint;
    std::optional<std::string> m_name;
    std::optional<std::string> m_note;
};
