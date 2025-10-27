#include "encode_ur_types.h"

CoinIdentity encodeCoinIdentity() {
    CoinIdentity req;

    std::string input("");

    std::cout << "UAI (e.g. uai://secp256k1.60.1.test.0x010203):\n";
    std::getline(std::cin, input);
    req.setCoinIdentity(input);

    return req;
}

Keypath encodeKeypath() {
    Keypath req;

    std::string input("");
    uint32_t value(0);

    std::cout << "m/1'/2/[3,4]/[5,6]'/[]/[]'/[7,8']/[9',0] format:\n";
    std::getline(std::cin, input);
    req.setDerivationPath(input);

    std::cout << "Source fingerprint (uint32):\n";
    std::getline(std::cin, input);
    if (!input.empty()) {
        std::istringstream iss(input);
        if (iss >> value) {
            req.setSourceFingerprint(value);
        } else {
            std::cerr << "Invalid input. Skipping setting fingerprint.\n";
        }
    } 

    return req;
}

CoinInfo encodeCoinInfo() {
    CoinInfo req;
    std::string input("");
    uint8_t value(0);

    // Prompt for BIP44 Coin Type (optional)
    std::cout << "BIP44 (e.g. 0 for BTC or 60 for ETH, or press Enter to skip): ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        std::istringstream iss(input);
        if (iss >> value) {
            req.setType(static_cast<CoinType>(value));
        } else {
            std::cerr << "Invalid input. Skipping setting BIP44 type.\n";
        }
    } 

    // Prompt for Network Type (optional)
    std::cout << "Network (e.g. 0 for Mainnet, 1 for BTC Testnet, or press Enter to skip): ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        std::istringstream iss(input);
        if (iss >> value) {
            req.setNetwork(static_cast<NetworkType>(value));
        } else {
            std::cerr << "Invalid input. Skipping setting network.\n";
        }
    }

    return req;
}

HDKey encodeHDKey() {
    HDKey req;

    std::string input("");

    std::cout << "Deserialize extended key (e.g. xpub6CRQif2S43vtEYf5cZdMhrFpuBFkgFkALM6qhJZz7ws2cfEf1f8Jiv8dXSkizobckHyfH1mFDFZn46AJoh8d4FpB6ydFFg49yPkJF69GsHq):\n";
    std::getline(std::cin, input);
    req.deserializeFromBIP32(input);

    std::cout << "Coin Info: ";
    CoinInfo useInfo = encodeCoinInfo();
    req.setUseInfo(useInfo);

    std::cout << "Origin (e.g. m/44'/0'/0'): ";
    Keypath origin = encodeKeypath();
    req.setOrigin(origin);

    std::cout << "Children (e.g. m/0/1): ";
    Keypath children = encodeKeypath();
    req.setChildren(children);

    std::cout << "Name (any string, leave empty to skip):\n";
    std::getline(std::cin, input);
    if (!input.empty()) req.setName(input);

    std::cout << "Note (any string, leave empty to skip):\n";
    std::getline(std::cin, input);
    if (!input.empty()) req.setNote(input);

    return req;
}

OutputDescriptor encodeOutputDescriptor() {
    OutputDescriptor req;

    std::string input("");

    std::cout << "Source (e.g. (pk(@0))):\n";
    std::getline(std::cin, input);
    req.setSource(input);

    std::cout << "HDkey: ";
    HDKey hdkey = encodeHDKey();
    req.setKeys({hdkey});

    return req;
}

DetailedAccount encodeDetailedAccount() {
    DetailedAccount req;

    std::string input("");
    std::cout << "Select main account type (hdkey/output-descriptor):\n";
    std::getline(std::cin, input);

    if (input == "hdkey") {
        HDKey hdkey = encodeHDKey();
        req.setAccount(hdkey);
    } else if (input == "output-descriptor") {
        OutputDescriptor desc = encodeOutputDescriptor();
        req.setAccount(desc);
    } else {
        std::cerr << "Invalid selection. Must be 'hdkey' or 'output-descriptor'.\n";
        return req;
    }

    uint16_t i(0);
    while (true) {
        std::string tokenId("");
        std::cout << "Add token ID n" << i << " (any string, press Enter to finish):\n";
        std::getline(std::cin, tokenId);

        if (tokenId.empty()) {
            break;
        } else {
            req.addTokenId(tokenId);
            i++;
        }
    }
    return req;
}

