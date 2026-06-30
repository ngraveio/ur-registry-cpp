#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <string>

#include "Test_DetailedAccount.h"
#include "../src/detailed_account.h"
#include "Test_UrHelpers.h"

void TestDetailedAccount::setUp(void)
{
}

void TestDetailedAccount::tearDown(void)
{
}

void TestDetailedAccount::testDetailedAccountWithHDKey()
{
    /* detailed-account with a hdkey account and tokenids list */
    HDKey hdkey0;
    hdkey0.setKeyData(convertToKeyData(fromHex("021c0b479ecf6e67713ddf0c43b634592f51c037b6f951fb1dc6361a98b1e5735e")));
    const DetailedAccount detailedAccount{hdkey0, {"1st tokenid str", "some tokenid str"}};
    const auto expectedBytes = fromHex("A201D99D6FA1035821021C0B479ECF6E67713DDF0C43B634592F51C037B6F951FB1DC6361A98B1E5735E02826F31737420746F6B656E69642073747270736F6D6520746F6B656E696420737472");
    const auto expectedUR = "ur:detailed-account/oeadtantjloyaxhdclaocebdflnntkjtiojsfsurbnfxrpeehkdlgyrtemrpytgyzocaswencymkpavwjkhyaolfjlehjkjycxjyjljeihjtiniecxjkjyjpjojkjljnihcxjyjljeihjtiniecxjkjyjpylwltyqd";

    ValidateCborResults(detailedAccount, expectedBytes);
    ValidateUrEncoding(detailedAccount, expectedUR);
}

void TestDetailedAccount::testDetailedAccountWithOutputDesc()
{
    /* detailed-account with an output-descriptor account and tokenids array list */
    HDKey hdkey0;
    hdkey0.setKeyData(convertToKeyData(fromHex("021c0b479ecf6e67713ddf0c43b634592f51c037b6f951fb1dc6361a98b1e5735e")));
    HDKey hdkey1;
    hdkey1.setKeyData(convertToKeyData(fromHex("0397fcf2274abd243d42d42d3c248608c6d1935efca46138afef43af08e9712896")));
    const OutputDescriptor outputDesc{"wsh(sortedmulti(2,@0,@1))", {hdkey0, hdkey1}, "a name", "a note"};
    const DetailedAccount detailedAccount{outputDesc, {"1st tokenid str", "some tokenid str"}};
    const auto expectedBytes = fromHex("A201D99D74A401781977736828736F727465646D756C746928322C40302C403129290282D99D6FA1035821021C0B479ECF6E67713DDF0C43B634592F51C037B6F951FB1DC6361A98B1E5735ED99D6FA10358210397FCF2274ABD243D42D42D3C248608C6D1935EFCA46138AFEF43AF08E9712896036661206E616D65046661206E6F746502826F31737420746F6B656E69642073747270736F6D6520746F6B656E696420737472");
    const auto expectedUR = std::string("ur:detailed-account/oeadtantjyoxadkscfktjkisdejkjljpjyihiejnkpjzjyindeeydwfzdydwfzehdtdtaolftantjloyaxhdclaocebdflnntkjtiojsfsurbnfxrpeehkdlgyrtemrpytgyzocaswencymkpavwjkhytantjloyaxhdclaxmsztwzdigerydkfsfwtydpfndklnayswttmuhyztoxhsetpewsfxpeaywljsdemtaxiyhscxjthsjnihaaiyhscxjtjljyihaolfjlehjkjycxjyjljeihjtiniecxjkjyjpjojkjljnihcxjyjljeihjtiniecxjkjyjptnmthkie");

    ValidateCborResults(detailedAccount, expectedBytes);
    ValidateUrEncoding(detailedAccount, expectedUR);
}

void TestDetailedAccount::testDetailedAccountWithUninitAccount()
{
    /* invalid detailed-account with uninitialized account field */
    const DetailedAccount detailedAccount = DetailedAccount{std::variant<std::monostate, HDKey, OutputDescriptor>{std::monostate{}}, {"a name", "a note"}};
    ValidateUrEncodingException(detailedAccount, CborError::CborErrorImproperValue);
}

