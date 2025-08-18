#pragma once

#include <regex>
#include "cbor_exception.h"

/**
 * @brief Unique Asset ID object as defined in NBCR-2024-01
 * Format: uai://curve.type[.subtype1.subtype2]:tokenid[.subtype1.subtype2]/derivation_path?master_fingerprint=uint32
 * 
 * Source: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2024-001-unique-asset-id.md
 */
class UniqueAssetId {
public:
    explicit UniqueAssetId(const std::string &uai);
        
    ~UniqueAssetId() = default;

    std::string getCurveName() const { return m_curveName; };
    void setCurveName(const std::string& curveName) { m_curveName = curveName; };

    std::string getType() const { return m_type; };
    void setType(const std::string& type) { m_type = type; };

    std::vector<std::string> getSubtypes() const { return m_subtypes; };
    void setSubtypes(const std::vector<std::string>& subtypes) { m_subtypes = subtypes; };

    std::string getTokenId() const { return m_tokenid; };
    void setTokenId(const std::string& tokenid) { m_tokenid = tokenid; };
    bool isToken() const { return (m_tokenid != ""); };

    std::string getDescriptorFunction() const { return m_descriptor_function; };
    void setDescriptorFunction(const std::string& function) { m_descriptor_function = function; };
    bool isOutputDescriptor() const { return (m_descriptor_function != ""); };

    std::string getDerivationPath() const { return m_derivationpath; };
    void setDerivationPath(const std::string& derivationpath) { m_derivationpath = derivationpath; };

    std::string getOrigin() const { return m_origin; };
    void setOrigin(const std::string& origin) { m_origin = origin; };

    std::string getChildren() const { return m_children; };
    void setChildren(const std::string& children) { m_children = children; };

    std::string getMasterFingerprint() const { return m_masterfingerprint; };
    void setMasterFingerprint(const std::string& masterfingerprint) { m_masterfingerprint = masterfingerprint; };

    void parseUAI(const std::string& uai);
    uint32_t getMasterFingerprintValue() const;

private:
    std::string m_curveName;
    std::string m_type;
    std::vector<std::string> m_subtypes;
    std::string m_descriptor_function;
    std::string m_tokenid;
    std::string m_derivationpath;
    std::string m_origin;
    std::string m_children;
    std::string m_masterfingerprint;

};
