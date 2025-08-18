#pragma once

#include <string>
#include <unordered_map>

/**
 * @brief Supported output descriptors
 * Source: https://github.com/bitcoin/bitcoin/blob/master/doc/descriptors.md
 * 
 */
enum class DescriptorFunction {
    PK, // Pay-to-pubkey scripts (P2PK) 
    PKH, // Pay-to-pubkey-hash scripts (P2PKH) - Legacy address - Purpose: 44h
    WPKH, // Pay-to-witness-pubkey-hash scripts (P2WPKH) - Native Segwit address - Purpose: 84h
    SH, // Pay-to-script-hash scripts (P2SH) - Purpose: 49h
    WSH, // Pay-to-witness-script-hash scripts (P2WSH) - Purpose: 49h
    TR, // Pay-to-taproot outputs (P2TR) - Taproot address - Purpose: 86h
    Multi, // Multisig scripts - Purpose: 48h
    SortedMulti, // Multisig scripts where the public keys are sorted lexicographically
    MultiA, // Multisig scripts inside taproot script trees
    SortedMultiA, // Sorted multisig scripts inside taproot script trees
    Addr, // Any type of supported address 
    Raw, // Raw hex scripts
    Cosigner, // Function introduced in https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-2023-010-output-descriptor.md
    Unknown
};


class OutputDescriptorFunction {

public:
    static std::string getFunctionName(const DescriptorFunction& script);

    static DescriptorFunction getFunctionType(const std::string& script);

private:
    static const std::unordered_map<DescriptorFunction, std::string> scriptNames;
};