void TestDetailedAccount::testDetailedAccountWithLegacyFlag()
{
    /* invalid detailed-account with legacy flag */
    HDKey hdkey0;
    hdkey0.setKeyData(convertToKeyData(fromHex("021c0b479ecf6e67713ddf0c43b634592f51c037b6f951fb1dc6361a98b1e5735e")));
    DetailedAccount detailedAccount{hdkey0};
    detailedAccount.setUseLegacyType(true);

    ValidateUrEncodingException(detailedAccount, CborError::CborErrorIllegalType);
}

void TestDetailedAccount::UaiExampleBtcTpubNativeSegwit()
{
    DetailedAccount detailedAccount_uai;

    // Bitcoin (BTC) Native Segwit tpub without derivation path
    // {1: 40308({1: "wpkh(@0)",
    //            2: [40303({3: h'026FE2355745BB2DB3630BBC80EF5D58951C963C841F54170BA6E5C12BE7FC12A6',
    //                       4: h'CED155C72456255881793514EDC5BD9447E7F74ABB88C6D6B6480FD016EE8C85',
    //                       8: 3910671603
    //              })]
    //           })
    // }
    CPPUNIT_ASSERT_NO_THROW(detailedAccount_uai.setDetailedAccount("uai://secp256k1.0@wpkh", "tpubDHW3GtnVrTatx38EcygoSf9UhUd9Dx1rht7FAL8unrMo8r2NWhJuYNqDFS7cZFVbDaxJkV94MLZAr86XFPsAPYcoHWJ7sWYsrmHDw5sKQ2K"));
    const auto expectedBytes = fromHex("A101D99D74A2016877706B68284030290281D99D6FA3035821026FE2355745BB2DB3630BBC80EF5D58951C963C841F54170BA6E5C12BE7FC12A6045820CED155C72456255881793514EDC5BD9447E7F74ABB88C6D6B6480FD016EE8C85081AE9181CF3");
    const auto expectedUR = std::string("ur:detailed-account/oyadtantjyoeadisktjojeisdefzdydtaolytantjlotaxhdclaojlvoechgferkdpqdiabdrflawshlhdmdcemtfnlrctghchbdolvwsednvdztbgolaahdcxtottgostdkhfdahdlykkecbbweskrymwflvdylgerkloswtbrpfdbsticmwylklpaycywlcscewfhtfghnjy");

    ValidateCborResults(detailedAccount_uai, expectedBytes);
    ValidateUrEncoding(detailedAccount_uai, expectedUR);
}

void TestDetailedAccount::UaiExampleDogeXpubLegacy()
{
    DetailedAccount detailedAccount_uai;

    // Dogecoin (DOGE) Legacy xpub with derivation path
    // {1: 40308({1: "pkh(@0)",
    //            2: [40303({3: h'03C95BDE557F5E9B971E57DC0EA83D5FEF45AED46379558DEEB18D1D7E4AE8FBDD',
    //                      4: h'8B2E94720DDAFE556DE20F50E584BBE1011ED5676B647515A369234330EF7CD0',
    //                      6: 40304({1: [44, true, 3, true, 0, true]}),
    //                      7: 40304({1: [0, false, 4, false]}),
    //                      8: 2311616109})
    //               ]
    //         })
    // }
    CPPUNIT_ASSERT_NO_THROW(detailedAccount_uai.setDetailedAccount("uai://secp256k1.3@pkh/44h/3h/0h/0/4", "xpub6CfWNaFcVGYvYXwgrr7viAS43PadEgGQUoB7PTxYnRPhAcomxXeDUUJK2iuXnrEadibcpdTBVmQ7vdsMq1zbr2EscwSCYKBWb2FXHGbtCeQ"));
    const auto expectedBytes = fromHex("A101D99D74A20167706B68284030290281D99D6FA503582103C95BDE557F5E9B971E57DC0EA83D5FEF45AED46379558DEEB18D1D7E4AE8FBDD0458208B2E94720DDAFE556DE20F50E584BBE1011ED5676B647515A369234330EF7CD006D99D70A10186182CF503F500F507D99D70A1018400F404F4081A89C8766D");
    const auto expectedUR = std::string("ur:detailed-account/oyadtantjyoeadiojojeisdefzdydtaolytantjlonaxhdclaxsohpuegolbhyndmsckhguobapdfshewsfepltyiakkgolgwypalgcakbgevszoutaahdcxludmmwjpbttnzegojnvobsgdvwlrrkvyadcktliojeiekpbzotincnfxdywsketiamtantjooyadlncsdwykaxykaeykattantjooyadlraewkaawkaycyldspkojnrnmhvown");

    ValidateCborResults(detailedAccount_uai, expectedBytes);
    ValidateUrEncoding(detailedAccount_uai, expectedUR);
}

