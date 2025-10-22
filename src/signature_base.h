#pragma once

#include <iostream>

#include "uuid.h"
#include "registry_item.h"

/**
 * @brief Base class for signature UR types
 *
 */
class SignatureBase : public RegistryItem
{
public:
    SignatureBase() = default;
    SignatureBase(const std::optional<Uuid> &requestId, const std::vector<uint8_t> &signature, const std::optional<std::string> &origin = std::nullopt, const std::optional<std::vector<uint8_t>> &publicKey = std::nullopt)
        : m_requestID(requestId), m_signature(signature), m_origin(origin), m_publickey(publicKey) {}
    virtual ~SignatureBase() override = default;

    virtual const std::optional<Uuid> &getRequestId() const { return m_requestID; }
    void setRequestID(const Uuid &requestID) { m_requestID = requestID; };
    const std::vector<uint8_t> &getSignature() const { return m_signature; };
    void setSignature(const std::vector<uint8_t> &signature) { m_signature = signature; };
    const std::optional<std::string> &getOrigin() const { return m_origin; };
    void setOrigin(const std::string &origin) { m_origin = origin; };

    const std::optional<std::vector<uint8_t>> &getPublickey() const { return m_publickey; };
    void setPublickey(const std::optional<std::vector<uint8_t>> &value) { m_publickey = value; };

    size_t getSignatureSize() const { return m_signature.size(); };
    void clearSignature() { m_signature.clear(); };

    bool cbor_encode_request_id(CborEncoder *container, const uint8_t map_index) const;
    void cbor_encode_signature(CborEncoder *container, const uint8_t map_index) const;
    bool cbor_encode_origin(CborEncoder *container, const uint8_t map_index) const;
    bool cbor_encode_public_key(CborEncoder *container, const uint8_t map_index) const;

private:
    std::optional<Uuid> m_requestID;
    std::vector<uint8_t> m_signature;
    std::optional<std::string> m_origin;
    std::optional<std::vector<uint8_t>> m_publickey;
};
