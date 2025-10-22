#pragma once

#include "request_base.h"

constexpr size_t ETH_ADDRESS_SIZE = 20;

using EthAddress = std::array<uint8_t, ETH_ADDRESS_SIZE>;

/**
 * @brief  Implementation of the eth-sig-request (CBOR tag #6.401) UR type
 * 
 * Source: https://eips.ethereum.org/EIPS/eip-4527
 * CDDL specification:
 * 
 * ; Metadata for the signing request for Ethereum.
 * ; 
 * sign-data-type = {
 *     type: int .default 1 transaction data; the unsigned data type
 * }
 * 
 * eth-transaction-data = 1; legacy transaction rlp encoding of unsigned transaction data
 * eth-typed-data = 2; EIP-712 typed signing data
 * eth-raw-bytes=3;   for signing message usage, like EIP-191 personal_sign data
 * eth-typed-transaction=4; EIP-2718 typed transaction of unsigned transaction data
 * 
 * ; Metadata for the signing request for Ethereum.
 * ; request-id: the identifier for this signing request.
 * ; sign-data: the unsigned data
 * ; data-type: see sign-data-type definition
 * ; chain-id: chain id definition see https://github.com/ethereum-lists/chains for detail
 * ; derivation-path: the key path of the private key to sign the data
 * ; address: Ethereum address of the signing type for verification purposes which is optional
 * 
 * eth-sign-request = (
 *     sign-data: sign-data-bytes, ; sign-data is the data to be signed by offline signer, currently it can be unsigned transaction or typed data
 *     ?data-type: sign-data-type,
 *     ?chain-id: int .default 1,
 *     derivation-path: #5.304(crypto-keypath), ;the key path for signing this request
 *     ?request-id: uuid, ; the uuid for this signing request
 *     ?address: eth-address-bytes,            ;verification purpose for the address of the signing key
 *     ?origin: text  ;the origin of this sign request, like wallet name
 * )
 * request-id = 1
 * sign-data = 2
 * data-type = 3
 * chain-id = 4 ;it will be the chain id of ethereum related blockchain
 * derivation-path = 5
 * address = 6
 * origin = 7
 * eth-address-bytes = bytes .size 20
 * sign-data-bytes = bytes ; for unsigned transactions it will be the rlp encoding for unsigned transaction data and ERC 712 typed data it will be the bytes of json string.
 * 
 * 
 * Important notes regarding incorrect CCDL description:
 * - data-type is NOT TAGGED with #3.401 but sent as an integer by Metamask and Rabby.
 * - data-type and chain-id are OPTIONAL fields on which we have defined the default values of 1 as specified in the CDDL description.
 */

enum class DataType {
    EthTransactionData = 1,
    EthTypedData = 2,
    EthRawBytes = 3,
    EthTypedTransaction = 4
};

class EthSignRequest: public RequestBase
{
public:
    EthSignRequest();
    ~EthSignRequest() override = default;

    enum class Key {
        REQUEST_ID = 1,
        SIGN_DATA,
        DATA_TYPE,
        CHAIN_ID,
        DERIVATION_PATH,
        ADDRESS,
        ORIGIN
    };

    size_t getMinMapLength() const override {
        return MIN_MAP_LENGTH;
    }

    size_t getMaxMapLength() const override {
        return MAX_MAP_LENGTH;
    }

    DataType getDataType() const { return m_data_type; };
    void setDataType(const DataType dataType) { m_data_type = dataType; };
    uint32_t getChainID() const { return m_chain_id; };
    void setChainId(const uint32_t chainId) { m_chain_id = chainId; };
    const std::optional<EthAddress>& getAddress() const { return m_address; };
    void setAddress(const std::optional<EthAddress>& address) { m_address = address; };
    
    size_t getMapSize() const override {
        throw CborException("Get Map size not supported", CborErrorUnimplementedValidation);
    }
    void toMap(CborEncoder*) const override {
        throw CborException("Encoder not supported", CborErrorUnimplementedValidation);
    }
    void fromMap(CborValue* map) override;
    std::string getAddressStr() const override;
    
    void clearAllItems();

private:
    static constexpr size_t MIN_MAP_LENGTH = 2;
    static constexpr size_t MAX_MAP_LENGTH = 7;

    DataType m_data_type;
    uint32_t m_chain_id;
    std::optional<EthAddress> m_address;
};
