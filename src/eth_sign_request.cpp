#include "eth_sign_request.h"
#include <bc-ur/utils.hpp>

EthSignRequest::EthSignRequest(){
    setRegistryType(ETH_SIGN_REQUEST);
    // Default values for optional fields
    m_data_type = DataType::EthTransactionData;
    m_chain_id = 1;
}

void EthSignRequest::fromMap(CborValue* map) {
    CborError err = CborNoError;
    CborValue container;
    clearAllItems();

    err = cbor_value_enter_container(map, &container);
    checkCborError(err, "Failed to enter container");

    bool requestIdFound = false;
    bool signDataFound = false;
    bool dataTypeFound = false;
    bool chainIdFound = false;
    bool derivationPathFound = false;
    bool addressFound = false;
    bool originFound = false;
    while (!cbor_value_at_end(&container)) {
        uint64_t key;
        
        checkIsUint(&container);
        err = cbor_value_get_uint64(&container, &key);
        checkCborError(err, "Failed to get uint");
        checkMaxUint8(key);

        err = cbor_value_advance(&container);
        checkCborError(err, "Failed to advance");

        switch (static_cast<Key>(key)) {
            case Key::REQUEST_ID: {
                if (requestIdFound) {
                    throw CborException("Request ID map key duplicated", CborErrorMapKeysNotUnique);
                }
                requestIdFound = true;

                cbor_decode_requestid(&container);
                break;
            }
            case Key::SIGN_DATA: {
                if (signDataFound) {
                    throw CborException("Sign data map key duplicated", CborErrorMapKeysNotUnique);
                }
                signDataFound = true;

                cbor_decode_sign_data(&container);
                break;
            }
            case Key::DATA_TYPE: {
                if (dataTypeFound) {
                    throw CborException("Data type map key duplicated", CborErrorMapKeysNotUnique);
                }
                dataTypeFound = true;

                uint64_t dataType = 0;
                checkIsUint(&container);
                err = cbor_value_get_uint64(&container, &dataType);
                checkCborError(err, "Failed to get uint");

                m_data_type = static_cast<DataType>(dataType);
                break;
            }
            case Key::CHAIN_ID: {
                if (chainIdFound) {
                    throw CborException("Chain ID map key duplicated", CborErrorMapKeysNotUnique);
                }
                chainIdFound = true;

                uint64_t chainId = 0;
                checkIsUint(&container);
                err = cbor_value_get_uint64(&container, &chainId);
                checkCborError(err, "Failed to get uint");

                checkMaxUint32(chainId);
                m_chain_id = static_cast<uint32_t>(chainId);
                break;
            }
            case Key::DERIVATION_PATH: {
                if (derivationPathFound) {
                    throw CborException("Derivation path map key duplicated", CborErrorMapKeysNotUnique);
                }
                derivationPathFound = true;
                
                checkIsTag(&container);
                CborTag tag{};
                err = cbor_value_get_tag(&container, &tag);                
                checkCborError(err, "Failed to get keypath tag");
                
                if (tag != KEYPATH.tag() && tag != CRYPTO_KEYPATH.tag())
                    throw CborException("Decoded keypath tag is incorrect", CborErrorInappropriateTagForType);
                                    
                cbor_decode_derivation_path(&container);
                break;
            }
            case Key::ADDRESS: {
                if (addressFound) {
                    throw CborException("Address map key duplicated", CborErrorMapKeysNotUnique);
                }
                addressFound = true;

                EthAddress address;
                size_t address_size = 0;
                checkIsByteStr(&container);
                err = cbor_value_get_string_length(&container, &address_size);
                checkCborError(err, "Failed to get string length");
                if (address_size != ETH_ADDRESS_SIZE) {
                    throw CborException("Address size incorrect", CborErrorImproperValue);
                }
                err = cbor_value_copy_byte_string(&container, address.data(), &address_size, nullptr);
                checkCborError(err, "Failed to copy byte string");

                m_address.emplace(std::move(address));
                break;
            }
            case Key::ORIGIN: {
                if (originFound) {
                    throw CborException("Origin map key duplicated", CborErrorMapKeysNotUnique);
                }
                originFound = true;

                cbor_decode_origin(&container);
                break;
            }
            default: {
                throw CborException("Unknown map key", CborErrorUnknownType);
            }
        }

        if (key != static_cast<uint64_t>(Key::DERIVATION_PATH)) {
            err = cbor_value_advance(&container);
            checkCborError(err, "Failed to advance");
        }
    }

    // Verify that mandatory keys are present
    if (!signDataFound) {
        throw CborException("Mandatory sign data key missing", CborErrorTooFewItems);
    }
    if (!derivationPathFound) {
        throw CborException("Mandatory derivation path key missing", CborErrorTooFewItems);
    }

    err = cbor_value_leave_container(map, &container);
    checkCborError(err, "Failed to leave container");
}

std::string EthSignRequest::getAddressStr() const
{
    if(m_address.has_value())
        return ur::data_to_hex(std::vector<uint8_t>(m_address.value().begin(), m_address.value().end()));
    return "";
}

void EthSignRequest::clearAllItems()
{
    clearBaseItems();
    m_data_type = DataType::EthTransactionData;
    m_chain_id = 1;
    m_address = std::nullopt;
}

