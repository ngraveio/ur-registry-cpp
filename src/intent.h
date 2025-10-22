#pragma once

#include <iostream>
#include <utility>
#include <optional>

#include "registry_item.h"


/**
 * @brief Implementation of the sigh-intent (CBOR tag #6.41415) UR type
 *
 * Source: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2025-002-intent.md
 * CDDL specification:
 * 
 * ; The encoding_type defines how the parameter value should be interpreted.
 * ; This enumeration is extensible to support future encoding formats.
 * encoding_type = uint .size 2
 * integer = 1
 * hex = 2
 * base32 = 3
 * base58 = 4
 * base64 = 5
 * ascii = 6
 * utf8 = 7

 * ; Each parameter specifies a name that uniquely identifies a placeholder 
 * ; in the transaction and the encoding used to interpret its replacement value.
 * parameter = [
 *     name: string,
 *     encoding: encoding_type
 * ]
 * 
 * ; substitute_fields defines a prefix used to mark placeholders in the transaction,
 * ; and the list of parameters that should replace these placeholders.
 * substitute_fields = {
 *     placeholder_prefix: string,
 *     parameters: [parameter]
 * }
 * 
 * placeholder_prefix = 1
 * parameters = 2
 * 
 * ; The top-level intent structure is a tagged CBOR map.
 * ; Additional intent types can be added in the future using new map keys.
 * intent = (
 *     ?substitute: substitute_fields
 * )
 * 
 * substitute = 1
 * 
 */

using Parameter = std::pair<std::string, int>;

class Intent : public RegistryItem {

public:

    Intent() {
        setRegistryType(INTENT);
    }
    
    Intent(const std::string& prefix, const std::vector<Parameter>& fields_list) : Intent() {
        m_substitute_fields.prefix_str = prefix;
        m_substitute_fields.fields_array = fields_list;
    }

    virtual ~Intent() override = default;
    
    struct SubstituteFieldsStruct {
        std::string prefix_str;
        std::vector<Parameter> fields_array;
    };
    
    enum EncodingType {
        INT = 1,
        HEX,
        BASE32,
        BASE58,
        BASE64,
        ASCII,
        UTF8,
    };
    
    enum class Key {
        SUBTITUTE_FIELDS = 1
    };

    size_t getMinMapLength() const override {
        return MIN_MAP_LENGTH;
    }

    size_t getMaxMapLength() const override {
        return MAX_MAP_LENGTH;
    }

    size_t getMapSize() const override;
    void toMap(CborEncoder*) const override {
        throw CborException("Encoder not supported", CborErrorUnimplementedValidation);
    };

    void fromMap(CborValue*) override;    
    
    SubstituteFieldsStruct getSubtituteFields() const {
        return m_substitute_fields;
    }
    void setSubtituteFields(const SubstituteFieldsStruct& value) {
        m_substitute_fields = value;
    }
    
private:
    static constexpr size_t MIN_MAP_LENGTH = 1;
    static constexpr size_t MAX_MAP_LENGTH = 1;

    SubstituteFieldsStruct m_substitute_fields;    
};
