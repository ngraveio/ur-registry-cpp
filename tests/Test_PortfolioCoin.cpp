#include "Test_PortfolioCoin.h"
#include "Test_UrHelpers.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestPortfolioCoin);

void TestPortfolioCoin::setUp()
{
}

void TestPortfolioCoin::tearDown()
{
}

CoinIdentity TestPortfolioCoin::createCoinIdentity()
{
    // Create a Bitcoin coin identity
    return CoinIdentity(EllipticCurve::Secp256k1, 0);
}

DetailedAccount TestPortfolioCoin::createHDKeyAccount()
{
    // Create a simple HDKey for testing
    HDKey hdkey;
    hdkey.setKeyData({0x02, 0x5a, 0x46, 0x3b, 0xab, 0x5b, 0x8a, 0x5a, 0xc3, 0x57, 0x83, 0xb0, 0x44, 0xe4, 0x2f, 0x25, 0x52, 0x3b, 0x38, 0x5c, 0x2a, 0x9c, 0x49, 0x0e, 0x9f, 0x31, 0x10, 0x51, 0x32, 0x2d, 0x9c, 0xf7, 0x2d});
    hdkey.setChainCode({0x0f, 0x05, 0x51, 0xa9, 0x46, 0x32, 0x0b, 0x97, 0x22, 0x13, 0x24, 0x55, 0x0a, 0xae, 0x42, 0xb2, 0x23, 0x53, 0x58, 0xc4, 0x2f, 0xaa, 0x6a, 0x76, 0x86, 0xc9, 0x06, 0x4e, 0x9a, 0x0e, 0x2c, 0xf5});
    hdkey.setUseInfo(CoinType::Bitcoin);
    hdkey.setOrigin(Keypath("m/44'/0'/0'"));
    hdkey.setParentFingerprint(0x12345678);
    hdkey.setName("Bitcoin Account");
    hdkey.setNote("Primary Bitcoin Account");

    return DetailedAccount(hdkey);
}

DetailedAccount TestPortfolioCoin::createOutputDescriptorAccount()
{
    // Create a simple OutputDescriptor for testing
    HDKey hdkey;
    hdkey.setKeyData({0x02, 0x5a, 0x46, 0x3b, 0xab, 0x5b, 0x8a, 0x5a, 0xc3, 0x57, 0x83, 0xb0, 0x44, 0xe4, 0x2f, 0x25, 0x52, 0x3b, 0x38, 0x5c, 0x2a, 0x9c, 0x49, 0x0e, 0x9f, 0x31, 0x10, 0x51, 0x32, 0x2d, 0x9c, 0xf7, 0x2d});
    std::vector<HDKey> keys = {hdkey};

    // Use placeholder syntax (@0) in the source string
    OutputDescriptor outputDescriptor(
        "wpkh(@0)",
        keys,
        "Bitcoin Descriptor",
        "WPKH Descriptor");

    return DetailedAccount(outputDescriptor);
}

void TestPortfolioCoin::EncodeWithHDKey()
{
    // Create a PortfolioCoin with an HDKey account
    CoinIdentity coinId = createCoinIdentity();
    DetailedAccount account = createHDKeyAccount();
    std::vector<DetailedAccount> accounts = {account};

    PortfolioCoin portfolioCoin(coinId, accounts);

    // Expected CBOR format:
    // {
    //   1: #6.41401({...}),  // coin-id
    //   2: [#6.41402({...})] // accounts
    // }
    // Expected CBOR bytes:
    // A2                                      # map(2)
    //    01                                   # unsigned(1)
    //    D9 A1B9                              # tag(41401)
    //       A2                                # map(2)
    //          01                             # unsigned(1)
    //          08                             # unsigned(8)
    //          02                             # unsigned(2)
    //          00                             # unsigned(0)
    //    02                                   # unsigned(2)
    //    81                                   # array(1)
    //       D9 A1BA                           # tag(41402)
    //          A1                             # map(1)
    //             01                          # unsigned(1)
    //             D9 9D6F                     # tag(40303)
    //                A7                       # map(7)
    //                   03                    # unsigned(3)
    //                   58 21                 # bytes(33)
    //                      025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D # "\u0002ZF;\xAB[\x8AZ\xC3W\x83\xB0D\xE4/%R;8\\*\x9CI\u000E\x9F1\u0010Q2-\x9C\xF7-"
    //                   04                    # unsigned(4)
    //                   58 20                 # bytes(32)
    //                      0F0551A946320B97221324550AAE42B2235358C42FAA6A7686C9064E9A0E2CF5 # "\u000F\u0005Q\xA9F2\v\x97\"\u0013$U\n\xAEB\xB2#SX\xC4/\xAAjv\x86\xC9\u0006N\x9A\u000E,\xF5"
    //                   05                    # unsigned(5)
    //                   D9 9D71               # tag(40305)
    //                      A1                 # map(1)
    //                         01              # unsigned(1)
    //                         00              # unsigned(0)
    //                   06                    # unsigned(6)
    //                   D9 9D70               # tag(40304)
    //                      A1                 # map(1)
    //                         01              # unsigned(1)
    //                         86              # array(6)
    //                            18 2C        # unsigned(44)
    //                            F5           # primitive(21)
    //                            00           # unsigned(0)
    //                            F5           # primitive(21)
    //                            00           # unsigned(0)
    //                            F5           # primitive(21)
    //                   08                    # unsigned(8)
    //                   1A 12345678           # unsigned(305419896)
    //                   09                    # unsigned(9)
    //                   6F                    # text(15)
    //                      426974636F696E204163636F756E74 # "Bitcoin Account"
    //                   0A                    # unsigned(10)
    //                   77                    # text(23)
    //                      5072696D61727920426974636F696E204163636F756E74 # "Primary Bitcoin Account"

    auto expectedCbor = fromHex("A201D9A1B9A2010802000281D9A1BAA101D99D6FA7035821025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D0458200F0551A946320B97221324550AAE42B2235358C42FAA6A7686C9064E9A0E2CF505D99D71A1010006D99D70A10186182CF500F500F5081A12345678096F426974636F696E204163636F756E740A775072696D61727920426974636F696E204163636F756E74");
    auto expectedUR = "ur:portfolio-coin/oeadtaoyrhoeadayaoaeaolytaoyrdoyadtantjlosaxhdclaohtfgfrpyhplehtsrhglspffyvedldagmfrethhdrnsgabaneehbegyeydpnsyldpaahdcxbsahgyptfgeybdmscpbwdkgobkplfwprcnguhdssdlpkimkolnsoamglnybadwykahtantjsoyadaeamtantjooyadlncsdwykaeykaeykaycybgeehfksasjlfwinjyiajlinjtcxfpiaiajlkpjtjybkktgdjpinjnhsjpkkcxfwinjyiajlinjtcxfpiaiajlkpjtjyckcsrtoe";

    ValidateCborResults(portfolioCoin, expectedCbor);
    ValidateUrEncoding(portfolioCoin, expectedUR);
}

