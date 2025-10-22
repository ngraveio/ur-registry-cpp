#include "uai.h"
#include <iostream>

UniqueAssetId::UniqueAssetId(const std::string &uai)
{
    parseUAI(uai);
}

static void splitDerivationPath(const std::string &path, std::string &origin, std::string &children)
{
    std::istringstream stream(path);
    std::vector<std::string> components;
    std::string component;

    // Split the path by '/'
    while (std::getline(stream, component, '/'))
    {
        components.push_back(component);
    }
    // Start from the children to detect the first hardened path, definining the separation between origin and children
    std::reverse(components.begin(), components.end());
    auto inOrigin = false;
    for (const auto &comp : components)
    {
        if (!inOrigin)
        {
            auto found_hardened1 = comp.find('h');
            auto found_hardened2 = comp.find('\'');
            inOrigin = ((found_hardened1 != std::string::npos) || (found_hardened2 != std::string::npos)) ? true : false;
        }
        if (inOrigin)
        {
            origin = "/" + comp + origin;
        }
        else
        {
            children = "/" + comp + children;
        }
    }

    if (!origin.empty())
    {
        origin.replace(0, 1, "m/");
    }
    if (!children.empty())
    {
        children.replace(0, 1, "m/");
    }
}

static std::vector<std::string> splitSubtypes(const std::string &subtypesStr)
{
    std::vector<std::string> subtypeVector;
    if (!subtypesStr.empty())
    {
        std::istringstream stream(subtypesStr);
        std::string subtype;
        while (std::getline(stream, subtype, '.'))
        {
            subtypeVector.push_back(subtype);
        }
    }
    return subtypeVector;
}

void UniqueAssetId::parseUAI(const std::string &uai)
{
    /**
     * @brief Regex pattern to validate the UAI string
     * - Group 1: curve
     * - Group 2: type
     * - Group 3: subtypes (including the separators ".")
     * - Group 4: Descriptor function
     * - Group 5: Token ID (including subtypes if present)
     * - Group 6: Derivation path
     * - Group 7: Master fingerprint (only the uint32 value)
     *
     * Source: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2024-001-unique-asset-id.md
     *
     */
    static constexpr const char *uaiRegexPattern =
        R"(^uai:\/\/([a-zA-Z0-9]+)\.([0-9]+)(?:\.([a-zA-Z0-9_.-]+))?(?:@([a-zA-Z]+))?(?::([a-zA-Z0-9_.-]+))?((?:\/(?:[0-9\[\]\<,;]+)[h|']?(?:\>)?)+)?(?:\?master_fingerprint=([0-9]+))?$)";
    static constexpr uint8_t CURVE_GROUP = 1;
    static constexpr uint8_t TYPE_GROUP = 2;
    static constexpr uint8_t SUBTYPES_GROUP = 3;
    static constexpr uint8_t DESCRIPTOR_FUNCTION_GROUP = 4;
    static constexpr uint8_t TOKENID_GROUP = 5;
    static constexpr uint8_t DERIVATION_PATH_GROUP = 6;
    static constexpr uint8_t MASTER_FINGERPRINT_GROUP = 7;

    const std::regex uaiPattern(uaiRegexPattern);

    std::smatch matches;
    if (!std::regex_match(uai, matches, uaiPattern))
    {
        throw CborException("String does not match UAI format: " + uai, CborErrorImproperValue);
    }

    m_curveName = matches[CURVE_GROUP].str();

    m_type = matches[TYPE_GROUP].str();

    auto subtypesStr = matches[SUBTYPES_GROUP].str();
    if (!subtypesStr.empty())
    {
        auto subtypes = splitSubtypes(subtypesStr);
        m_subtypes = subtypes;
    }

    m_descriptor_function = matches[DESCRIPTOR_FUNCTION_GROUP].str();

    m_tokenid = matches[TOKENID_GROUP].str();

    auto derivationPathStr = matches[DERIVATION_PATH_GROUP].str();
    if (!derivationPathStr.empty())
    {
        // Remove "/" prefix
        m_derivationpath = derivationPathStr.substr(1);
        splitDerivationPath(m_derivationpath, m_origin, m_children);

        // Add the prefix "m/" to identify the derivation path
        m_derivationpath.insert(0, "m/");
    }

    m_masterfingerprint = matches[MASTER_FINGERPRINT_GROUP].str();
};

uint32_t UniqueAssetId::getMasterFingerprintValue() const
{
    if (!m_masterfingerprint.empty())
    {
        try
        {
            std::size_t pos{0};
            uint32_t masterFingerprintValue = std::stoul(m_masterfingerprint, &pos, 10);
            if (pos == m_masterfingerprint.size())
            {
                return masterFingerprintValue;
            }
            else
            {
                throw CborException("Invalid master fingerprint value", CborErrorImproperValue);
            }
        }
        catch (const std::invalid_argument &)
        {
            throw CborException("Invalid master fingerprint value", CborErrorImproperValue);
        }
    }
    return 0;
}