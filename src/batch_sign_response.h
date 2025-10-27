#pragma once

#include "sign_response.h"
#include "registry_type.h"

/**
 * @brief  Implementation of the batch-sign-response (CBOR tag #6.41414) UR type encoder
 *
 * Source: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2025-001-batch-sign.md
 * CDDL specification:
 *
 * ; request-id inside each #6.41412(sign-response) must be specified to identify the corresponding signature in the batch
 *
 * batch-sign-response = {
 *     sign-responses: [+ #6.41412(sign-response)]
 * }
 *
 * sign-responses = 1
 *
 */

class BatchSignResponse : public RegistryItem
{
public:
    BatchSignResponse()
    {
        setRegistryType(BATCH_SIGN_RESPONE);
        m_signrespones.clear();
    }

    explicit BatchSignResponse(const std::vector<SignResponse> &signresponses) : BatchSignResponse()
    {
        m_signrespones = signresponses;
    }

    ~BatchSignResponse() override = default;

    enum class Key
    {
        SIGN_RESPONSES = 1
    };

    size_t getMinMapLength() const override
    {
        return MIN_MAP_LENGTH;
    }

    size_t getMaxMapLength() const override
    {
        return MAX_MAP_LENGTH;
    }

    void setSignResponses(const std::vector<SignResponse> &value) { m_signrespones = value; };
    const std::vector<SignResponse> &getSignResponses() const { return m_signrespones; };

    size_t getMapSize() const override
    {
        return MAX_MAP_LENGTH;
    };

    void toMap(CborEncoder *parentEncoder) const override;

    void fromMap(CborValue *) override
    {
        throw CborException("Decoder not supported", CborErrorUnimplementedValidation);
    };

private:
    static constexpr size_t MIN_MAP_LENGTH = 1;
    static constexpr size_t MAX_MAP_LENGTH = 1;

    std::vector<SignResponse> m_signrespones;
};