void TestPortfolioCoin::EncodeWithOutputDescriptor()
{
    // Create a PortfolioCoin with an OutputDescriptor account
    CoinIdentity coinId = createCoinIdentity();
    DetailedAccount account = createOutputDescriptorAccount();
    std::vector<DetailedAccount> accounts = {account};

    PortfolioCoin portfolioCoin(coinId, accounts);

    // Expected CBOR format:
    // {
    //   1: #6.41401({...}),  // coin-id
    //   2: [#6.41402({...})] // accounts
    // }
    // Expected CBOR bytes:
    // A2                                      # map(2)
    //    01                                   # unsigned(1)
    //    D9 A1B9                              # tag(41401)
    //       A2                                # map(2)
    //          01                             # unsigned(1)
    //          08                             # unsigned(8)
    //          02                             # unsigned(2)
    //          00                             # unsigned(0)
    //    02                                   # unsigned(2)
    //    81                                   # array(1)
    //       D9 A1BA                           # tag(41402)
    //          A1                             # map(1)
    //             01                          # unsigned(1)
    //             D9 9D74                     # tag(40308)
    //                A4                       # map(4)
    //                   01                    # unsigned(1)
    //                   68                    # text(8)
    //                      77706B6828403029   # "wpkh(@0)"
    //                   02                    # unsigned(2)
    //                   81                    # array(1)
    //                      D9 9D6F            # tag(40303)
    //                         A1              # map(1)
    //                            03           # unsigned(3)
    //                            58 21        # bytes(33)
    //                               025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D # "\u0002ZF;\xAB[\x8AZ\xC3W\x83\xB0D\xE4/%R;8\\*\x9CI\u000E\x9F1\u0010Q2-\x9C\xF7-"
    //                   03                    # unsigned(3)
    //                   72                    # text(18)
    //                      426974636F696E2044657363726970746F72 # "Bitcoin Descriptor"
    //                   04                    # unsigned(4)
    //                   6F                    # text(15)
    //                      57504B482044657363726970746F72 # "WPKH Descriptor"

    // Encode to CBOR and validate
    auto expectedCbor = fromHex("A201D9A1B9A2010802000281D9A1BAA101D99D74A4016877706B68284030290281D99D6FA1035821025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D0372426974636F696E2044657363726970746F72046F57504B482044657363726970746F72");
    auto expectedUR = std::string("ur:portfolio-coin/oeadtaoyrhoeadayaoaeaolytaoyrdoyadtantjyoxadisktjojeisdefzdydtaolytantjloyaxhdclaohtfgfrpyhplehtsrhglspffyvedldagmfrethhdrnsgabaneehbegyeydpnsyldpaxjpfwinjyiajlinjtcxfyihjkiajpinjojyjljpaajlhggdgrfdcxfyihjkiajpinjojyjljpvsbatldw");

    // Validate CBOR and UR encoding
    ValidateCborResults(portfolioCoin, expectedCbor);
    ValidateUrEncoding(portfolioCoin, expectedUR);
}