void TestDetailedAccount::UaiExampleEthUsdc()
{
    DetailedAccount detailedAccount_uai;

    // Ethereum (ETH) xpub with USDC token address
    // {1: 40303({3: h'0204FA032947600AAE94A889DFC31B5C96AFCB7F7E23A189114B71E0254C4B88B5',
    //            4: h'1D710954E58073F927F283C61BC1428AC30A3D86D04DACB3A78613401F781839',
    //            6: 40304({1: [44, true, 60, true, 0, true]}),
    //            7: 40304({1: [1, false, 0, false]}),
    //            8: 1756817576}),
    //  2: ["0x3c499c542cEF5E3811e1192ce70d8cC03d5c3359"]}
    CPPUNIT_ASSERT_NO_THROW(detailedAccount_uai.setDetailedAccount("uai://secp256k1.60.137:0x3c499c542cEF5E3811e1192ce70d8cC03d5c3359/44h/60h/0h/1/0?master_fingerprint=2819587291", "xpub6CRQif2S43vtEYf5cZdMhrFpuBFkgFkALM6qhJZz7ws2cfEf1f8Jiv8dXSkizobckHyfH1mFDFZn46AJoh8d4FpB6ydFFg49yPkJF69GsHq"));
    auto expectedBytes = fromHex("A201D99D6FA50358210204FA032947600AAE94A889DFC31B5C96AFCB7F7E23A189114B71E0254C4B88B50458201D710954E58073F927F283C61BC1428AC30A3D86D04DACB3A78613401F78183906D99D70A10186182CF5183CF500F507D99D70A1018401F400F4081A68B6E8A80281543C499C542CEF5E3811E1192CE70D8CC03D5C3359");
    auto expectedUR = std::string("ur:detailed-account/oeadtantjlonaxhdclaoaazsaxdtflhnbkplmwpdldursrcwhhmtpesblbkbcnoyldbygrjsvtdagsgrloreaahdcxcajsasghvwlajkytdiwzlsswcwsefwlesrbkfslntigtpsqdoslnbwfzctkscsesamtantjooyadlncsdwykcsfnykaeykattantjooyadlradwkaewkaycyisrpvspdaolyghfngansghdwwshyetbyvycfdwvdbtlkrtfshheohkwyldcalk");

    ValidateCborResults(detailedAccount_uai, expectedBytes);
    ValidateUrEncoding(detailedAccount_uai, expectedUR);
}

