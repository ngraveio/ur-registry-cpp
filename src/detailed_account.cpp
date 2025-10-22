#include "detailed_account.h"
#include "uai.h"
#include "output_descriptor_function.h"

#include <regex>
#include <charconv>

DetailedAccount::DetailedAccount() {
    setRegistryType(DETAILED_ACCOUNT);
}

DetailedAccount::DetailedAccount(const std::variant<std::monostate, HDKey, OutputDescriptor> &account, const std::vector<std::string> &tokenids)
    : DetailedAccount() {
    setAccount(account);
    setTokenids(tokenids);
}

DetailedAccount::DetailedAccount(const std::string& uai, const std::string& key)
    : DetailedAccount() {
    setDetailedAccount(uai, key);
}

size_t DetailedAccount::getMapSize() const
{
    return (m_tokenids.empty() ? MIN_MAP_LENGTH : MIN_MAP_LENGTH + 1);
}


void DetailedAccount::toMap(CborEncoder* parentEncoder) const {
    if (this->getUseLegacyType().has_value() && this->getUseLegacyType().value())
        throw CborException("Legacy type is not supported.", CborErrorIllegalType);
    
    CborError err = CborNoError;
    CborEncoder localEncoder;

    err = cbor_encoder_create_map(parentEncoder, &localEncoder, getMapSize());
    checkCborError(err, "Failed to create detailed-account map");    

    if(std::holds_alternative<std::monostate>(m_account))
        throw CborException("Account field should not be empty.", CborErrorImproperValue);
    
    err = cbor_encode_uint(&localEncoder, static_cast<uint8_t>(Key::ACCOUNT));
    checkCborError(err, "Failed to encode uint");

    if (const auto hdkeyAccount = std::get_if<HDKey>(&m_account)) {
        err = cbor_encode_tag(&localEncoder, hdkeyAccount->getRegistryType().tag());
        checkCborError(err, "Failed to encode tag");        
        hdkeyAccount->toMap(&localEncoder);
    } else if (const auto outputDescAccount = std::get_if<OutputDescriptor>(&m_account)) {
        err = cbor_encode_tag(&localEncoder, outputDescAccount->getRegistryType().tag());
        checkCborError(err, "Failed to encode tag");        
        outputDescAccount->toMap(&localEncoder);
    }
    
    if(!m_tokenids.empty()) {
        err = cbor_encode_uint(&localEncoder, static_cast<uint8_t>(Key::TOKENIDS));
        checkCborError(err, "Failed to encode uint");

        CborEncoder arrayEncoder;
        err = cbor_encoder_create_array(&localEncoder, &arrayEncoder, m_tokenids.size());
        checkCborError(err, "Failed to create cbor array");        

        for(const std::string_view tokenid: m_tokenids) {
            err = cbor_encode_text_string(&arrayEncoder, tokenid.data(), tokenid.length());
            checkCborError(err, "Failed to encode tokenid str");
        }

        err = cbor_encoder_close_container(&localEncoder, &arrayEncoder);
        checkCborError(err, "Failed to close cbor array");
    }

    err = cbor_encoder_close_container(parentEncoder, &localEncoder);
    checkCborError(err, "Failed to close localEncoder");
}

static std::string getFunctionType(const UniqueAssetId& UaiObject) {
    auto function = UaiObject.getDescriptorFunction();
    if (OutputDescriptorFunction::getFunctionType(function) == DescriptorFunction::Unknown) {
        throw CborException("Unknown descriptor function", CborErrorImproperValue);
    }
    return function;
}

void DetailedAccount::setDetailedAccount(const std::string& uai, const std::string& key) {
    UniqueAssetId UaiObject(uai);

    auto tokenId = UaiObject.getTokenId();
    if (!tokenId.empty()) {
        this->addTokenId(tokenId);
    }

    HDKey hdkey;
    try {
        hdkey.deserializeFromBIP32(key);
    } catch (const CborException& e) {
        // This is not an xpub, encode as public key
        hdkey.setKeyData(BIP32Deserializer::hexStringToKeyData(key));
    }

    auto derivationPath = UaiObject.getDerivationPath();
    if (!derivationPath.empty()) {
        auto origin = UaiObject.getOrigin();
        auto children = UaiObject.getChildren();
        if (!origin.empty()) {
            hdkey.setOrigin(origin);
        }
        if (!children.empty()) {
            hdkey.setChildren(children);
        }
    }

    if (UaiObject.isOutputDescriptor()) {
        auto descriptor_function = getFunctionType(UaiObject);
        descriptor_function.append("(@0)");
        OutputDescriptor output{descriptor_function, {hdkey}};
        m_account = output;
    } else {
        m_account = hdkey;
    }
}