void TestPortfolioCoin::EncodeWithMasterFingerprint()
{
    // Create a PortfolioCoin with a master fingerprint
    CoinIdentity coinId = createCoinIdentity();
    DetailedAccount account = createHDKeyAccount();
    std::vector<DetailedAccount> accounts = {account};
    uint32_t masterFingerprint = 0x12345678;

    PortfolioCoin portfolioCoin(coinId, accounts, masterFingerprint);

    // Expected CBOR format:
    // {
    //   1: #6.41401({...}),  // coin-id
    //   2: [#6.41402({...})], // accounts
    //   3: 0x12345678 // master-fingerprint
    // }
    // Expected CBOR bytes:
    // A3                                      # map(3)
    //    01                                   # unsigned(1)
    //    D9 A1B9                              # tag(41401)
    //       A2                                # map(2)
    //          01                             # unsigned(1)
    //          08                             # unsigned(8)
    //          02                             # unsigned(2)
    //          00                             # unsigned(0)
    //    02                                   # unsigned(2)
    //    81                                   # array(1)
    //       D9 A1BA                           # tag(41402)
    //          A1                             # map(1)
    //             01                          # unsigned(1)
    //             D9 9D6F                     # tag(40303)
    //                A7                       # map(7)
    //                   03                    # unsigned(3)
    //                   58 21                 # bytes(33)
    //                      025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D # "\u0002ZF;\xAB[\x8AZ\xC3W\x83\xB0D\xE4/%R;8\\*\x9CI\u000E\x9F1\u0010Q2-\x9C\xF7-"
    //                   04                    # unsigned(4)
    //                   58 20                 # bytes(32)
    //                      0F0551A946320B97221324550AAE42B2235358C42FAA6A7686C9064E9A0E2CF5 # "\u000F\u0005Q\xA9F2\v\x97\"\u0013$U\n\xAEB\xB2#SX\xC4/\xAAjv\x86\xC9\u0006N\x9A\u000E,\xF5"
    //                   05                    # unsigned(5)
    //                   D9 9D71               # tag(40305)
    //                      A1                 # map(1)
    //                         01              # unsigned(1)
    //                         00              # unsigned(0)
    //                   06                    # unsigned(6)
    //                   D9 9D70               # tag(40304)
    //                      A1                 # map(1)
    //                         01              # unsigned(1)
    //                         86              # array(6)
    //                            18 2C        # unsigned(44)
    //                            F5           # primitive(21)
    //                            00           # unsigned(0)
    //                            F5           # primitive(21)
    //                            00           # unsigned(0)
    //                            F5           # primitive(21)
    //                   08                    # unsigned(8)
    //                   1A 12345678           # unsigned(305419896)
    //                   09                    # unsigned(9)
    //                   6F                    # text(15)
    //                      426974636F696E204163636F756E74 # "Bitcoin Account"
    //                   0A                    # unsigned(10)
    //                   77                    # text(23)
    //                      5072696D61727920426974636F696E204163636F756E74 # "Primary Bitcoin Account"
    //    03                                   # unsigned(3)
    //    1A 12345678                          # unsigned(305419896)

    // Encode to CBOR and validate
    auto expectedCbor = fromHex("A301D9A1B9A2010802000281D9A1BAA101D99D6FA7035821025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D0458200F0551A946320B97221324550AAE42B2235358C42FAA6A7686C9064E9A0E2CF505D99D71A1010006D99D70A10186182CF500F500F5081A12345678096F426974636F696E204163636F756E740A775072696D61727920426974636F696E204163636F756E74031A12345678");
    auto expectedUR = std::string("ur:portfolio-coin/otadtaoyrhoeadayaoaeaolytaoyrdoyadtantjlosaxhdclaohtfgfrpyhplehtsrhglspffyvedldagmfrethhdrnsgabaneehbegyeydpnsyldpaahdcxbsahgyptfgeybdmscpbwdkgobkplfwprcnguhdssdlpkimkolnsoamglnybadwykahtantjsoyadaeamtantjooyadlncsdwykaeykaeykaycybgeehfksasjlfwinjyiajlinjtcxfpiaiajlkpjtjybkktgdjpinjnhsjpkkcxfwinjyiajlinjtcxfpiaiajlkpjtjyaxcybgeehfksdwuywzft");

    // Validate CBOR and UR encoding
    ValidateCborResults(portfolioCoin, expectedCbor);
    ValidateUrEncoding(portfolioCoin, expectedUR);

    // Verify the master fingerprint
    CPPUNIT_ASSERT(portfolioCoin.getMasterFingerprint().value() == masterFingerprint);
}

