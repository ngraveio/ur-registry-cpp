#include <string>
#include <regex>

#include "../common/utils.h"
#include "display_ur_fields.h"
#include "demo_urs.h"

template <typename T>
T decode(const std::string &ur)
{
    T obj;
    obj.fromUr(ur);
    return obj;
}

struct URHandler
{
    std::function<void(const std::string &)> run;
};

const std::unordered_map<std::string, URHandler> ur_dispatch = {
    {"eth-sign-request", {[](const std::string &ur)
                          {
                              auto obj = decode<EthSignRequest>(ur);
                              displayEthSignRequest(obj);
                          }}},
    {"sign-request", {[](const std::string &ur)
                      {
                          auto obj = decode<SignRequest>(ur);
                          displaySignRequest(obj);
                      }}},
    {"batch-sign-request", {[](const std::string &ur)
                            {
                                auto obj = decode<BatchSignRequest>(ur);
                                displayBatchSignRequest(obj);
                            }}},
    {"coin-identity", {[](const std::string &ur)
                       {
                           auto obj = decode<CoinIdentity>(ur);
                           displayCoinIdentity(obj);
                       }}},
    {"keypath", {[](const std::string &ur)
                 {
                     auto obj = decode<Keypath>(ur);
                     displayKeypath(obj);
                 }}},
    {"crypto-keypath", {[](const std::string &ur)
                        {
                            auto obj = decode<Keypath>(ur);
                            displayKeypath(obj);
                        }}},
    {"intent", {[](const std::string &ur)
                {
                    auto obj = decode<Intent>(ur);
                    displayIntent(obj);
                }}}};

void decodeGeneric(const std::string &ur)
{
    std::regex type_regex(R"(ur:([^/]+))");
    std::smatch match;
    if ((std::regex_search(ur, match, type_regex)))
    {
        std::string ur_type = match[1];

        auto it = ur_dispatch.find(ur_type);
        if (it != ur_dispatch.end())
        {
            try
            {
                it->second.run(ur);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Failed to decode UR type '" << ur_type << "': " << e.what() << "\n";
            }
        }
        else
        {
            std::cerr << "Unsupported UR type: " << ur_type << "\n";
        }
    }
}

int main()
{
    std::cout << "UR Registry Decoder\n";
    std::cout << "Paste a UR below (or 'demo' to decode pre-defined UR types, or 'q' to quit):\n\n";

    std::string input;
    while (true)
    {
        std::cout << "UR> ";
        std::getline(std::cin, input);
        if (input.empty())
            continue;
        if (input == "demo")
        {
            for (const auto &ur : DEMO_URS)
            {
                std::cout << "Decode example: " << ur << "\n\n";
                decodeGeneric(ur);
                std::cout << "\n\n";
            }
            continue;
        }
        if (input == "q")
            break;

        decodeGeneric(input);
        std::cout << "\n";
    }

    return 0;
}
