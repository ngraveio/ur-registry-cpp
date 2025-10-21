#include "display_ur_fields.h"
#include "../common/utils.h"

void displayIntent(const Intent& req) {
    std::cout << "[intent]\n";
    const auto& subst = req.getSubtituteFields();

    std::cout << "  Prefix          : " << subst.prefix_str << "\n";
    std::cout << "  Fields          : [";


    for (size_t i = 0; i < subst.fields_array.size(); i++) {
        const auto& [key, value] = subst.fields_array[i];
        std::cout << "(" << key << ", " << value << ")";
    }

    std::cout << "]\n";
}

void displaySignRequest(const SignRequest& req) {
    std::cout << "[sign-request]\n";
    if (req.getRequestID().has_value()) {
        std::cout << "  Request ID      : " << req.getRequestID()->str() << "\n";
    }
    std::cout << " Coin Identity: ";
    displayCoinIdentity(req.getCoinIdentity());
    if (req.getDerivationPath().has_value()) {
        std::cout << " Derivation path: ";
        displayKeypath(req.getDerivationPath().value());
    }
    std::cout << "  Sign Data       : " << vectorToHex(req.getSignData()) << "\n";
    if (req.getOrigin().has_value()) {
        std::cout << "  Origin          : " << req.getOrigin().value() << "\n";
    }
    if (req.getTxtype().has_value()) {
        std::cout << "  TX type         : " << req.getTxtype().value() << "\n";
    }
    std::cout << "  Address         : " << req.getAddressStr() << "\n";
    if (req.getIntent().has_value()) {
        std::cout << " Intent: ";
        displayIntent(req.getIntent().value());
    }
}

void displayEthSignRequest(const EthSignRequest& req) {
    std::cout << "[eth-sign-request]\n";
    if (req.getRequestID().has_value()) {
        std::cout << "  Request ID      : " << req.getRequestID()->str() << "\n";
    }
    std::cout << "  Sign Data       : " << vectorToHex(req.getSignData()) << "\n";
    std::cout << "  Data Type       : " << static_cast<int>(req.getDataType()) << "\n";
    std::cout << "  Chain ID        : " << req.getChainID() << "\n";
    if (req.getDerivationPath().has_value()) {
        std::cout << " Derivation path: ";
        displayKeypath(req.getDerivationPath().value());
    }
    if (req.getAddress().has_value()) {
        std::cout << "  Address         : " << arrayToHex(req.getAddress().value()) << "\n";
    }
    if (req.getOrigin().has_value()) {
        std::cout << "  Origin          : " << req.getOrigin().value() << "\n";
    }
}

void displayBatchSignRequest(const BatchSignRequest& req) {
    std::cout << "[batch-sign-request]\n";
    const auto& signRequests = req.getSignRequests();

    for (size_t i = 0; i < signRequests.size(); ++i) {
        std::cout << "[batch-sign-request][" << i << "]: ";
        displaySignRequest(signRequests[i]);
        std::cout << std::endl;
    }
}

void displayCoinIdentity(const CoinIdentity& req) {
    std::cout << "[coin-identity]\n";
    std::cout << "  Coin ID         : " << req.toUaiStr() << "\n";
}

void displayKeypath(const Keypath& req) {
    std::cout << "[keypath]\n";
    std::cout << "  Derivation path : " << req.getDerivationPath() << "\n";
}
