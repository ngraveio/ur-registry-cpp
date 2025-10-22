#include "batch_sign_request.h"

void BatchSignRequest::fromMap(CborValue* map)
{
    CborError err = CborNoError;
    CborValue container;
    
    err = cbor_value_enter_container(map, &container);
    checkCborError(err, "Failed to enter container");
    
    bool requestsFound = false;
    
    while (!cbor_value_at_end(&container)) {
        uint64_t key;
        checkIsUint(&container);
        err = cbor_value_get_uint64(&container, &key);
        checkCborError(err, "Failed to get uint");
        checkMaxUint8(key);
        
        err = cbor_value_advance(&container);
        checkCborError(err, "Failed to advance");
        
        switch (static_cast<Key>(key)) {
        case Key::SIGN_REQUESTS: {
            
            if (requestsFound)
                throw CborException("Sign requests array key is duplicated", CborErrorMapKeysNotUnique);
            
            requestsFound = true;
            
            checkIsArray(&container);
            
            size_t arraySize = 0;
            err = cbor_value_get_array_length(&container, &arraySize);
            checkCborError(err, "Failed to get array length");
            
            if (arraySize == 0)
                throw CborException("Sign requests array cannot be empty", CborErrorImproperValue);
            
            CborValue array;
            err = cbor_value_enter_container(&container, &array);
            checkCborError(err, "Failed to enter array container");
            
            while (!cbor_value_at_end(&array)) {
                if (cbor_value_is_tag(&array)) {
                    CborTag tag;
                    err = cbor_value_get_tag(&array, &tag);
                    checkCborError(err, "Failed to get tag");
                    
                    RequestBase::validateTag(&array, SIGN_REQUEST);
                    
                    err = cbor_value_advance(&array);
                    checkCborError(err, "Failed to advance");
                    
                    checkIsMap(&array);
                    
                    SignRequest request;
                    request.fromMap(&array);
                    
                    if(!request.getRequestID().has_value() || request.getRequestID().value().str() == "")
                        throw CborException("Requests IDs can't be empty", CborErrorImproperValue);
                    
                    m_sign_requests.emplace_back(std::move(request));
                    
                    continue;
                } else {
                    throw CborException("Invalid field type (should be array)", CborErrorIllegalType);
                }
                
                err = cbor_value_advance(&array);
                checkCborError(err, "Failed to advance");
            }
            
            err = cbor_value_leave_container(&container, &array);
            checkCborError(err, "Failed to leave array container");
            
            break;
        }
        default: {
            throw CborException("Unknown map key", CborErrorUnknownType);
        }
        }
        
    }
    
    if (!requestsFound) {
        throw CborException("Mandatory sign-requests array is missing", CborErrorTooFewItems);
    }
    
    err = cbor_value_leave_container(map, &container);
    checkCborError(err, "Failed to leave container");
}
