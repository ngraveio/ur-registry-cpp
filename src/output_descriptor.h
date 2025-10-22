#pragma once

#include <iostream>

#include "registry_item.h"
#include "hdkey.h"

/**
 * @brief  Implementation of the output-descriptor (CBOR tag #6.40308) UR type and its legacy version crypto-output (CBOR tag #6.308)
 *
 * Source: https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-2023-010-output-descriptor.md
 *
 * CDDL specification:
 * output-descriptor = #6.40308(
 *     {
 *         source: text,       ; text descriptor with keys replaced by placeholders
 *         ? keys: [+key], ; array of keys corresponding to placeholders, omitted if source is a complete text descriptor with no placeholders
 *         ? name: text,       ; optional user-assigned name
 *         ? note: text        ; optional user-assigned note
 *     }
 * )
 *
 * source = 1
 * keys = 2
 * name = 3
 * note = 4
 *
 * key = (
 *     hd-key /    ; BCR-2020-007
 *     ec-key /    ; BCR-2020-008
 *     address     ; BCR-2020-009
 * )
 *
 * @note: for keys, we only support hd-key.
 */

class OutputDescriptor : public RegistryItem
{
public:
    OutputDescriptor();
    ~OutputDescriptor() override = default;
    explicit OutputDescriptor(const std::string &source,
                              const std::vector<HDKey> &keys = {},
                              const std::optional<std::string> &name = std::nullopt,
                              const std::optional<std::string> &note = std::nullopt);

    enum class Key
    {
        SOURCE = 1,
        KEYS,
        NAME,
        NOTE
    };

    size_t getMinMapLength() const override { return MIN_MAP_LENGTH; }
    size_t getMaxMapLength() const override { return MAX_MAP_LENGTH; }
    size_t getMapSize() const override;

    void toMap(CborEncoder *parentEncoder) const override;
    void fromMap(CborValue *) override
    {
        throw CborException("Decoder not supported", CborErrorUnimplementedValidation);
    }

    std::string getSource() const { return m_source; };
    void setSource(const std::string &source) { m_source = source; };

    std::vector<HDKey> getKeys() const { return m_keys; };
    void setKeys(const std::vector<HDKey> &keys) { m_keys = keys; };

    std::optional<std::string> getName() const { return m_name; }
    void setName(const std::optional<std::string> &name) { m_name = name; };

    std::optional<std::string> getNote() const { return m_note; }
    void setNote(const std::optional<std::string> &note) { m_note = note; }

    bool operator==(const OutputDescriptor &output) const
    {
        return ((m_source == output.getSource()) && (m_keys == output.getKeys()));
    };

private:
    static constexpr size_t MIN_MAP_LENGTH = 1;
    static constexpr size_t MAX_MAP_LENGTH = 4;

    std::string m_source;
    std::vector<HDKey> m_keys;
    std::optional<std::string> m_name;
    std::optional<std::string> m_note;
};