void TestPortfolioCoin::EncodeWithMultipleAccounts()
{
    // Create a PortfolioCoin with multiple accounts
    CoinIdentity coinId = createCoinIdentity();
    DetailedAccount hdkeyAccount = createHDKeyAccount();

    // Create a second account with OutputDescriptor using placeholder syntax
    HDKey hdkey;
    hdkey.setKeyData({0x02, 0x5a, 0x46, 0x3b, 0xab, 0x5b, 0x8a, 0x5a, 0xc3, 0x57, 0x83, 0xb0, 0x44, 0xe4, 0x2f, 0x25, 0x52, 0x3b, 0x38, 0x5c, 0x2a, 0x9c, 0x49, 0x0e, 0x9f, 0x31, 0x10, 0x51, 0x32, 0x2d, 0x9c, 0xf7, 0x2d});
    std::vector<HDKey> keys = {hdkey};

    OutputDescriptor outputDescriptor(
        "wpkh(@0)",
        keys,
        "Bitcoin Descriptor",
        "WPKH Descriptor");

    DetailedAccount outputDescriptorAccount(outputDescriptor);
    std::vector<DetailedAccount> accounts = {hdkeyAccount, outputDescriptorAccount};

    PortfolioCoin portfolioCoin(coinId, accounts);

    // Expected CBOR format:
    // {
    //   1: #6.41401({...}),  // coin-id
    //   2: [#6.41402({...}), #6.41402({...})] // accounts
    // }
    // Expected CBOR bytes:
    // A2                                      # map(2)
    // 01                                   # unsigned(1)
    // D9 A1B9                              # tag(41401)
    //     A2                                # map(2)
    //         01                             # unsigned(1)
    //         08                             # unsigned(8)
    //         02                             # unsigned(2)
    //         00                             # unsigned(0)
    // 02                                   # unsigned(2)
    // 82                                   # array(2)
    //     D9 A1BA                           # tag(41402)
    //         A1                             # map(1)
    //         01                          # unsigned(1)
    //         D9 9D6F                     # tag(40303)
    //             A7                       # map(7)
    //                 03                    # unsigned(3)
    //                 58 21                 # bytes(33)
    //                     025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D # "\u0002ZF;\xAB[\x8AZ\xC3W\x83\xB0D\xE4/%R;8\\*\x9CI\u000E\x9F1\u0010Q2-\x9C\xF7-"
    //                 04                    # unsigned(4)
    //                 58 20                 # bytes(32)
    //                     0F0551A946320B97221324550AAE42B2235358C42FAA6A7686C9064E9A0E2CF5 # "\u000F\u0005Q\xA9F2\v\x97\"\u0013$U\n\xAEB\xB2#SX\xC4/\xAAjv\x86\xC9\u0006N\x9A\u000E,\xF5"
    //                 05                    # unsigned(5)
    //                 D9 9D71               # tag(40305)
    //                     A1                 # map(1)
    //                     01              # unsigned(1)
    //                     00              # unsigned(0)
    //                 06                    # unsigned(6)
    //                 D9 9D70               # tag(40304)
    //                     A1                 # map(1)
    //                     01              # unsigned(1)
    //                     86              # array(6)
    //                         18 2C        # unsigned(44)
    //                         F5           # primitive(21)
    //                         00           # unsigned(0)
    //                         F5           # primitive(21)
    //                         00           # unsigned(0)
    //                         F5           # primitive(21)
    //                 08                    # unsigned(8)
    //                 1A 12345678           # unsigned(305419896)
    //                 09                    # unsigned(9)
    //                 6F                    # text(15)
    //                     426974636F696E204163636F756E74 # "Bitcoin Account"
    //                 0A                    # unsigned(10)
    //                 77                    # text(23)
    //                     5072696D61727920426974636F696E204163636F756E74 # "Primary Bitcoin Account"
    //     D9 A1BA                           # tag(41402)
    //         A1                             # map(1)
    //         01                          # unsigned(1)
    //         D9 9D74                     # tag(40308)
    //             A4                       # map(4)
    //                 01                    # unsigned(1)
    //                 68                    # text(8)
    //                     77706B6828403029   # "wpkh(@0)"
    //                 02                    # unsigned(2)
    //                 81                    # array(1)
    //                     D9 9D6F            # tag(40303)
    //                     A1              # map(1)
    //                         03           # unsigned(3)
    //                         58 21        # bytes(33)
    //                             025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D # "\u0002ZF;\xAB[\x8AZ\xC3W\x83\xB0D\xE4/%R;8\\*\x9CI\u000E\x9F1\u0010Q2-\x9C\xF7-"
    //                 03                    # unsigned(3)
    //                 72                    # text(18)
    //                     426974636F696E2044657363726970746F72 # "Bitcoin Descriptor"
    //                 04                    # unsigned(4)
    //                 6F                    # text(15)
    //                     57504B482044657363726970746F72 # "WPKH Descriptor"

    // Encode to CBOR and validate
    auto expectedCbor = fromHex("A201D9A1B9A2010802000282D9A1BAA101D99D6FA7035821025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D0458200F0551A946320B97221324550AAE42B2235358C42FAA6A7686C9064E9A0E2CF505D99D71A1010006D99D70A10186182CF500F500F5081A12345678096F426974636F696E204163636F756E740A775072696D61727920426974636F696E204163636F756E74D9A1BAA101D99D74A4016877706B68284030290281D99D6FA1035821025A463BAB5B8A5AC35783B044E42F25523B385C2A9C490E9F311051322D9CF72D0372426974636F696E2044657363726970746F72046F57504B482044657363726970746F72");
    auto expectedUR = std::string("ur:portfolio-coin/oeadtaoyrhoeadayaoaeaolftaoyrdoyadtantjlosaxhdclaohtfgfrpyhplehtsrhglspffyvedldagmfrethhdrnsgabaneehbegyeydpnsyldpaahdcxbsahgyptfgeybdmscpbwdkgobkplfwprcnguhdssdlpkimkolnsoamglnybadwykahtantjsoyadaeamtantjooyadlncsdwykaeykaeykaycybgeehfksasjlfwinjyiajlinjtcxfpiaiajlkpjtjybkktgdjpinjnhsjpkkcxfwinjyiajlinjtcxfpiaiajlkpjtjytaoyrdoyadtantjyoxadisktjojeisdefzdydtaolytantjloyaxhdclaohtfgfrpyhplehtsrhglspffyvedldagmfrethhdrnsgabaneehbegyeydpnsyldpaxjpfwinjyiajlinjtcxfyihjkiajpinjojyjljpaajlhggdgrfdcxfyihjkiajpinjojyjljpnyihsrvw");

    // Validate CBOR and UR encoding
    ValidateCborResults(portfolioCoin, expectedCbor);
    ValidateUrEncoding(portfolioCoin, expectedUR);

    // Verify the number of accounts
    CPPUNIT_ASSERT(portfolioCoin.getAccounts().size() == 2);
}

