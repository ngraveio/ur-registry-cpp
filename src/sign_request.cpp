#include "sign_request.h"
#include <bc-ur/utils.hpp>

SignRequest::SignRequest() {
    setRegistryType(SIGN_REQUEST);
    m_txtype = 1;
}

SignRequest::SignRequest(const CoinIdentity &coinId, const BytesVector &sign_data) : SignRequest()
{
    setSignData(sign_data);
    setCoinIdentity(coinId);
}

SignRequest::SignRequest(const Uuid &uuid, const CoinIdentity &coinId, const Keypath &derivationPath, const BytesVector &sign_data, const std::string &origin, const StringOrBytesVariant &address, const Intent& intent) : SignRequest(coinId, sign_data)
{
    setRequestID(uuid);
    setDerivationPath(derivationPath);
    setOrigin(origin);
    setAddress(address);
    setIntent(intent);
}

size_t SignRequest::getMapSize() const
{
    size_t size = MIN_MAP_LENGTH;

    if(getRequestID().has_value()) ++size;
    if(getDerivationPath().has_value()) ++size;
    if(getOrigin().has_value()) ++size;
    if(getTxtype().has_value()) ++size;
    if(std::holds_alternative<std::monostate>(m_address) == false) ++size;
    if(getIntent().has_value()) ++size;
    
    return size;    
}

std::string SignRequest::getAddressStr() const
{
    if(const auto adrPtr = std::get_if<std::string>(&m_address))
        return *adrPtr;
    if(const auto adrPtr = std::get_if<BytesVector>(&m_address)) {
        return ur::data_to_hex(*adrPtr);
    }
    return "";
}

void SignRequest::fromMap(CborValue *cborMap) {
    CborValue localDecoder;    
    CborError err = cbor_value_enter_container(cborMap, &localDecoder);
    checkCborError(err, "Failed to enter localDecoder");
    
    clearAllItems();
    
    bool uuidFound = false;
    bool coinIdFound = false;
    bool derivationPathFound = false;
    bool signDataFound = false;
    bool originFound = false;
    bool txTypeFound = false;
    bool addressFound = false;
    bool IntentFound = false;

    while(!cbor_value_at_end(&localDecoder)) {
        uint64_t keyIndex;

        checkIsUint(&localDecoder);
        err = cbor_value_get_uint64(&localDecoder, &keyIndex);
        checkCborError(err, "Failed to get uint");
        checkMaxUint8(keyIndex);
        
        err = cbor_value_advance(&localDecoder);
        checkCborError(err, "Failed to advance");
        const Key key = static_cast<Key>(keyIndex);
        
        switch (key) {
        case Key::REQUEST_ID: {
            if (uuidFound)
                throw CborException("UUID duplicated", CborErrorMapKeysNotUnique);
            uuidFound = true;

            this->cbor_decode_requestid(&localDecoder);
            break;
        }
        case Key::COIN_IDENTITY: {
            if (coinIdFound)
                throw CborException("Coin-identity map key duplicated", CborErrorMapKeysNotUnique);

            coinIdFound = true;
            validateTag(&localDecoder, COIN_IDENTITY);
            
            err = cbor_value_advance(&localDecoder);
            checkCborError(err, "Failed to advance");
            
            checkIsMap(&localDecoder);
            m_coin_identity.fromMap(&localDecoder);

            break;
        }
        case Key::DERIVATION_PATH: {
            if (derivationPathFound)
                throw CborException("Derivation path map key duplicated", CborErrorMapKeysNotUnique);

            derivationPathFound = true;
            validateTag(&localDecoder, KEYPATH);       
            cbor_decode_derivation_path(&localDecoder);

            break;
        }
        case Key::SIGN_DATA: {
            if (signDataFound)
                throw CborException("Sign data map key duplicated", CborErrorMapKeysNotUnique);

            signDataFound = true;            
            cbor_decode_sign_data(&localDecoder);
            
            break;
        }
        case Key::ORIGIN: {
            if (originFound)
                throw CborException("Origin map key duplicated", CborErrorMapKeysNotUnique);
            
            originFound = true;            
            cbor_decode_origin(&localDecoder);

            break;
        }
        case Key::TXTYPE: {
            if (txTypeFound)
                throw CborException("Transaction type map key duplicated", CborErrorMapKeysNotUnique);

            txTypeFound = true;
            
            uint64_t dataType = 0;
            checkIsUint(&localDecoder);
            err = cbor_value_get_uint64(&localDecoder, &dataType);
            checkCborError(err, "Failed to get uint");
            
            m_txtype = dataType;
            break;
        }
        case Key::ADDRESS: {
            if (addressFound) {
                throw CborException("Address map key duplicated", CborErrorMapKeysNotUnique);
            }
            addressFound = true;            
            size_t address_size = 0;           

            if (cbor_value_is_byte_string(&localDecoder)) {
                err = cbor_value_get_string_length(&localDecoder, &address_size);
                checkCborError(err, "Failed to get bytes string length");

                BytesVector address(address_size, 0);
                err = cbor_value_copy_byte_string(&localDecoder, address.data(), &address_size, nullptr);
                checkCborError(err, "Failed to copy bytes string");            
                m_address = std::move(address);

            } else if (cbor_value_is_text_string(&localDecoder)) {
                err = cbor_value_get_string_length(&localDecoder, &address_size);
                checkCborError(err, "Failed to get text string length");
                
                std::string address(address_size, '\0');
                err = cbor_value_copy_text_string(&localDecoder, address.data(), &address_size, nullptr);
                checkCborError(err, "Failed to copy text string");            
                m_address = std::move(address);

            } else {
                throw CborException("Expected bytes string or text string for the Address field.", CborErrorIllegalType);
            }
            
            break;
        }
        case Key::INTENT: {
            if (IntentFound)
                throw CborException("Sign-intent map key duplicated", CborErrorMapKeysNotUnique);

            IntentFound = true;
            validateTag(&localDecoder, INTENT);
            
            err = cbor_value_advance(&localDecoder);
            checkCborError(err, "Failed to advance");
            
            checkIsMap(&localDecoder);
            
            Intent intent;
            intent.fromMap(&localDecoder);            
            m_intent.emplace(std::move(intent));

            break;
        }
        default: {
            throw CborException("Unknown map key", CborErrorUnknownType);
        }
        }

        if( key == Key::SIGN_DATA || key == Key::REQUEST_ID 
                || key == Key::ORIGIN || key == Key::TXTYPE 
                || key == Key::ADDRESS) {
            err = cbor_value_advance(&localDecoder);
            checkCborError(err, "Failed to advance");
        }
        
    }
    
    err = cbor_value_leave_container(cborMap, &localDecoder);
    checkCborError(err, "Failed to leave container");

    if (!coinIdFound)
        throw CborException("Mandatory coin-id key missing", CborErrorTooFewItems);

    if (!signDataFound)
        throw CborException("Mandatory sign-data key missing", CborErrorTooFewItems);
    
}

void SignRequest::clearAllItems()
{
    clearBaseItems();
    m_coin_identity = CoinIdentity{};
    m_txtype = 1;
    m_address = std::monostate();
    m_intent = std::nullopt;
}