void TestDetailedAccount::UaiExampleEthRangeIndex()
{
    DetailedAccount detailedAccount_uai;

    // Ethereum (ETH) xpub with range index 0 to 2 as children
    // {1: 40303({3: h'0204FA032947600AAE94A889DFC31B5C96AFCB7F7E23A189114B71E0254C4B88B5',
    //            4: h'1D710954E58073F927F283C61BC1428AC30A3D86D04DACB3A78613401F781839',
    //            6: 40304({1: [44, true, 60, true, 0, true]}),
    //            7: 40304({1: [0, false, 0, false, [0, 2], false]}),
    //            8: 1756817576})
    // }
    CPPUNIT_ASSERT_NO_THROW(detailedAccount_uai.setDetailedAccount("uai://secp256k1.60.1/44h/60h/0h/0/0/[0,2]?master_fingerprint=934670036", "xpub6CRQif2S43vtEYf5cZdMhrFpuBFkgFkALM6qhJZz7ws2cfEf1f8Jiv8dXSkizobckHyfH1mFDFZn46AJoh8d4FpB6ydFFg49yPkJF69GsHq"));
    auto expectedBytes = fromHex("A101D99D6FA50358210204FA032947600AAE94A889DFC31B5C96AFCB7F7E23A189114B71E0254C4B88B50458201D710954E58073F927F283C61BC1428AC30A3D86D04DACB3A78613401F78183906D99D70A10186182CF5183CF500F507D99D70A1018600F400F4820002F4081A68B6E8A8");
    auto expectedUR = std::string("ur:detailed-account/oyadtantjlonaxhdclaoaazsaxdtflhnbkplmwpdldursrcwhhmtpesblbkbcnoyldbygrjsvtdagsgrloreaahdcxcajsasghvwlajkytdiwzlsswcwsefwlesrbkfslntigtpsqdoslnbwfzctkscsesamtantjooyadlncsdwykcsfnykaeykattantjooyadlnaewkaewklfaeaowkaycyisrpvspdfhykihga");

    ValidateCborResults(detailedAccount_uai, expectedBytes);
    ValidateUrEncoding(detailedAccount_uai, expectedUR);
}

void TestDetailedAccount::UaiExampleEthPairIndexAndWildcardIndex()
{
    DetailedAccount detailedAccount_uai;

    // Ethereum (ETH) xpub with a pair index in origin and a wildcard index in children
    // {1: 40303({3: h'0204FA032947600AAE94A889DFC31B5C96AFCB7F7E23A189114B71E0254C4B88B5',
    //            4: h'1D710954E58073F927F283C61BC1428AC30A3D86D04DACB3A78613401F781839',
    //            6: 40304({1: [44, true, 60, true, 0, true, 0, false, [0, false, 1, true]]}),
    //            7: 40304({1: [1, false, [], false]}),
    //            8: 1756817576})
    // }
    CPPUNIT_ASSERT_NO_THROW(detailedAccount_uai.setDetailedAccount("uai://secp256k1.60.1/44h/60h/0h/0/<0;1h>/1/[]?master_fingerprint=934670036", "xpub6CRQif2S43vtEYf5cZdMhrFpuBFkgFkALM6qhJZz7ws2cfEf1f8Jiv8dXSkizobckHyfH1mFDFZn46AJoh8d4FpB6ydFFg49yPkJF69GsHq"));
    auto expectedBytes = fromHex("A101D99D6FA50358210204FA032947600AAE94A889DFC31B5C96AFCB7F7E23A189114B71E0254C4B88B50458201D710954E58073F927F283C61BC1428AC30A3D86D04DACB3A78613401F78183906D99D70A10189182CF5183CF500F500F48400F401F507D99D70A1018401F480F4081A68B6E8A8");
    auto expectedUR = std::string("ur:detailed-account/oyadtantjlonaxhdclaoaazsaxdtflhnbkplmwpdldursrcwhhmtpesblbkbcnoyldbygrjsvtdagsgrloreaahdcxcajsasghvwlajkytdiwzlsswcwsefwlesrbkfslntigtpsqdoslnbwfzctkscsesamtantjooyadldcsdwykcsfnykaeykaewklraewkadykattantjooyadlradwklawkaycyisrpvspdfzcmkorh");

    ValidateCborResults(detailedAccount_uai, expectedBytes);
    ValidateUrEncoding(detailedAccount_uai, expectedUR);
}