void TestPortfolioCoin::EncodeSolUaiAccounts()
{
    PortfolioCoin portfolioCoin;
    // Create PortfolioCoin from UAI and public key
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.createPortfolioCoin("uai://ed25519.501:EPjFWdd5AufqSSqeM2qN1xzybapC8G4wEGGkZwyTDt1v", "954768223bb94015350462bada20daeed7e25c4777f997b56c027746e4f8ac67"));
    // Nothing to add since it's the account has already been added before
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.addAccount("uai://ed25519.501", "954768223bb94015350462bada20daeed7e25c4777f997b56c027746e4f8ac67"));
    // Add another account to the same PortfolioCoin
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.addAccount("uai://ed25519.501/44h/508h/0h/0h/0h", "4137c16b966abac1df4cb893af421ea0be61a52266aa7b3aa993900f7139640f"));
    // Add a second token to the first account
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.addAccount("uai://ed25519.501:Es9vMFrzaCERmJfrF4H2FYD4KCoNkY11McCe8BenwNYB", "954768223bb94015350462bada20daeed7e25c4777f997b56c027746e4f8ac67"));
    // Add a first token to the second account
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.addAccount("uai://ed25519.501:Es9vMFrzaCERmJfrF4H2FYD4KCoNkY11McCe8BenwNYB", "4137c16b966abac1df4cb893af421ea0be61a52266aa7b3aa993900f7139640f"));

    // Validate UAI encoding
    // {1: 41401({1: 6, 2: 501}),
    //  2: [41402({1: 40303({3: h'00954768223BB94015350462BADA20DAEED7E25C4777F997B56C027746E4F8AC67'}),
    //             2: ["EPjFWdd5AufqSSqeM2qN1xzybapC8G4wEGGkZwyTDt1v", "Es9vMFrzaCERmJfrF4H2FYD4KCoNkY11McCe8BenwNYB"]}),
    //      41402({1: 40303({3: h'004137C16B966ABAC1DF4CB893AF421EA0BE61A52266AA7B3AA993900F7139640F',
    //                       6: 40304({1: [44, true, 508, true, 0, true, 0, true, 0, true]})}),
    //             2: ["Es9vMFrzaCERmJfrF4H2FYD4KCoNkY11McCe8BenwNYB"]})]
    // }
    auto expectedCbor = fromHex("A201D9A1B9A20106021901F50282D9A1BAA201D99D6FA103582100954768223BB94015350462BADA20DAEED7E25C4777F997B56C027746E4F8AC670282782C45506A465764643541756671535371654D32714E31787A7962617043384734774547476B5A77795444743176782C457339764D46727A614345526D4A667246344832465944344B436F4E6B5931314D6343653842656E774E5942D9A1BAA201D99D6FA2035821004137C16B966ABAC1DF4CB893AF421EA0BE61A52266AA7B3AA993900F7139640F06D99D70A1018A182CF51901FCF500F500F500F50281782C457339764D46727A614345526D4A667246344832465944344B436F4E6B5931314D6343653842656E774E5942");
    auto expectedUR = std::string("ur:portfolio-coin/oeadtaoyrhoeadamaocfadykaolftaoyrdoeadtantjloyaxhdclaemdfliscpfrrhfzbzecaaidrdtncxtnwytsvohhflktytmsrejzaoktfgveyapsioaolfksdwfegdimfghgieieecfpkpiyjsgugujsihgteyjsglehksknkkidhsjofxetfleektfeflfljehtktkkghfyjyehkoksdwfejkeskogtfgjpknhsfxfegmjngeiyjpfgeefdeyfghkfyeegrfxjlgljehkehehgtiafxihetfwihjtktglhkfwtaoyrdoeadtantjloeaxhdclaefpemsejemtimrdseurgsromupefwcknbrnhsoncpiypkkgftptmumhbsjsesiebsamtantjooyadlecsdwykcfadztykaeykaeykaeykaolyksdwfejkeskogtfgjpknhsfxfegmjngeiyjpfgeefdeyfghkfyeegrfxjlgljehkehehgtiafxihetfwihjtktglhkfwgrcpfmut");

    ValidateCborResults(portfolioCoin, expectedCbor);
    ValidateUrEncoding(portfolioCoin, expectedUR);
}

