#include "intent.h"


size_t Intent::getMapSize() const
{
    if (!m_substitute_fields.prefix_str.empty() || !m_substitute_fields.fields_array.empty())
        return MAX_MAP_LENGTH;
    return 0;
}

void Intent::fromMap(CborValue *cborMap) {
    CborValue localDecoder;    
    CborError err = cbor_value_enter_container(cborMap, &localDecoder);
    checkCborError(err, "Failed to enter localDecoder");    
    
    bool subtFieldsFound = false;

    while (!cbor_value_at_end(&localDecoder)) {
        uint64_t key;
        checkIsUint(&localDecoder);
        err = cbor_value_get_uint64(&localDecoder, &key);
        checkCborError(err, "Failed to get uint");
        checkMaxUint8(key);
        
        err = cbor_value_advance(&localDecoder);
        checkCborError(err, "Failed to advance");
        
        switch (static_cast<Key>(key)) {
        case Key::SUBTITUTE_FIELDS: {
            
            if (subtFieldsFound)
                throw CborException("Subtitute fields key is duplicated", CborErrorMapKeysNotUnique);
            
            subtFieldsFound = true;
            
            checkIsMap(&localDecoder);
            
            CborValue subtFieldsMap;
            err = cbor_value_enter_container(&localDecoder, &subtFieldsMap);
            checkCborError(err, "Failed to enter map container");
            
            uint64_t subtFieldsMapKey;
            
            checkIsUint(&subtFieldsMap);
            err = cbor_value_get_uint64(&subtFieldsMap, &subtFieldsMapKey);
            checkCborError(err, "Failed to get uint");
            checkMaxUint8(subtFieldsMapKey);
            
            if(subtFieldsMapKey != 1)
                throw CborException("Invalid key for placeholder prefix", CborErrorIllegalNumber);
            
            err = cbor_value_advance(&subtFieldsMap);
            checkCborError(err, "Failed to advance");
            
            checkIsTextStr(&subtFieldsMap);
            
            size_t strLen {0};
            err = cbor_value_get_string_length(&subtFieldsMap, &strLen);
            checkCborError(err, "Failed to get string length");
            
            m_substitute_fields.prefix_str = std::string(strLen, '\0');

            err = cbor_value_copy_text_string(&subtFieldsMap, m_substitute_fields.prefix_str.data(), &strLen, nullptr);
            checkCborError(err, "Failed to copy text string");
            
            err = cbor_value_advance(&subtFieldsMap);
            checkCborError(err, "Failed to advance");

            checkIsUint(&subtFieldsMap);
            err = cbor_value_get_uint64(&subtFieldsMap, &subtFieldsMapKey);
            checkCborError(err, "Failed to get uint");
            checkMaxUint8(subtFieldsMapKey);
            
            if(subtFieldsMapKey != 2)
                throw CborException("Invalid key for parameters array", CborErrorIllegalNumber);
            
            err = cbor_value_advance(&subtFieldsMap);
            checkCborError(err, "Failed to advance");
            
            checkIsArray(&subtFieldsMap);
            
            size_t paramArrayLen {0};
            size_t paramLen {0};            
            CborValue paramArray;            
            
            err = cbor_value_get_array_length(&subtFieldsMap, &paramArrayLen);
            checkCborError(err, "Failed to get array length");
            
            err = cbor_value_enter_container(&subtFieldsMap, &paramArray);
            checkCborError(err, "Failed to enter params array");
            
            while(!cbor_value_at_end(&paramArray)) {                
                checkIsArray(&paramArray);
                err = cbor_value_get_array_length(&paramArray, &paramLen);
                checkCborError(err, "Failed to get array length");
                
                if(paramLen != 2)
                    throw CborException("Invalid length for parameter tuple", CborErrorIllegalNumber);
                
                CborValue paramTuple;
                err = cbor_value_enter_container(&paramArray, &paramTuple);
                checkCborError(err, "Failed to enter params array");
                
                // check for the name/identifier
                checkIsTextStr(&paramTuple);
                
                strLen = 0;
                err = cbor_value_get_string_length(&paramTuple, &strLen);
                checkCborError(err, "Failed to get string length");
                
                Parameter tmpParam {std::string(strLen, '\0'), {}};
                
                err = cbor_value_copy_text_string(&paramTuple, tmpParam.first.data(), &strLen, nullptr);
                checkCborError(err, "Failed to copy text string");
                
                err = cbor_value_advance(&paramTuple);
                checkCborError(err, "Failed to advance");
                
                // check for the encoding type int
                checkIsInt(&paramTuple);
                err = cbor_value_get_int(&paramTuple, &tmpParam.second);
                checkCborError(err, "Failed to get int");
                
                m_substitute_fields.fields_array.emplace_back(std::move(tmpParam));
                
                err = cbor_value_advance(&paramTuple);
                checkCborError(err, "Failed to advance");
                
                // leaving the container advances the parent container
                err = cbor_value_leave_container(&paramArray, &paramTuple);
                checkCborError(err, "Failed to leave params array container");
                
            }
            
            err = cbor_value_leave_container(&subtFieldsMap, &paramArray);
            checkCborError(err, "Failed to leave array container");
            err = cbor_value_leave_container(&localDecoder, &subtFieldsMap);
            checkCborError(err, "Failed to leave map container");            
            
            break;
        }
        default: {
            throw CborException("Unknown map key", CborErrorUnknownType);
        }
        }
        
    }
    
    err = cbor_value_leave_container(cborMap, &localDecoder);
    checkCborError(err, "Failed to leave container");

    if (!subtFieldsFound)
        throw CborException("Mandatory substitute-fields key missing", CborErrorTooFewItems);
    
}