PortfolioCoin encodePortfolioCoin() {
    PortfolioCoin req;

    std::string input("");
    uint32_t value(0);

    std::cout << "Coin Identity: ";
    CoinIdentity uai = encodeCoinIdentity();

    std::cout << "Master fingerprint (uint32):\n";
    std::getline(std::cin, input);
    if (!input.empty()) {
        std::istringstream iss(input);
        if (iss >> value) {
            req.setMasterFingerprint(value);
        } else {
            std::cerr << "Invalid input. Skipping setting fingerprint.\n";
        }
    } 

    uint16_t i(0);
    while (true) {
        std::string selection("");
        std::cout << "Add account n" << i << " (press 'y' to add or Enter to finish):\n";
        std::getline(std::cin, selection);
        if (selection == "y") {
            
            DetailedAccount account = encodeDetailedAccount();
            req.addAccount(account);
            i++;
        } else if (selection.empty()) {
            break;
        } else {
            std::cout << "Unrecognized selection, press 'y' to add or enter to finish";
        }
    }

    return req;
}

PortfolioMetadata encodePortfolioMetadata() {
    PortfolioMetadata req;

    std::string input("");

    std::cout << "Sync ID (16 bytes in hex without 0x prefix, e.g. 1234567890abcdef1234567890abcdef, leave empty to skip):\n";
    std::getline(std::cin, input);
    if (!input.empty()) req.setSyncId(fromHex(input));

    std::cout << "Language (e.g. en, leave empty to skip):\n";
    std::getline(std::cin, input);
    if (!input.empty()) req.setLanguage(input);

    std::cout << "Firmware version (e.g. 1.0.0, leave empty to skip):\n";
    std::getline(std::cin, input);
    if (!input.empty()) req.setFwVersion(input);

    std::cout << "Device name (e.g. NGRAVE ZERO, leave empty to skip):\n";
    std::getline(std::cin, input);
    if (!input.empty()) req.setDevice(input);

    return req;
}

Portfolio encodePortfolio() {
    Portfolio req;

    std::cout << "Metadata: ";
    PortfolioMetadata metadata = encodePortfolioMetadata();
    req.setMetadata(metadata);

    uint16_t i(0);
    while (true) {
        std::string selection("");
        std::cout << "Add coin n" << i << " (press 'y' to add or Enter to finish):\n";
        std::getline(std::cin, selection);
        if (selection == "y") {
            PortfolioCoin coin = encodePortfolioCoin();
            req.addCoin(coin);
            i++;
        } else if (selection.empty()) {
            break;
        } else {
            std::cout << "Unrecognized selection, press 'y' to add or enter to finish";
        }
    }

    return req;
}

template<typename T>
T encodeGenericSignature() {
    T req;

    std::string input("");
    std::cout << "Enter Request ID (UUID format, e.g. 9b1deb4d-3b7d-4bad-9bdd-2b0d7b3dcb6d, leave empty to skip):\n";
    std::getline(std::cin, input);
    if (!input.empty()) {
        Uuid uuid(input);
        req.setRequestID(uuid);
    }

    std::cout << "Enter signature (hex without 0x prefix): ";
    std::getline(std::cin, input);
    req.setSignature(fromHex(input));

    std::cout << "Enter origin (any string, leave empty to skip): ";
    std::getline(std::cin, input);
    if (!input.empty()) req.setOrigin(input);
    
    return req;
}

SignResponse encodeSignResponse() {
    SignResponse req = encodeGenericSignature<SignResponse>();

    std::string input("");
    std::cout << "Enter public key (hex without 0x prefix, leave empty to skip): ";
    std::getline(std::cin, input);
    if (!input.empty()) req.setPublickey(fromHex(input));

    return req;
}

EthSignature encodeEthSignature() {
    return encodeGenericSignature<EthSignature>();
}