void TestPortfolioCoin::EncodePolUaiAccounts()
{
    PortfolioCoin portfolioCoin;
    // Create PortfolioCoin from UAI and public key
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.createPortfolioCoin("uai://secp256k1.60.137/44'/60'/0h/0/0", "xpub6CVDAP5Ae2wxTNoDXtFqBiwyWU13ejtf16LJXbwMXMmW8i8HdDpWdaC75ss8c1oAmsFFvHXvmJi5MCU1nJZUkvJ3ZsHkCzHRwczDheGWrp3"));
    // Same account, do nothing
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.addAccount("uai://secp256k1.60.137", "xpub6CVDAP5Ae2wxTNoDXtFqBiwyWU13ejtf16LJXbwMXMmW8i8HdDpWdaC75ss8c1oAmsFFvHXvmJi5MCU1nJZUkvJ3ZsHkCzHRwczDheGWrp3"));
    // Add second account with token
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.addAccount("uai://secp256k1.60.137:0x7ceb23fd6bc0add59e62ac25578270cff1b9f619", "xpub6CfWNaFcVGYvYXwgrr7viAS43PadEgGQUoB7PTxYnRPhAcomxXeDUUJK2iuXnrEadibcpdTBVmQ7vdsMq1zbr2EscwSCYKBWb2FXHGbtCeQ"));
    // Add third account with another token
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.addAccount("uai://secp256k1.60.137:0xc2132d05d31c914a87c6611c10748aeb04b58e8f/44'/60'/0h/0/2", "xpub6DRJw8xG7CWTgpJxG5KRcD9qFEzut1DhCHFdmXsZ3VJZBoUFAft9UohvmBWjqRpyV7aP9CW87523WKmsRXJu7Gsg5znuDCp2Sg8Mi6B3Bcc"));
    // Add another token to the third account and define master fingerprint
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.addAccount("uai://secp256k1.60.137:0x3c499c542cEF5E3811e1192ce70d8cC03d5c3359/44'/60'/0h/0/2?master_fingerprint=123456789", "xpub6DRJw8xG7CWTgpJxG5KRcD9qFEzut1DhCHFdmXsZ3VJZBoUFAft9UohvmBWjqRpyV7aP9CW87523WKmsRXJu7Gsg5znuDCp2Sg8Mi6B3Bcc"));
    // Add another token to the second account with the same master fingerprint
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.addAccount("uai://secp256k1.60.137:0x1bfd67037b42cf73acf2047067bd4f2c47d9bfd6?master_fingerprint=123456789", "xpub6CfWNaFcVGYvYXwgrr7viAS43PadEgGQUoB7PTxYnRPhAcomxXeDUUJK2iuXnrEadibcpdTBVmQ7vdsMq1zbr2EscwSCYKBWb2FXHGbtCeQ"));

    // Validate UAI encoding
    // {1: 41401({1: 8, 2: 60, 3: [137]}),
    //  2: [41402({1: 40303({3: h'032503D7DCA4FF0594F0404D56188542A18D8E0784443134C716178BC1819C3DD4', 4: h'1719EA8CADCA1BBC71BF8511AC3A487286B4D34A860007B8FD498F2732EB8991', 6: 40304({1: [44, true, 60, true, 0, true]}), 7: 40304({1: [0, false, 0, false]}), 8: 1906394030})}),
    //      41402({1: 40303({3: h'03C95BDE557F5E9B971E57DC0EA83D5FEF45AED46379558DEEB18D1D7E4AE8FBDD', 4: h'8B2E94720DDAFE556DE20F50E584BBE1011ED5676B647515A369234330EF7CD0', 8: 2311616109}), 2: ["0x7ceb23fd6bc0add59e62ac25578270cff1b9f619", "0x1bfd67037b42cf73acf2047067bd4f2c47d9bfd6"]}),
    //      41402({1: 40303({3: h'03D3EEAC2252BA2F99AC8D1261BEFC3DA34EBB7DC545A5FF444939C0C8F1ED3FEF', 4: h'3E96E998197419852921A8533E1673C01C6B0058A8CA8D25751A1CA4A3D1019C', 6: 40304({1: [44, true, 60, true, 0, true]}), 7: 40304({1: [0, false, 2, false]}), 8: 4035450625}), 2: ["0xc2132d05d31c914a87c6611c10748aeb04b58e8f", "0x3c499c542cEF5E3811e1192ce70d8cC03d5c3359"]})],
    // 3: 123456789}
    auto expectedCbor = fromHex("A301D9A1B9A3010802183C038118890283D9A1BAA101D99D6FA5035821032503D7DCA4FF0594F0404D56188542A18D8E0784443134C716178BC1819C3DD40458201719EA8CADCA1BBC71BF8511AC3A487286B4D34A860007B8FD498F2732EB899106D99D70A10186182CF5183CF500F507D99D70A1018400F400F4081A71A143AED9A1BAA201D99D6FA303582103C95BDE557F5E9B971E57DC0EA83D5FEF45AED46379558DEEB18D1D7E4AE8FBDD0458208B2E94720DDAFE556DE20F50E584BBE1011ED5676B647515A369234330EF7CD0081A89C8766D0282782A307837636562323366643662633061646435396536326163323535373832373063666631623966363139782A307831626664363730333762343263663733616366323034373036376264346632633437643962666436D9A1BAA201D99D6FA503582103D3EEAC2252BA2F99AC8D1261BEFC3DA34EBB7DC545A5FF444939C0C8F1ED3FEF0458203E96E998197419852921A8533E1673C01C6B0058A8CA8D25751A1CA4A3D1019C06D99D70A10186182CF5183CF500F507D99D70A1018400F402F4081AF08817010282782A307863323133326430356433316339313461383763363631316331303734386165623034623538653866782A307833633439396335343263454635453338313165313139326365373064386343303364356333333539031A075BCD15");
    auto expectedUR = std::string("ur:portfolio-coin/otadtaoyrhotadayaocsfnaxlycsldaolstaoyrdoyadtantjlonaxhdclaxdaaxtsuooxzmahmwwtfzgthfcslpfwoylgmnatlrfyeheestcmchluselynsfstyaahdcxchcfwdlkpmsgcwrfjsrslpbypsftfdjplnqztegelnaeatrozcgamydieywmldmeamtantjooyadlncsdwykcsfnykaeykattantjooyadlraewkaewkaycyjsoyfxpltaoyrdoeadtantjlotaxhdclaxsohpuegolbhyndmsckhguobapdfshewsfepltyiakkgolgwypalgcakbgevszoutaahdcxludmmwjpbttnzegojnvobsgdvwlrrkvyadcktliojeiekpbzotincnfxdywsketiaycyldspkojnaolfksdrdyksemiaihideyeoiyieenidiadyhsieieecesiheneyhsiaeyececemeteyemdyiaiyiyehidesiyenehesksdrdyksehidiyieenemdyeoemideeeyiaiyemeohsiaiyeydyeeemdyenemidieeeiyeyiaeeemieesidiyieentaoyrdoeadtantjlonaxhdclaxtewypscpgmrddlnlpslgbghsrnztfsotglrkkiskfeonzmfygaesrtspwnwefhwsaahdcxfmmtwlmkcfjycflpdtclpdgufmcmjkrtcejeaehdpdsglgdakpcyceoxotttadnsamtantjooyadlncsdwykcsfnykaeykattantjooyadlraewkaowkaycywtlochadaolfksdrdyksiaeyeheoeyiedyecieeoehiaeseheehsetemiaenenehehiaehdyemeeethsihiddyeeidecetihetiyksdrdykseoiaeeesesiaeceeeyiafefgecfeeoetehehiheheheseyiaihemdyieetiafxdyeoieeciaeoeoecesaxcyathpsnbzhnmntbhy");

    ValidateCborResults(portfolioCoin, expectedCbor);
    ValidateUrEncoding(portfolioCoin, expectedUR);
}