void TestDetailedAccount::UaiExampleSolanaUsdc()
{
    DetailedAccount detailedAccount_uai;

    // Solana (SOL) public key with USDC as token and without derivation path
    // {1: 40303({3: h'00954768223bb94015350462bada20daeed7e25c4777f997b56c027746e4f8ac67'}),
    //  2: ["EPjFWdd5AufqSSqeM2qN1xzybapC8G4wEGGkZwyTDt1v"]}
    CPPUNIT_ASSERT_NO_THROW(detailedAccount_uai.setDetailedAccount("uai://ed25519.501:EPjFWdd5AufqSSqeM2qN1xzybapC8G4wEGGkZwyTDt1v", "954768223bb94015350462bada20daeed7e25c4777f997b56c027746e4f8ac67"));
    auto expectedBytes = fromHex("A201D99D6FA103582100954768223BB94015350462BADA20DAEED7E25C4777F997B56C027746E4F8AC670281782C45506A465764643541756671535371654D32714E31787A7962617043384734774547476B5A77795444743176");
    auto expectedUR = std::string("ur:detailed-account/oeadtantjloyaxhdclaemdfliscpfrrhfzbzecaaidrdtncxtnwytsvohhflktytmsrejzaoktfgveyapsioaolyksdwfegdimfghgieieecfpkpiyjsgugujsihgteyjsglehksknkkidhsjofxetfleektfeflfljehtktkkghfyjyehkooemytasw");

    ValidateCborResults(detailedAccount_uai, expectedBytes);
    ValidateUrEncoding(detailedAccount_uai, expectedUR);
}

void TestDetailedAccount::UaiExampleMultiversxUsdc()
{
    DetailedAccount detailedAccount_uai;

    // MultiversX (EGLD) public key with USDC as token and with derivation path
    // {1: 40303({3: h'004137C16B966ABAC1DF4CB893AF421EA0BE61A52266AA7B3AA993900F7139640F',
    //            6: 40304({1: [44, true, 508, true, 0, true, 0, true, 0, true]})}),
    // 2: ["USDC-c76f1f"]}
    CPPUNIT_ASSERT_NO_THROW(detailedAccount_uai.setDetailedAccount("uai://ed25519.508:USDC-c76f1f/44h/508h/0h/0h/0h", "4137c16b966abac1df4cb893af421ea0be61a52266aa7b3aa993900f7139640f"));
    auto expectedBytes = fromHex("A201D99D6FA2035821004137C16B966ABAC1DF4CB893AF421EA0BE61A52266AA7B3AA993900F7139640F06D99D70A1018A182CF51901FCF500F500F500F502816B555344432D633736663166");
    auto expectedUR = std::string("ur:detailed-account/oeadtantjloeaxhdclaefpemsejemtimrdseurgsromupefwcknbrnhsoncpiypkkgftptmumhbsjsesiebsamtantjooyadlecsdwykcfadztykaeykaeykaeykaolyjegogufyfxdpiaemeniyehiyronnkiuy");

    ValidateCborResults(detailedAccount_uai, expectedBytes);
    ValidateUrEncoding(detailedAccount_uai, expectedUR);
}

void TestDetailedAccount::UaiExampleLtubLegacy()
{
    DetailedAccount detailedAccount_uai;

    // LTC Legacy with Ltub xpub
    // {1: 40308({1: "pkh(@0)",
    //           2: [40303({3: h'029A4BB443B79E8D2F1440D87F593735AF394C9C8417B28DB29681DF463D30E6E1',
    //                      4: h'1652CEFDB06935CFBC8765DD56287210301613CB4F90B356F6BBD9E7A6BAA846',
    //                      8: 2714334149})]})
    // }
    CPPUNIT_ASSERT_NO_THROW(detailedAccount_uai.setDetailedAccount("uai://secp256k1.2@pkh", "Ltub2ZGrMN5pRdgrvkpENeeEzrSHLhWBHHTod8xragphYPGzw4dE4LDh14Row7feeEESRP4LKzV99BzSQEq41FrG7dMhcG3PvQ3qAKfDPwpCFhb"));
    auto expectedBytes = fromHex("A101D99D74A20167706B68284030290281D99D6FA3035821029A4BB443B79E8D2F1440D87F593735AF394C9C8417B28DB29681DF463D30E6E10458201652CEFDB06935CFBC8765DD56287210301613CB4F90B356F6BBD9E7A6BAA846081AA1C973C5");
    auto expectedUR = std::string("ur:detailed-account/oyadtantjyoeadiojojeisdefzdydtaolytantjlotaxhdclaonygrqzfxrlnnlgdlbbfztplbhkemecpeesgsnslrchprlgprmtlyurfgfsdyvavyaahdcxcmgmtozcpfinectkrfltihuthfdejpbedycmbwsbgwmhqdhfynrktavdolrdpdfgaycyoysojksketwzbwfp");

    ValidateCborResults(detailedAccount_uai, expectedBytes);
    ValidateUrEncoding(detailedAccount_uai, expectedUR);
}

