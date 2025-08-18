#include "output_descriptor_function.h"

const std::unordered_map<DescriptorFunction, std::string> OutputDescriptorFunction::scriptNames = {
    {DescriptorFunction::PK, "pk"},
    {DescriptorFunction::PKH, "pkh"},
    {DescriptorFunction::WPKH, "wpkh"},
    {DescriptorFunction::SH, "sh"},
    {DescriptorFunction::WSH, "wsh"},
    {DescriptorFunction::TR, "tr"},
    {DescriptorFunction::Multi, "multi"},
    {DescriptorFunction::SortedMulti, "sortedmulti"},
    {DescriptorFunction::MultiA, "multi_a"},
    {DescriptorFunction::SortedMultiA, "sortedmulti_a"},
    {DescriptorFunction::Addr, "addr"},
    {DescriptorFunction::Raw, "raw"},
    {DescriptorFunction::Cosigner, "cosigner"}
};

std::string OutputDescriptorFunction::getFunctionName(const DescriptorFunction& script) {
    auto it = scriptNames.find(script);
    if (it != scriptNames.end()) {
        return it->second;
    }
    return "Unknown Function";
};

DescriptorFunction OutputDescriptorFunction::getFunctionType(const std::string& script) {
    for (const auto& pair : scriptNames) {
        if (pair.second == script) { // cppcheck-suppress useStlAlgorithm
            return pair.first;
        }
    }
    return DescriptorFunction::Unknown;
};