void TestPortfolioCoin::EncodeBtcUaiAccounts()
{
    PortfolioCoin portfolioCoin;
    // Create PortfolioCoin from UAI and public key with output descriptor
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.createPortfolioCoin("uai://secp256k1.0@wpkh/84'/0'/0'", "xpub6CVDAP5Ae2wxTNoDXtFqBiwyWU13ejtf16LJXbwMXMmW8i8HdDpWdaC75ss8c1oAmsFFvHXvmJi5MCU1nJZUkvJ3ZsHkCzHRwczDheGWrp3"));
    // Add another output descriptor
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.addAccount("uai://secp256k1.0@sh", "xpub6CfWNaFcVGYvYXwgrr7viAS43PadEgGQUoB7PTxYnRPhAcomxXeDUUJK2iuXnrEadibcpdTBVmQ7vdsMq1zbr2EscwSCYKBWb2FXHGbtCeQ"));
    // Add another output descriptor
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.addAccount("uai://secp256k1.0@tr/86h/0h/0h/2/<1;2h>", "xpub6DRJw8xG7CWTgpJxG5KRcD9qFEzut1DhCHFdmXsZ3VJZBoUFAft9UohvmBWjqRpyV7aP9CW87523WKmsRXJu7Gsg5znuDCp2Sg8Mi6B3Bcc"));
    // Same account as first one, do nothing
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.addAccount("uai://secp256k1.0@wpkh", "xpub6CVDAP5Ae2wxTNoDXtFqBiwyWU13ejtf16LJXbwMXMmW8i8HdDpWdaC75ss8c1oAmsFFvHXvmJi5MCU1nJZUkvJ3ZsHkCzHRwczDheGWrp3"));

    // Validate UAI encoding
    // {1: 41401({1: 8, 2: 0}),
    //  2: [41402({1: 40308({1: "wpkh(@0)", 2: [40303({3: h'032503D7DCA4FF0594F0404D56188542A18D8E0784443134C716178BC1819C3DD4', 4: h'1719EA8CADCA1BBC71BF8511AC3A487286B4D34A860007B8FD498F2732EB8991', 6: 40304({1: [84, true, 0, true, 0, true]}), 8: 1906394030})]})}),
    //      41402({1: 40308({1: "sh(@0)", 2: [40303({3: h'03C95BDE557F5E9B971E57DC0EA83D5FEF45AED46379558DEEB18D1D7E4AE8FBDD', 4: h'8B2E94720DDAFE556DE20F50E584BBE1011ED5676B647515A369234330EF7CD0', 8: 2311616109})]})}),
    //      41402({1: 40308({1: "tr(@0)", 2: [40303({3: h'03D3EEAC2252BA2F99AC8D1261BEFC3DA34EBB7DC545A5FF444939C0C8F1ED3FEF', 4: h'3E96E998197419852921A8533E1673C01C6B0058A8CA8D25751A1CA4A3D1019C', 6: 40304({1: [86, true, 0, true, 0, true, 2, false, [1, false, 2, true]]}), 8: 4035450625})]})})]
    // }
    auto expectedCbor = fromHex("A201D9A1B9A2010802000283D9A1BAA101D99D74A2016877706B68284030290281D99D6FA4035821032503D7DCA4FF0594F0404D56188542A18D8E0784443134C716178BC1819C3DD40458201719EA8CADCA1BBC71BF8511AC3A487286B4D34A860007B8FD498F2732EB899106D99D70A101861854F500F500F5081A71A143AED9A1BAA101D99D74A201667368284030290281D99D6FA303582103C95BDE557F5E9B971E57DC0EA83D5FEF45AED46379558DEEB18D1D7E4AE8FBDD0458208B2E94720DDAFE556DE20F50E584BBE1011ED5676B647515A369234330EF7CD0081A89C8766DD9A1BAA101D99D74A201667472284030290281D99D6FA403582103D3EEAC2252BA2F99AC8D1261BEFC3DA34EBB7DC545A5FF444939C0C8F1ED3FEF0458203E96E998197419852921A8533E1673C01C6B0058A8CA8D25751A1CA4A3D1019C06D99D70A101891856F500F500F502F48401F402F5081AF0881701");
    auto expectedUR = std::string("ur:portfolio-coin/oeadtaoyrhoeadayaoaeaolstaoyrdoyadtantjyoeadisktjojeisdefzdydtaolytantjloxaxhdclaxdaaxtsuooxzmahmwwtfzgthfcslpfwoylgmnatlrfyeheestcmchluselynsfstyaahdcxchcfwdlkpmsgcwrfjsrslpbypsftfdjplnqztegelnaeatrozcgamydieywmldmeamtantjooyadlncsghykaeykaeykaycyjsoyfxpltaoyrdoyadtantjyoeadiyjkisdefzdydtaolytantjlotaxhdclaxsohpuegolbhyndmsckhguobapdfshewsfepltyiakkgolgwypalgcakbgevszoutaahdcxludmmwjpbttnzegojnvobsgdvwlrrkvyadcktliojeiekpbzotincnfxdywsketiaycyldspkojntaoyrdoyadtantjyoeadiyjyjpdefzdydtaolytantjloxaxhdclaxtewypscpgmrddlnlpslgbghsrnztfsotglrkkiskfeonzmfygaesrtspwnwefhwsaahdcxfmmtwlmkcfjycflpdtclpdgufmcmjkrtcejeaehdpdsglgdakpcyceoxotttadnsamtantjooyadldcshfykaeykaeykaowklradwkaoykaycywtlochadbejsskjo");

    ValidateCborResults(portfolioCoin, expectedCbor);
    ValidateUrEncoding(portfolioCoin, expectedUR);
}

