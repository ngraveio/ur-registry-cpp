#pragma once

#include <iostream>
#include <vector>
#include <optional>

#include "sign_request.h"

/**
 * @brief Implementation of the batch-sign-request (CBOR tag #6.41413) UR type
 *
 * Source: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2025-001-batch-sign.md
 * CDDL specification:
 *
 * ; request-id inside each #6.41411(sign-request) must be specified to identify each transaction in the batch

 * batch-sign-request = {
 *    sign-requests: [+ #6.41411(sign-request)]
 * }
 *
 * sign-requests = 1
 */

class BatchSignRequest : public RegistryItem
{
public:
    BatchSignRequest()
    {
        setRegistryType(BATCH_SIGN_REQUEST);
    };

    virtual ~BatchSignRequest() override = default;
    explicit BatchSignRequest(const std::vector<SignRequest> &sign_requests) : m_sign_requests(sign_requests) {};

    enum class Key
    {
        SIGN_REQUESTS = 1,
    };

    size_t getMinMapLength() const override
    {
        return MIN_MAP_LENGTH;
    }

    size_t getMaxMapLength() const override
    {
        return MAX_MAP_LENGTH;
    }

    const std::vector<SignRequest> &getSignRequests() const { return m_sign_requests; }
    void setSignRequests(const std::vector<SignRequest> &value) { m_sign_requests = value; }

    size_t getMapSize() const override { return MAX_MAP_LENGTH; };

    void toMap(CborEncoder *) const override
    {
        throw CborException("Encoder not supported", CborErrorUnimplementedValidation);
    };

    void fromMap(CborValue *map) override;

private:
    static constexpr size_t MIN_MAP_LENGTH = 1;
    static constexpr size_t MAX_MAP_LENGTH = 1;

    std::vector<SignRequest> m_sign_requests;
};
