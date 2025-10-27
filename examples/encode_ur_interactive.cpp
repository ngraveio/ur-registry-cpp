#include <iostream>
#include <string>

#include "encode_ur_types.h"

using URType = std::variant<CoinIdentity, Keypath, CoinInfo, HDKey, OutputDescriptor, DetailedAccount, PortfolioCoin, PortfolioMetadata, Portfolio, SignResponse, EthSignature>;
using EncodeFn = std::function<URType()>;

std::unordered_map<std::string, EncodeFn> ur_encoder_map = {
    {"coin-identity", encodeCoinIdentity},
    {"keypath", encodeKeypath},
    {"crypto-keypath", encodeKeypath},
    {"coininfo", encodeCoinInfo},
    {"crypto-coin-info", encodeCoinInfo},
    {"hdkey", encodeHDKey},
    {"crypto-hdkey", encodeHDKey},
    {"output-descriptor", encodeOutputDescriptor},
    {"crypto-output", encodeOutputDescriptor},
    {"detailed-account", encodeDetailedAccount},
    {"portfolio-coin", encodePortfolioCoin},
    {"portfolio-metadata", encodePortfolioMetadata},
    {"portfolio", encodePortfolio},
    {"sign-response", encodeSignResponse},
    {"eth-signature", encodeEthSignature}
};

int main() {
    std::string input;

    while (true) {
        std::cout << "Enter UR type to encode or 'q' to quit: ";
        std::getline(std::cin, input);

        if (input == "q") break;
        auto it = ur_encoder_map.find(input);
        if (it != ur_encoder_map.end()) {
            try {
                URType result = it->second(); // call encoder

                std::visit([](auto&& value) {
                    std::cout << "Encoded UR: " << value.toSinglePartUr() << "\n";
                }, result);
            } catch (const std::exception& e) {
                std::cerr << "Failed to encode UR type '" << input << "': " << e.what() << "\n";
            }
        } else {
            std::cout << "Unsupported type\n";
        }

        std::cout << "\n";
    }

    return 0;
}