void TestPortfolioCoin::EncodeBothHDKeyAndOutputDescriptorFromUai()
{
    PortfolioCoin portfolioCoin;
    // Create PortfolioCoin from UAI and public key with output descriptor
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.createPortfolioCoin("uai://secp256k1.0@wpkh/84h/0h/0h/0/<1;2'>", "xpub6CVDAP5Ae2wxTNoDXtFqBiwyWU13ejtf16LJXbwMXMmW8i8HdDpWdaC75ss8c1oAmsFFvHXvmJi5MCU1nJZUkvJ3ZsHkCzHRwczDheGWrp3"));
    // Add an account based on hdkey
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.addAccount("uai://secp256k1.0/86h/0h/0h", "xpub6CfWNaFcVGYvYXwgrr7viAS43PadEgGQUoB7PTxYnRPhAcomxXeDUUJK2iuXnrEadibcpdTBVmQ7vdsMq1zbr2EscwSCYKBWb2FXHGbtCeQ"));

    // Validate UAI encoding
    // {1: 41401({1: 8, 2: 0}),
    //  2: [41402({1: 40308({1: "wpkh(@0)", 2: [40303({3: h'032503D7DCA4FF0594F0404D56188542A18D8E0784443134C716178BC1819C3DD4', 4: h'1719EA8CADCA1BBC71BF8511AC3A487286B4D34A860007B8FD498F2732EB8991', 6: 40304({1: [84, true, 0, true, 0, true, 0, false, [1, false, 2, true]]}), 8: 1906394030})]})}),
    //      41402({1: 40303({3: h'03C95BDE557F5E9B971E57DC0EA83D5FEF45AED46379558DEEB18D1D7E4AE8FBDD', 4: h'8B2E94720DDAFE556DE20F50E584BBE1011ED5676B647515A369234330EF7CD0', 6: 40304({1: [86, true, 0, true, 0, true]}), 8: 2311616109})})]
    // }
    auto expectedCbor = fromHex("A201D9A1B9A2010802000282D9A1BAA101D99D74A2016877706B68284030290281D99D6FA4035821032503D7DCA4FF0594F0404D56188542A18D8E0784443134C716178BC1819C3DD40458201719EA8CADCA1BBC71BF8511AC3A487286B4D34A860007B8FD498F2732EB899106D99D70A101891854F500F500F500F48401F402F5081A71A143AED9A1BAA101D99D6FA403582103C95BDE557F5E9B971E57DC0EA83D5FEF45AED46379558DEEB18D1D7E4AE8FBDD0458208B2E94720DDAFE556DE20F50E584BBE1011ED5676B647515A369234330EF7CD006D99D70A101861856F500F500F5081A89C8766D");
    auto expectedUR = std::string("ur:portfolio-coin/oeadtaoyrhoeadayaoaeaolftaoyrdoyadtantjyoeadisktjojeisdefzdydtaolytantjloxaxhdclaxdaaxtsuooxzmahmwwtfzgthfcslpfwoylgmnatlrfyeheestcmchluselynsfstyaahdcxchcfwdlkpmsgcwrfjsrslpbypsftfdjplnqztegelnaeatrozcgamydieywmldmeamtantjooyadldcsghykaeykaeykaewklradwkaoykaycyjsoyfxpltaoyrdoyadtantjloxaxhdclaxsohpuegolbhyndmsckhguobapdfshewsfepltyiakkgolgwypalgcakbgevszoutaahdcxludmmwjpbttnzegojnvobsgdvwlrrkvyadcktliojeiekpbzotincnfxdywsketiamtantjooyadlncshfykaeykaeykaycyldspkojnueadswch");

    ValidateCborResults(portfolioCoin, expectedCbor);
    ValidateUrEncoding(portfolioCoin, expectedUR);
}

void TestPortfolioCoin::AddDifferentUaiCoinIdentity()
{
    PortfolioCoin portfolioCoin;
    // Create PortfolioCoin from UAI and public key
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.createPortfolioCoin("uai://secp256k1.60.137/44'/60'/0h/0/0", "xpub6CVDAP5Ae2wxTNoDXtFqBiwyWU13ejtf16LJXbwMXMmW8i8HdDpWdaC75ss8c1oAmsFFvHXvmJi5MCU1nJZUkvJ3ZsHkCzHRwczDheGWrp3"));
    // Same account, do nothing
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Adding another coin identity should generate an exception",
        portfolioCoin.addAccount("uai://secp256k1.60.1", "xpub6CVDAP5Ae2wxTNoDXtFqBiwyWU13ejtf16LJXbwMXMmW8i8HdDpWdaC75ss8c1oAmsFFvHXvmJi5MCU1nJZUkvJ3ZsHkCzHRwczDheGWrp3"),
        CborException);
}

void TestPortfolioCoin::AddDifferentUaiMasterFingerprint()
{
    PortfolioCoin portfolioCoin;
    // Create PortfolioCoin from UAI and public key
    CPPUNIT_ASSERT_NO_THROW(portfolioCoin.createPortfolioCoin("uai://secp256k1.60.1/44'/60'/0h/0/0?master_fingerprint=450912374", "xpub6CVDAP5Ae2wxTNoDXtFqBiwyWU13ejtf16LJXbwMXMmW8i8HdDpWdaC75ss8c1oAmsFFvHXvmJi5MCU1nJZUkvJ3ZsHkCzHRwczDheGWrp3"));
    // Same account, do nothing
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Adding another coin identity should generate an exception",
        portfolioCoin.addAccount("uai://secp256k1.60.1:0x1bfd67037b42cf73acf2047067bd4f2c47d9bfd6?master_fingerprint=987300433", "xpub6DRJw8xG7CWTgpJxG5KRcD9qFEzut1DhCHFdmXsZ3VJZBoUFAft9UohvmBWjqRpyV7aP9CW87523WKmsRXJu7Gsg5znuDCp2Sg8Mi6B3Bcc"),
        CborException);
}