void TestDetailedAccount::UaiExampleBtcZpubNativeSegwit()
{
    DetailedAccount detailedAccount_uai;

    // BTC Native Segwit with zpub
    // {1: 40308({1: "wpkh(@0)",
    //            2: [40303({3: h'027601C36582260D43FF6469418045B735F76558454B877B383FB5E0D4057959BD',
    //                       4: h'3389E29E33E9A43EA55C16F958CF61B03E236DE145CCFA55CF8DB5CE72F3E84B',
    //                       8: 233185855})]})
    // }
    CPPUNIT_ASSERT_NO_THROW(detailedAccount_uai.setDetailedAccount("uai://secp256k1.0@wpkh", "zpub6qRDpfdaPNHmConZmgoNH3eHj7fVHbm8QvpQaRZDu5y3iRtseBZRCd3VXhRMVY6RUbQWmr9gNiAeCpjYm2wUWg13Mh4ArpoJb9VBdKxnAvX"));
    auto expectedBytes = fromHex("A101D99D74A2016877706B68284030290281D99D6FA3035821027601C36582260D43FF6469418045B735F76558454B877B383FB5E0D4057959BD0458203389E29E33E9A43EA55C16F958CF61B03E236DE145CCFA55CF8DB5CE72F3E84B081A0DE6223F");
    auto expectedUR = std::string("ur:detailed-account/oyadtantjyoeadisktjojeisdefzdydtaolytantjlotaxhdclaokoadsrihlfdsbtfxzmieinfplaferlecylihhdfegrltkgetfhrevttyahkkhkryaahdcxeoldvonneowloxfmonhhcmythdtkhspffmcnjnvyfesfzsgotklgretojpwfvsgraycybtvacpfhbwfdeogl");

    ValidateCborResults(detailedAccount_uai, expectedBytes);
    ValidateUrEncoding(detailedAccount_uai, expectedUR);
}

void TestDetailedAccount::InvalidPubKey()
{
    DetailedAccount invalidDetailedAccount_uai;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Invalid hex character in pubkey should generate an error",
        invalidDetailedAccount_uai.setDetailedAccount("uai://ed25519.508", "4137c16b966abac1df4cb893af421ea0be61a52266aa7b3aa993900f713964ZE"),
        CborException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Incorrect pubkey length should generate an error",
        invalidDetailedAccount_uai.setDetailedAccount("uai://ed25519.508", "4137c16b966abac1df4cb893af421ea0be61a52266aa7b3aa993900f71396400543587"),
        CborException);
}

void TestDetailedAccount::InvalidUai()
{
    DetailedAccount invalidDetailedAccount_uai;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Invalid derivation path should generate an error",
        invalidDetailedAccount_uai.setDetailedAccount("uai://secp256k1.0/5/5b", "xpub6CRQif2S43vtEYf5cZdMhrFpuBFkgFkALM6qhJZz7ws2cfEf1f8Jiv8dXSkizobckHyfH1mFDFZn46AJoh8d4FpB6ydFFg49yPkJF69GsHq"),
        CborException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Invalid output descriptor function should generate an error",
        invalidDetailedAccount_uai.setDetailedAccount("uai://secp256k1.0@test/5/5", "xpub6CRQif2S43vtEYf5cZdMhrFpuBFkgFkALM6qhJZz7ws2cfEf1f8Jiv8dXSkizobckHyfH1mFDFZn46AJoh8d4FpB6ydFFg49yPkJF69GsHq"),
        CborException);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Invalid UAI format should generate an error",
        invalidDetailedAccount_uai.setDetailedAccount("uai:/?secp256k1.0/5/5b", "xpub6CRQif2S43vtEYf5cZdMhrFpuBFkgFkALM6qhJZz7ws2cfEf1f8Jiv8dXSkizobckHyfH1mFDFZn46AJoh8d4FpB6ydFFg49yPkJF69GsHq"),
        CborException);
}