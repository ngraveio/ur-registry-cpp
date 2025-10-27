#pragma once

#include "registry_item.h"
#include "uuid.h"
#include "keypath.h"

/** @brief: Base class for *-sign-request UR types decoders. */

class RequestBase : public RegistryItem
{
public:
    RequestBase() = default;
    virtual ~RequestBase() override = default;

    void cbor_decode_requestid(CborValue *container);
    void cbor_decode_derivation_path(CborValue *container);
    void cbor_decode_sign_data(CborValue *container);
    void cbor_decode_origin(CborValue *container);
    void clearBaseItems();

    static void validateTag(const CborValue *encoder, const RegistryType &tagUrType);

    std::optional<Uuid> getRequestID() const { return m_requestid; }
    void setRequestID(const std::optional<Uuid> &RequestID) { m_requestid = RequestID; }

    std::optional<Keypath> getDerivationPath() const { return m_derivation_path; }
    void setDerivationPath(const std::optional<Keypath> &derivation_path) { m_derivation_path = derivation_path; }
    void setDerivationPath(const std::string &derivationPath) { m_derivation_path = Keypath(derivationPath); };
    void setDerivationPath(const std::string &derivationPath, const uint32_t sourceFingerprint) { m_derivation_path = Keypath(derivationPath, sourceFingerprint); };
    void setDerivationPath(const std::string &derivationPath, const uint32_t sourceFingerprint, const bool withDepth) { m_derivation_path = Keypath(derivationPath, sourceFingerprint, withDepth); };

    std::vector<uint8_t> getSignData() const { return m_sign_data; }
    void setSignData(const std::vector<uint8_t> &sign_data) { m_sign_data = sign_data; }

    std::optional<std::string> getOrigin() const { return m_origin; }
    void setOrigin(const std::optional<std::string> &origin) { m_origin = origin; }

    virtual std::string getAddressStr() const = 0;

private:
    std::optional<Uuid> m_requestid;
    std::optional<std::string> m_origin;
    std::optional<Keypath> m_derivation_path;
    std::vector<uint8_t> m_sign_data;
};
