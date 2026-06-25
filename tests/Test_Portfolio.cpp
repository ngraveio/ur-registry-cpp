#include "Test_Portfolio.h"
#include "Test_UrHelpers.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestPortfolio);

void TestPortfolio::setUp()
{
}

void TestPortfolio::tearDown()
{
}

std::vector<uint8_t> TestPortfolio::createSyncId()
{
    // Create a 16-byte sync ID
    return std::vector<uint8_t>{
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
}

PortfolioMetadata TestPortfolio::createMetadata()
{
    PortfolioMetadata metadata;
    metadata.setSyncId(createSyncId());
    metadata.setLanguage("en");
    metadata.setFwVersion("1.7-2.rc");
    metadata.setDevice("NGRAVE");
    return metadata;
}

PortfolioCoin TestPortfolio::createBitcoinCoin()
{
    // Create a Bitcoin coin identity (Secp256k1, BIP-0044 index 0)
    CoinIdentity coinId(EllipticCurve::Secp256k1, 0);

    // Create an HDKey for the account
    HDKey hdkey;
    hdkey.setKeyData({0x02, 0x5a, 0x46, 0x3b, 0xab, 0x5b, 0x8a, 0x5a, 0xc3, 0x57, 0x83, 0xb0, 0x44, 0xe4, 0x2f, 0x25, 0x52, 0x3b, 0x38, 0x5c, 0x2a, 0x9c, 0x49, 0x0e, 0x9f, 0x31, 0x10, 0x51, 0x32, 0x2d, 0x9c, 0xf7, 0x2d});
    hdkey.setChainCode({0x0f, 0x05, 0x51, 0xa9, 0x46, 0x32, 0x0b, 0x97, 0x22, 0x13, 0x24, 0x55, 0x0a, 0xae, 0x42, 0xb2, 0x23, 0x53, 0x58, 0xc4, 0x2f, 0xaa, 0x6a, 0x76, 0x86, 0xc9, 0x06, 0x4e, 0x9a, 0x0e, 0x2c, 0xf5});
    hdkey.setUseInfo(CoinType::Bitcoin);
    hdkey.setOrigin(Keypath("m/44'/0'/0'"));
    hdkey.setParentFingerprint(0x12345678);
    hdkey.setName("Bitcoin Account");
    hdkey.setNote("Primary Bitcoin Account");

    DetailedAccount account(hdkey);
    std::vector<DetailedAccount> accounts = {account};

    return PortfolioCoin(coinId, accounts);
}

PortfolioCoin TestPortfolio::createEthereumCoin()
{
    // Create an Ethereum coin identity (Secp256k1, BIP-0044 index 60)
    CoinIdentity coinId(EllipticCurve::Secp256k1, 60);

    // Create an HDKey for the account
    HDKey hdkey;
    hdkey.setKeyData({0x03, 0x5a, 0x46, 0x3b, 0xab, 0x5b, 0x8a, 0x5a, 0xc3, 0x57, 0x83, 0xb0, 0x44, 0xe4, 0x2f, 0x25, 0x52, 0x3b, 0x38, 0x5c, 0x2a, 0x9c, 0x49, 0x0e, 0x9f, 0x31, 0x10, 0x51, 0x32, 0x2d, 0x9c, 0xf7, 0x2d});
    hdkey.setChainCode({0x0f, 0x05, 0x51, 0xa9, 0x46, 0x32, 0x0b, 0x97, 0x22, 0x13, 0x24, 0x55, 0x0a, 0xae, 0x42, 0xb2, 0x23, 0x53, 0x58, 0xc4, 0x2f, 0xaa, 0x6a, 0x76, 0x86, 0xc9, 0x06, 0x4e, 0x9a, 0x0e, 0x2c, 0xf5});
    hdkey.setUseInfo(CoinType::Ethereum);
    hdkey.setOrigin(Keypath("m/44'/60'/0'"));
    hdkey.setParentFingerprint(0x87654321);
    hdkey.setName("Ethereum Account");
    hdkey.setNote("Primary Ethereum Account");

    DetailedAccount account(hdkey);
    std::vector<DetailedAccount> accounts = {account};

    return PortfolioCoin(coinId, accounts);
}

void TestPortfolio::EncodeWithCoinsOnly()
{
    // Create a Portfolio with a single coin and no metadata
    PortfolioCoin bitcoinCoin = createBitcoinCoin();
    std::vector<PortfolioCoin> coins = {bitcoinCoin};

    Portfolio portfolio(coins);

    // Expected CBOR format:
    // {
    //   1: [#6.41403(portfolio-coin)]  // coins array with one coin
    // }
    // Expected CBOR bytes:
    // A1                                      # map(1)
    //    01                                   # unsigned(1)
    //    81                                   # array(1)
    //       D9 A1BB                           # tag(41403)
    //          A2                             # map(2)
    //             01                          # unsigned(1)
    //             D9 A1B9                     # tag(41401)
    //                A2                       # map(2)
    //                   01                    # unsigned(1)
    //                   08                    # unsigned(8)
    //                   02                    # unsigned(2)
    //                   00                    # unsigned(0)
    //             02                          # unsigned(2)
    //             81                          # array(1)
    //                D9 A1BA                  # tag(41402)
    //                   A1                    # map(1)
    //                      01                 # unsigned(1)
    //                      D9 9D6F            # tag(40303)
    //                         A7              # map(7)
    //                            03           # unsigned(3)
    //                            58 21        # bytes(33)
    //                               025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D # "\u0002ZF;\xAB[\x8AZ\xC3W\x83\xB0D\xE4/%R;8\\*\x9CI\u000E\x9F1\u0010Q2-\x9C\xF7-"
    //                            04           # unsigned(4)
    //                            58 20        # bytes(32)
    //                               0F0551A946320B97221324550AAE42B2235358C42FAA6A7686C9064E9A0E2CF5 # "\u000F\u0005Q\xA9F2\v\x97\"\u0013$U\n\xAEB\xB2#SX\xC4/\xAAjv\x86\xC9\u0006N\x9A\u000E,\xF5"
    //                            05           # unsigned(5)
    //                            D9 9D71      # tag(40305)
    //                               A1        # map(1)
    //                                  01     # unsigned(1)
    //                                  00     # unsigned(0)
    //                            06           # unsigned(6)
    //                            D9 9D70      # tag(40304)
    //                               A1        # map(1)
    //                                  01     # unsigned(1)
    //                                  86     # array(6)
    //                                     18 2C # unsigned(44)
    //                                     F5  # primitive(21)
    //                                     00  # unsigned(0)
    //                                     F5  # primitive(21)
    //                                     00  # unsigned(0)
    //                                     F5  # primitive(21)
    //                            08           # unsigned(8)
    //                            1A 12345678  # unsigned(305419896)
    //                            09           # unsigned(9)
    //                            6F           # text(15)
    //                               426974636F696E204163636F756E74 # "Bitcoin Account"
    //                            0A           # unsigned(10)
    //                            77           # text(23)
    //                               5072696D61727920426974636F696E204163636F756E74 # "Primary Bitcoin Account"

    auto expectedCbor = fromHex("A10181D9A1BBA201D9A1B9A2010802000281D9A1BAA101D99D6FA7035821025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D0458200F0551A946320B97221324550AAE42B2235358C42FAA6A7686C9064E9A0E2CF505D99D71A1010006D99D70A10186182CF500F500F5081A12345678096F426974636F696E204163636F756E740A775072696D61727920426974636F696E204163636F756E74");
    auto expectedUR = std::string("ur:portfolio/oyadlytaoyrkoeadtaoyrhoeadayaoaeaolytaoyrdoyadtantjlosaxhdclaohtfgfrpyhplehtsrhglspffyvedldagmfrethhdrnsgabaneehbegyeydpnsyldpaahdcxbsahgyptfgeybdmscpbwdkgobkplfwprcnguhdssdlpkimkolnsoamglnybadwykahtantjsoyadaeamtantjooyadlncsdwykaeykaeykaycybgeehfksasjlfwinjyiajlinjtcxfpiaiajlkpjtjybkktgdjpinjnhsjpkkcxfwinjyiajlinjtcxfpiaiajlkpjtjydetbleee");

    ValidateCborResults(portfolio, expectedCbor);
    ValidateUrEncoding(portfolio, expectedUR);

    // Verify the portfolio has the expected number of coins and no metadata
    CPPUNIT_ASSERT(portfolio.getCoins().size() == 1);
    CPPUNIT_ASSERT(!portfolio.getMetadata().has_value());
}

void TestPortfolio::EncodeWithCoinsAndMetadata()
{
    // Create a Portfolio with a single coin and metadata
    PortfolioCoin bitcoinCoin = createBitcoinCoin();
    std::vector<PortfolioCoin> coins = {bitcoinCoin};
    PortfolioMetadata metadata = createMetadata();

    Portfolio portfolio(coins, metadata);

    // Expected CBOR format:
    // {
    //   1: [#6.41403(portfolio-coin)],  // coins array with one coin
    //   2: #6.41404(portfolio-metadata) // metadata
    // }
    // Expected CBOR bytes:
    // A2                                      # map(2)
    //    01                                   # unsigned(1)
    //    81                                   # array(1)
    //       D9 A1BB                           # tag(41403)
    //          A2                             # map(2)
    //             01                          # unsigned(1)
    //             D9 A1B9                     # tag(41401)
    //                A2                       # map(2)
    //                   01                    # unsigned(1)
    //                   08                    # unsigned(8)
    //                   02                    # unsigned(2)
    //                   00                    # unsigned(0)
    //             02                          # unsigned(2)
    //             81                          # array(1)
    //                D9 A1BA                  # tag(41402)
    //                   A1                    # map(1)
    //                      01                 # unsigned(1)
    //                      D9 9D6F            # tag(40303)
    //                         A7              # map(7)
    //                            03           # unsigned(3)
    //                            58 21        # bytes(33)
    //                               025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D # "\u0002ZF;\xAB[\x8AZ\xC3W\x83\xB0D\xE4/%R;8\\*\x9CI\u000E\x9F1\u0010Q2-\x9C\xF7-"
    //                            04           # unsigned(4)
    //                            58 20        # bytes(32)
    //                               0F0551A946320B97221324550AAE42B2235358C42FAA6A7686C9064E9A0E2CF5 # "\u000F\u0005Q\xA9F2\v\x97\"\u0013$U\n\xAEB\xB2#SX\xC4/\xAAjv\x86\xC9\u0006N\x9A\u000E,\xF5"
    //                            05           # unsigned(5)
    //                            D9 9D71      # tag(40305)
    //                               A1        # map(1)
    //                                  01     # unsigned(1)
    //                                  00     # unsigned(0)
    //                            06           # unsigned(6)
    //                            D9 9D70      # tag(40304)
    //                               A1        # map(1)
    //                                  01     # unsigned(1)
    //                                  86     # array(6)
    //                                     18 2C # unsigned(44)
    //                                     F5  # primitive(21)
    //                                     00  # unsigned(0)
    //                                     F5  # primitive(21)
    //                                     00  # unsigned(0)
    //                                     F5  # primitive(21)
    //                            08           # unsigned(8)
    //                            1A 12345678  # unsigned(305419896)
    //                            09           # unsigned(9)
    //                            6F           # text(15)
    //                               426974636F696E204163636F756E74 # "Bitcoin Account"
    //                            0A           # unsigned(10)
    //                            77           # text(23)
    //                               5072696D61727920426974636F696E204163636F756E74 # "Primary Bitcoin Account"
    //    02                                   # unsigned(2)
    //    D9 A1BC                              # tag(41404)
    //       A4                                # map(4)
    //          01                             # unsigned(1)
    //          50                             # bytes(16)
    //             0102030405060708090A0B0C0D0E0F10 # "\u0001\u0002\u0003\u0004\u0005\u0006\u0007\b\t\n\u000b\f\r\u000e\u000f\u0010"
    //          02                             # unsigned(2)
    //          62                             # text(2)
    //             656E                        # "en"
    //          03                             # unsigned(3)
    //          68                             # text(8)
    //             312E372D322E7263            # "1.7-2.rc"
    //          04                             # unsigned(4)
    //          66                             # text(6)
    //             4E4752415645                # "NGRAVE"

    auto expectedCbor = fromHex("A20181D9A1BBA201D9A1B9A2010802000281D9A1BAA101D99D6FA7035821025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D0458200F0551A946320B97221324550AAE42B2235358C42FAA6A7686C9064E9A0E2CF505D99D71A1010006D99D70A10186182CF500F500F5081A12345678096F426974636F696E204163636F756E740A775072696D61727920426974636F696E204163636F756E7402D9A1BCA401500102030405060708090A0B0C0D0E0F100262656E0368312E372D322E72630466"
                                "4E4752415645");
    auto expectedUR = std::string("ur:portfolio/oeadlytaoyrkoeadtaoyrhoeadayaoaeaolytaoyrdoyadtantjlosaxhdclaohtfgfrpyhplehtsrhglspffyvedldagmfrethhdrnsgabaneehbegyeydpnsyldpaahdcxbsahgyptfgeybdmscpbwdkgobkplfwprcnguhdssdlpkimkolnsoamglnybadwykahtantjsoyadaeamtantjooyadlncsdwykaeykaeykaycybgeehfksasjlfwinjyiajlinjtcxfpiaiajlkpjtjybkktgdjpinjnhsjpkkcxfwinjyiajlinjtcxfpiaiajlkpjtjyaotaoyrfoxadgdadaoaxaaahamatayasbkbdbnbtbabsbeaoidihjtaxisehdmemdpeydmjpiaaaiyglflgmfphffesntoceke");

    ValidateCborResults(portfolio, expectedCbor);
    ValidateUrEncoding(portfolio, expectedUR);

    // Verify the portfolio has the expected number of coins and metadata
    CPPUNIT_ASSERT(portfolio.getCoins().size() == 1);
    CPPUNIT_ASSERT(portfolio.getMetadata().has_value());
    CPPUNIT_ASSERT(portfolio.getMetadata().value().getLanguage() == "en");
    CPPUNIT_ASSERT(portfolio.getMetadata().value().getDevice() == "NGRAVE");
}

void TestPortfolio::EncodeWithMultipleCoins()
{
    // Create a Portfolio with multiple coins
    PortfolioCoin bitcoinCoin = createBitcoinCoin();
    PortfolioCoin ethereumCoin = createEthereumCoin();
    std::vector<PortfolioCoin> coins = {bitcoinCoin, ethereumCoin};

    Portfolio portfolio(coins);

    // Expected CBOR format:
    // {
    //   1: [#6.41403(portfolio-coin), #6.41403(portfolio-coin)]  // coins array with two coins
    // }
    //
    // A1                                      # map(1)
    //    01                                   # unsigned(1)
    //    82                                   # array(2)
    //       D9 A1BB                           # tag(41403)
    //          A2                             # map(2)
    //             01                          # unsigned(1)
    //             D9 A1B9                     # tag(41401)
    //                A2                       # map(2)
    //                   01                    # unsigned(1)
    //                   08                    # unsigned(8)
    //                   02                    # unsigned(2)
    //                   00                    # unsigned(0)
    //             02                          # unsigned(2)
    //             81                          # array(1)
    //                D9 A1BA                  # tag(41402)
    //                   A1                    # map(1)
    //                      01                 # unsigned(1)
    //                      D9 9D6F            # tag(40303)
    //                         A7              # map(7)
    //                            03           # unsigned(3)
    //                            58 21        # bytes(33)
    //                               025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D # "\u0002ZF;\xAB[\x8AZ\xC3W\x83\xB0D\xE4/%R;8\\*\x9CI\u000E\x9F1\u0010Q2-\x9C\xF7-"
    //                            04           # unsigned(4)
    //                            58 20        # bytes(32)
    //                               0F0551A946320B97221324550AAE42B2235358C42FAA6A7686C9064E9A0E2CF5 # "\u000F\u0005Q\xA9F2\v\x97\"\u0013$U\n\xAEB\xB2#SX\xC4/\xAAjv\x86\xC9\u0006N\x9A\u000E,\xF5"
    //                            05           # unsigned(5)
    //                            D9 9D71      # tag(40305)
    //                               A1        # map(1)
    //                                  01     # unsigned(1)
    //                                  00     # unsigned(0)
    //                            06           # unsigned(6)
    //                            D9 9D70      # tag(40304)
    //                               A1        # map(1)
    //                                  01     # unsigned(1)
    //                                  86     # array(6)
    //                                     18 2C # unsigned(44)
    //                                     F5  # primitive(21)
    //                                     00  # unsigned(0)
    //                                     F5  # primitive(21)
    //                                     00  # unsigned(0)
    //                                     F5  # primitive(21)
    //                            08           # unsigned(8)
    //                            1A 12345678  # unsigned(305419896)
    //                            09           # unsigned(9)
    //                            6F           # text(15)
    //                               426974636F696E204163636F756E74 # "Bitcoin Account"
    //                            0A           # unsigned(10)
    //                            77           # text(23)
    //                               5072696D61727920426974636F696E204163636F756E74 # "Primary Bitcoin Account"
    //       D9 A1BB                           # tag(41403)
    //          A2                             # map(2)
    //             01                          # unsigned(1)
    //             D9 A1B9                     # tag(41401)
    //                A2                       # map(2)
    //                   01                    # unsigned(1)
    //                   08                    # unsigned(8)
    //                   02                    # unsigned(2)
    //                   18 3C                 # unsigned(60)
    //             02                          # unsigned(2)
    //             81                          # array(1)
    //                D9 A1BA                  # tag(41402)
    //                   A1                    # map(1)
    //                      01                 # unsigned(1)
    //                      D9 9D6F            # tag(40303)
    //                         A7              # map(7)
    //                            03           # unsigned(3)
    //                            58 21        # bytes(33)
    //                               035A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D # "\u0003ZF;\xAB[\x8AZ\xC3W\x83\xB0D\xE4/%R;8\\*\x9CI\u000E\x9F1\u0010Q2-\x9C\xF7-"
    //                            04           # unsigned(4)
    //                            58 20        # bytes(32)
    //                               0F0551A946320B97221324550AAE42B2235358C42FAA6A7686C9064E9A0E2CF5 # "\u000F\u0005Q\xA9F2\v\x97\"\u0013$U\n\xAEB\xB2#SX\xC4/\xAAjv\x86\xC9\u0006N\x9A\u000E,\xF5"
    //                            05           # unsigned(5)
    //                            D9 9D71      # tag(40305)
    //                               A1        # map(1)
    //                                  01     # unsigned(1)
    //                                  18 3C  # unsigned(60)
    //                            06           # unsigned(6)
    //                            D9 9D70      # tag(40304)
    //                               A1        # map(1)
    //                                  01     # unsigned(1)
    //                                  86     # array(6)
    //                                     18 2C # unsigned(44)
    //                                     F5  # primitive(21)
    //                                     18 3C # unsigned(60)
    //                                     F5  # primitive(21)
    //                                     00  # unsigned(0)
    //                                     F5  # primitive(21)
    //                            08           # unsigned(8)
    //                            1A 87654321  # unsigned(2271560481)
    //                            09           # unsigned(9)
    //                            70           # text(16)
    //                               457468657265756D204163636F756E74 # "Ethereum Account"
    //                            0A           # unsigned(10)
    //                            78 18        # text(24)
    //                               5072696D61727920457468657265756D204163636F756E74 # "Primary Ethereum Account"

    auto expectedCbor = fromHex("A10182D9A1BBA201D9A1B9A2010802000281D9A1BAA101D99D6FA7035821025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D0458200F0551A946320B97221324550AAE42B2235358C42FAA6A7686C9064E9A0E2CF505D99D71A1010006D99D70A10186182CF500F500F5081A12345678096F426974636F696E204163636F756E740A775072696D61727920426974636F696E204163636F756E74D9A1BBA201D9A1B9A2010802183C0281D9A1BAA101D99D6FA7035821035A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D0458200F0551A946320B97221324550AAE42B2235358C42FAA6A7686C9064E9A0E2CF505D99D71A101183C06D99D70A10186182CF5183CF500F5081A876543210970457468657265756D204163636F756E740A78185072696D61727920457468657265756D204163636F756E74");
    auto expectedUR = std::string("ur:portfolio/oyadlftaoyrkoeadtaoyrhoeadayaoaeaolytaoyrdoyadtantjlosaxhdclaohtfgfrpyhplehtsrhglspffyvedldagmfrethhdrnsgabaneehbegyeydpnsyldpaahdcxbsahgyptfgeybdmscpbwdkgobkplfwprcnguhdssdlpkimkolnsoamglnybadwykahtantjsoyadaeamtantjooyadlncsdwykaeykaeykaycybgeehfksasjlfwinjyiajlinjtcxfpiaiajlkpjtjybkktgdjpinjnhsjpkkcxfwinjyiajlinjtcxfpiaiajlkpjtjytaoyrkoeadtaoyrhoeadayaocsfnaolytaoyrdoyadtantjlosaxhdclaxhtfgfrpyhplehtsrhglspffyvedldagmfrethhdrnsgabaneehbegyeydpnsyldpaahdcxbsahgyptfgeybdmscpbwdkgobkplfwprcnguhdssdlpkimkolnsoamglnybadwykahtantjsoyadcsfnamtantjooyadlncsdwykcsfnykaeykaycyltihfxclasjofejyisihjpihkpjncxfpiaiajlkpjtjybkkscsgdjpinjnhsjpkkcxfejyisihjpihkpjncxfpiaiajlkpjtjywkvahhgw");

    ValidateCborResults(portfolio, expectedCbor);
    ValidateUrEncoding(portfolio, expectedUR);

    // Verify the portfolio has the expected number of coins
    CPPUNIT_ASSERT(portfolio.getCoins().size() == 2);
    CPPUNIT_ASSERT(!portfolio.getMetadata().has_value());
}

void TestPortfolio::EncodeEmpty()
{
    // Create an empty Portfolio and then add coins
    Portfolio portfolio;

    // Verify the portfolio is initially empty
    CPPUNIT_ASSERT(portfolio.getCoins().empty());
    CPPUNIT_ASSERT(!portfolio.getMetadata().has_value());

    // Add a coin
    PortfolioCoin bitcoinCoin = createBitcoinCoin();
    std::vector<PortfolioCoin> coins = {bitcoinCoin};
    portfolio.setCoins(coins);

    // Verify the portfolio now has one coin
    CPPUNIT_ASSERT(portfolio.getCoins().size() == 1);

    // Add metadata
    PortfolioMetadata metadata = createMetadata();
    portfolio.setMetadata(metadata);

    // Verify the portfolio now has metadata
    CPPUNIT_ASSERT(portfolio.getMetadata().has_value());

    // Expected CBOR format after adding coin and metadata:
    // {
    //   1: [#6.41403(portfolio-coin)],  // coins array with one coin
    //   2: #6.41404(portfolio-metadata) // metadata
    // }
    // Expected CBOR bytes:
    // A2                                      # map(2)
    //    01                                   # unsigned(1)
    //    81                                   # array(1)
    //       D9 A1BB                           # tag(41403)
    //          A2                             # map(2)
    //             01                          # unsigned(1)
    //             D9 A1B9                     # tag(41401)
    //                A2                       # map(2)
    //                   01                    # unsigned(1)
    //                   08                    # unsigned(8)
    //                   02                    # unsigned(2)
    //                   00                    # unsigned(0)
    //             02                          # unsigned(2)
    //             81                          # array(1)
    //                D9 A1BA                  # tag(41402)
    //                   A1                    # map(1)
    //                      01                 # unsigned(1)
    //                      D9 9D6F            # tag(40303)
    //                         A7              # map(7)
    //                            03           # unsigned(3)
    //                            58 21        # bytes(33)
    //                               025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D # "\u0002ZF;\xAB[\x8AZ\xC3W\x83\xB0D\xE4/%R;8\\*\x9CI\u000E\x9F1\u0010Q2-\x9C\xF7-"
    //                            04           # unsigned(4)
    //                            58 20        # bytes(32)
    //                               0F0551A946320B97221324550AAE42B2235358C42FAA6A7686C9064E9A0E2CF5 # "\u000F\u0005Q\xA9F2\v\x97\"\u0013$U\n\xAEB\xB2#SX\xC4/\xAAjv\x86\xC9\u0006N\x9A\u000E,\xF5"
    //                            05           # unsigned(5)
    //                            D9 9D71      # tag(40305)
    //                               A1        # map(1)
    //                                  01     # unsigned(1)
    //                                  00     # unsigned(0)
    //                            06           # unsigned(6)
    //                            D9 9D70      # tag(40304)
    //                               A1        # map(1)
    //                                  01     # unsigned(1)
    //                                  86     # array(6)
    //                                     18 2C # unsigned(44)
    //                                     F5  # primitive(21)
    //                                     00  # unsigned(0)
    //                                     F5  # primitive(21)
    //                                     00  # unsigned(0)
    //                                     F5  # primitive(21)
    //                            08           # unsigned(8)
    //                            1A 12345678  # unsigned(305419896)
    //                            09           # unsigned(9)
    //                            6F           # text(15)
    //                               426974636F696E204163636F756E74 # "Bitcoin Account"
    //                            0A           # unsigned(10)
    //                            77           # text(23)
    //                               5072696D61727920426974636F696E204163636F756E74 # "Primary Bitcoin Account"
    //    02                                   # unsigned(2)
    //    D9 A1BC                              # tag(41404)
    //       A4                                # map(4)
    //          01                             # unsigned(1)
    //          50                             # bytes(16)
    //             0102030405060708090A0B0C0D0E0F10 # "\u0001\u0002\u0003\u0004\u0005\u0006\u0007\b\t\n\u000b\f\r\u000e\u000f\u0010"
    //          02                             # unsigned(2)
    //          62                             # text(2)
    //             656E                        # "en"
    //          03                             # unsigned(3)
    //          68                             # text(8)
    //             312E372D322E7263            # "1.7-2.rc"
    //          04                             # unsigned(4)
    //          66                             # text(6)
    //             4E4752415645                # "NGRAVE"

    auto expectedCbor = fromHex("A20181D9A1BBA201D9A1B9A2010802000281D9A1BAA101D99D6FA7035821025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D0458200F0551A946320B97221324550AAE42B2235358C42FAA6A7686C9064E9A0E2CF505D99D71A1010006D99D70A10186182CF500F500F5081A12345678096F426974636F696E204163636F756E740A775072696D61727920426974636F696E204163636F756E7402D9A1BCA401500102030405060708090A0B0C0D0E0F100262656E0368312E372D322E72630466"
                                "4E4752415645");
    auto expectedUR = std::string("ur:portfolio/oeadlytaoyrkoeadtaoyrhoeadayaoaeaolytaoyrdoyadtantjlosaxhdclaohtfgfrpyhplehtsrhglspffyvedldagmfrethhdrnsgabaneehbegyeydpnsyldpaahdcxbsahgyptfgeybdmscpbwdkgobkplfwprcnguhdssdlpkimkolnsoamglnybadwykahtantjsoyadaeamtantjooyadlncsdwykaeykaeykaycybgeehfksasjlfwinjyiajlinjtcxfpiaiajlkpjtjybkktgdjpinjnhsjpkkcxfwinjyiajlinjtcxfpiaiajlkpjtjyaotaoyrfoxadgdadaoaxaaahamatayasbkbdbnbtbabsbeaoidihjtaxisehdmemdpeydmjpiaaaiyglflgmfphffesntoceke");

    ValidateCborResults(portfolio, expectedCbor);
    ValidateUrEncoding(portfolio, expectedUR);
}

void TestPortfolio::EncodeUaiToPortfolio()
{
    Portfolio portfolio;
    // Create initial portfolio with POL coin
    CPPUNIT_ASSERT_NO_THROW(portfolio.createPortfolio("uai://secp256k1.60.137/44'/60'/0'/0/[0,1]?master_fingerprint=123456789", "xpub6CVDAP5Ae2wxTNoDXtFqBiwyWU13ejtf16LJXbwMXMmW8i8HdDpWdaC75ss8c1oAmsFFvHXvmJi5MCU1nJZUkvJ3ZsHkCzHRwczDheGWrp3"));
    // Add BTC Native Segwit to portfolio
    CPPUNIT_ASSERT_NO_THROW(portfolio.addCoin("uai://secp256k1.0@wpkh/84'/0'/0'?master_fingerprint=123456789", "zpub6rnoYH74yc8GSmDT98KUbV1UdzZ87ZSUaL1fBNBWUDnNAP4hkYDEQaTRbk2bVDUfwVedRyCBPkm7esgfxijuG61thMRffbEEaAdUuKo2nRQ"));
    // Add POL token to existing POL coin
    CPPUNIT_ASSERT_NO_THROW(portfolio.addCoin("uai://secp256k1.60.137:0x7ceb23fd6bc0add59e62ac25578270cff1b9f619/44'/60'/0'/0/0?master_fingerprint=123456789", "xpub6CVDAP5Ae2wxTNoDXtFqBiwyWU13ejtf16LJXbwMXMmW8i8HdDpWdaC75ss8c1oAmsFFvHXvmJi5MCU1nJZUkvJ3ZsHkCzHRwczDheGWrp3"));
    // Add SOL account 1 to portfolio
    CPPUNIT_ASSERT_NO_THROW(portfolio.addCoin("uai://ed25519.501/44h/508h/0h/0h/0h", "4137c16b966abac1df4cb893af421ea0be61a52266aa7b3aa993900f7139640f"));
    // Add SOL account 2 with token to existing SOL coin
    CPPUNIT_ASSERT_NO_THROW(portfolio.addCoin("uai://ed25519.501:Es9vMFrzaCERmJfrF4H2FYD4KCoNkY11McCe8BenwNYB/44h/508h/0h/0h/1h", "954768223bb94015350462bada20daeed7e25c4777f997b56c027746e4f8ac67"));
    // Add BTC Legacy to existing BTC coin
    CPPUNIT_ASSERT_NO_THROW(portfolio.addCoin("uai://secp256k1.0@pkh", "xpub6DRJw8xG7CWTgpJxG5KRcD9qFEzut1DhCHFdmXsZ3VJZBoUFAft9UohvmBWjqRpyV7aP9CW87523WKmsRXJu7Gsg5znuDCp2Sg8Mi6B3Bcc"));
    // Add POL coin from another seed (different master fingerprint)
    CPPUNIT_ASSERT_NO_THROW(portfolio.addCoin("uai://secp256k1.60.137/44'/60'/0'/0/[]?master_fingerprint=444444444", "xpub6CfWNaFcVGYvYXwgrr7viAS43PadEgGQUoB7PTxYnRPhAcomxXeDUUJK2iuXnrEadibcpdTBVmQ7vdsMq1zbr2EscwSCYKBWb2FXHGbtCeQ"));
    // Add POL token to existing POL coin of the first seed
    CPPUNIT_ASSERT_NO_THROW(portfolio.addCoin("uai://secp256k1.60.137:0x1bfd67037b42cf73acf2047067bd4f2c47d9bfd6/44'/60'/0'", "xpub6CVDAP5Ae2wxTNoDXtFqBiwyWU13ejtf16LJXbwMXMmW8i8HdDpWdaC75ss8c1oAmsFFvHXvmJi5MCU1nJZUkvJ3ZsHkCzHRwczDheGWrp3"));

    // Validate UAI encoding
    // {1: [41403({1: 41401({1: 8, 2: 60, 3: [137]}),
    //             2: [41402({1: 40303({3: h'032503D7DCA4FF0594F0404D56188542A18D8E0784443134C716178BC1819C3DD4', 4: h'1719EA8CADCA1BBC71BF8511AC3A487286B4D34A860007B8FD498F2732EB8991', 6: 40304({1: [44, true, 60, true, 0, true]}), 7: 40304({1: [0, false, [0, 1], false]}), 8: 1906394030}),
    //                        2: [h'7ceb23fd6bc0add59e62ac25578270cff1b9f619', h'1bfd67037b42cf73acf2047067bd4f2c47d9bfd6']})],
    //             3: 123456789}),
    //      41403({1: 41401({1: 8, 2: 0}),
    //             2: [41402({1: 40308({1: "wpkh(@0)", 2: [40303({3: h'032CD40B55A07B5B591B6CA10602619F181759065113A69A2573DD4ADA6B90DB32', 4: h'DF914FC2F5223B0C4A8B2B140BA1858160D3FDE02CF829A054CEC922F77FA0D1', 6: 40304({1: [84, true, 0, true, 0, true]}), 8: 3365065644})]})}),
    //                 41402({1: 40308({1: "pkh(@0)", 2: [40303({3: h'03D3EEAC2252BA2F99AC8D1261BEFC3DA34EBB7DC545A5FF444939C0C8F1ED3FEF', 4: h'3E96E998197419852921A8533E1673C01C6B0058A8CA8D25751A1CA4A3D1019C', 8: 4035450625})]})})],
    //             3: 123456789}),
    //      41403({1: 41401({1: 6, 2: 501}),
    //             2: [41402({1: 40303({3: h'004137C16B966ABAC1DF4CB893AF421EA0BE61A52266AA7B3AA993900F7139640F', 6: 40304({1: [44, true, 508, true, 0, true, 0, true, 0, true]})})}),
    //                 41402({1: 40303({3: h'00954768223BB94015350462BADA20DAEED7E25C4777F997B56C027746E4F8AC67', 6: 40304({1: [44, true, 508, true, 0, true, 0, true, 1, true]})}),
    //                        2: ["Es9vMFrzaCERmJfrF4H2FYD4KCoNkY11McCe8BenwNYB"]})]}),
    //      41403({1: 41401({1: 8, 2: 60, 3: [137]}),
    //             2: [41402({1: 40303({3: h'03C95BDE557F5E9B971E57DC0EA83D5FEF45AED46379558DEEB18D1D7E4AE8FBDD', 4: h'8B2E94720DDAFE556DE20F50E584BBE1011ED5676B647515A369234330EF7CD0', 6: 40304({1: [44, true, 60, true, 0, true]}), 7: 40304({1: [0, false, [], false]}), 8: 2311616109})})],
    //             3: 444444444})]
    // }
    auto expectedCbor = fromHex("A10184D9A1BBA301D9A1B9A3010802183C038118890281D9A1BAA201D99D6FA5035821032503D7DCA4FF0594F0404D56188542A18D8E0784443134C716178BC1819C3DD40458201719EA8CADCA1BBC71BF8511AC3A487286B4D34A860007B8FD498F2732EB899106D99D70A10186182CF5183CF500F507D99D70A1018400F4820001F4081A71A143AE0282547CEB23FD6BC0ADD59E62AC25578270CFF1B9F619541BFD67037B42CF73ACF2047067BD4F2C47D9BFD6031A075BCD15D9A1BBA301D9A1B9A2010802000282D9A1BAA101D99D74A2016877706B68284030290281D99D6FA4035821032CD40B55A07B5B591B6CA10602619F181759065113A69A2573DD4ADA6B90DB32045820DF914FC2F5223B0C4A8B2B140BA1858160D3FDE02CF829A054CEC922F77FA0D106D99D70A101861854F500F500F5081AC892D3ACD9A1BAA101D99D74A20167706B68284030290281D99D6FA303582103D3EEAC2252BA2F99AC8D1261BEFC3DA34EBB7DC545A5FF444939C0C8F1ED3FEF0458203E96E998197419852921A8533E1673C01C6B0058A8CA8D25751A1CA4A3D1019C081AF0881701031A075BCD15D9A1BBA201D9A1B9A20106021901F50282D9A1BAA101D99D6FA2035821004137C16B966ABAC1DF4CB893AF421EA0BE61A52266AA7B3AA993900F7139640F06D99D70A1018A182CF51901FCF500F500F500F5D9A1BAA201D99D6FA203582100954768223BB94015350462BADA20DAEED7E25C4777F997B56C027746E4F8AC6706D99D70A1018A182CF51901FCF500F500F501F50281782C457339764D46727A614345526D4A667246344832465944344B436F4E6B5931314D6343653842656E774E5942D9A1BBA301D9A1B9A3010802183C038118890281D9A1BAA101D99D6FA503582103C95BDE557F5E9B971E57DC0EA83D5FEF45AED46379558DEEB18D1D7E4AE8FBDD0458208B2E94720DDAFE556DE20F50E584BBE1011ED5676B647515A369234330EF7CD006D99D70A10186182CF5183CF500F507D99D70A1018400F480F4081A89C8766D031A1A7DAF1C");
    auto expectedUR = std::string("ur:portfolio/oyadlrtaoyrkotadtaoyrhotadayaocsfnaxlycsldaolytaoyrdoeadtantjlonaxhdclaxdaaxtsuooxzmahmwwtfzgthfcslpfwoylgmnatlrfyeheestcmchluselynsfstyaahdcxchcfwdlkpmsgcwrfjsrslpbypsftfdjplnqztegelnaeatrozcgamydieywmldmeamtantjooyadlncsdwykcsfnykaeykattantjooyadlraewklfaeadwkaycyjsoyfxplaolfghkewmcnzcjertpmtlnnidpsdahglfjotkwnrhyncfghcwzcioaxkgfwtkjkpswzaajoiorygwdwfltarstbaxcyathpsnbztaoyrkotadtaoyrhoeadayaoaeaolftaoyrdoyadtantjyoeadisktjojeisdefzdydtaolytantjloxaxhdclaxdwtybdgonbkghphkcwjzoyamaohsnecschhkamgybwolnydajkutgetnjemhuyeyaahdcxurmegwsaykcpfrbngeludnbbbdoylplyhntezcvtdwyadtnbghtosocpyllbnbttamtantjooyadlncsghykaeykaeykaycyspmotepstaoyrdoyadtantjyoeadiojojeisdefzdydtaolytantjlotaxhdclaxtewypscpgmrddlnlpslgbghsrnztfsotglrkkiskfeonzmfygaesrtspwnwefhwsaahdcxfmmtwlmkcfjycflpdtclpdgufmcmjkrtcejeaehdpdsglgdakpcyceoxotttadnsaycywtlochadaxcyathpsnbztaoyrkoeadtaoyrhoeadamaocfadykaolftaoyrdoyadtantjloeaxhdclaefpemsejemtimrdseurgsromupefwcknbrnhsoncpiypkkgftptmumhbsjsesiebsamtantjooyadlecsdwykcfadztykaeykaeykaeyktaoyrdoeadtantjloeaxhdclaemdfliscpfrrhfzbzecaaidrdtncxtnwytsvohhflktytmsrejzaoktfgveyapsioamtantjooyadlecsdwykcfadztykaeykaeykadykaolyksdwfejkeskogtfgjpknhsfxfegmjngeiyjpfgeefdeyfghkfyeegrfxjlgljehkehehgtiafxihetfwihjtktglhkfwtaoyrkotadtaoyrhotadayaocsfnaxlycsldaolytaoyrdoyadtantjlonaxhdclaxsohpuegolbhyndmsckhguobapdfshewsfepltyiakkgolgwypalgcakbgevszoutaahdcxludmmwjpbttnzegojnvobsgdvwlrrkvyadcktliojeiekpbzotincnfxdywsketiamtantjooyadlncsdwykcsfnykaeykattantjooyadlraewklawkaycyldspkojnaxcycykipecewslesruy");

    ValidateCborResults(portfolio, expectedCbor);
    ValidateUrEncoding(portfolio, expectedUR);
}