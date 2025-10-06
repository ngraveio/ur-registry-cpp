#include "Test_HDKey.h"
#include "Test_UrHelpers.h"

void TestHDKey::setUp(void)
{

}

void TestHDKey::tearDown(void)
{
    
}

ChainCode convertToChainCode(const std::vector<uint8_t>& vec) {
    if (vec.size() != CHAIN_CODE_SIZE) {
        throw std::invalid_argument("Vector size does not match CHAIN_CODE_SIZE.");
    }

    ChainCode result;
    std::copy(vec.begin(), vec.end(), result.begin());
    return result;
}

/**
 * @brief Test the encoding of the test vector 1 of BCR-2020-007-hdkey paper
 * Source: https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-2020-007-hdkey.md
 * Key: xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi
 * CDDL:
 * {
 *    1: true, / is-master /
 *    3: h'00e8f32e723decf4051aefac8e2c93c9c5b214313817cdb01a1494b917c8436b35', / key-data /
 *    4: h'873dff81c02f525623fd1fe5167eac3a55a049de3d314bb42ee227ffed37d508' / chain-code /
 * }
 * 
 */
void TestHDKey::EncodeMasterKey(void)
{
    HDKey hdkey;
    auto keydata = fromHex("00e8f32e723decf4051aefac8e2c93c9c5b214313817cdb01a1494b917c8436b35");
    auto chaincode = fromHex("873dff81c02f525623fd1fe5167eac3a55a049de3d314bb42ee227ffed37d508");
    hdkey.setIsMaster(true);
    hdkey.setKeyData(convertToKeyData(keydata));
    hdkey.setChainCode(convertToChainCode(chaincode));

    auto expectedBytes = fromHex("a301f503582100e8f32e723decf4051aefac8e2c93c9c5b214313817cdb01a1494b917c8436b35045820873dff81c02f525623fd1fe5167eac3a55a049de3d314bb42ee227ffed37d508");
    auto expectedLegacyUR = std::string("ur:crypto-hdkey/otadykaxhdclaevswfdmjpfswpwkahcywspsmndwmusoskprbbehetchsnpfcybbmwrhchspfxjeecaahdcxltfszmlyrtdlgmhfcnzcctvwcmkbpsftgonbgauefsehgrqzdmvodizmweemtlaybakiylat");
    auto expectedNewUR = std::string("ur:hdkey/otadykaxhdclaevswfdmjpfswpwkahcywspsmndwmusoskprbbehetchsnpfcybbmwrhchspfxjeecaahdcxltfszmlyrtdlgmhfcnzcctvwcmkbpsftgonbgauefsehgrqzdmvodizmweemtlaybakiylat");

    ValidateCborResults(hdkey, expectedBytes);
    hdkey.setUseLegacyType(true);
    ValidateUrEncoding(hdkey, expectedLegacyUR);
    hdkey.setUseLegacyType(false);
    ValidateUrEncoding(hdkey, expectedNewUR);

    // Encode same example using deserialization method
    HDKey hdkey_bip32_key;
    CPPUNIT_ASSERT_NO_THROW(hdkey_bip32_key.deserializeFromBIP32("xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi"));
    ValidateCborResults(hdkey_bip32_key, expectedBytes);
    ValidateUrEncoding(hdkey_bip32_key, expectedNewUR);
}

/**
 * @brief Test the encoding of the test vector 2 of BCR-2020-007-hdkey paper
 * Source: https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-2020-007-hdkey.md
 * Key: tpubDHW3GtnVrTatx38EcygoSf9UhUd9Dx1rht7FAL8unrMo8r2NWhJuYNqDFS7cZFVbDaxJkV94MLZAr86XFPsAPYcoHWJ7sWYsrmHDw5sKQ2K
 * CDDL:
 * {
 *    3: h'026fe2355745bb2db3630bbc80ef5d58951c963c841f54170ba6e5c12be7fc12a6', / key-data /
 *    4: h'ced155c72456255881793514edc5bd9447e7f74abb88c6d6b6480fd016ee8c85', / chain-code /
 *    5: 40305({ / use-info /
 *        2: 1 / network: testnet-btc /
 *    }),
 *    6: 40304({ / origin /
 *        1: [44, true, 1, true, 1, true, 0, false, 1, false] / components `m/44'/1'/1'/0/1` /
 *    }),
 *    8: 3910671603 / parent-fingerprint /
 * }
 * 
 */
void TestHDKey::EncodeBtcTesnetPubKey(void)
{
    // Set data to encode
    HDKey hdkey;
    auto keydata = fromHex("026fe2355745bb2db3630bbc80ef5d58951c963c841f54170ba6e5c12be7fc12a6");
    auto chaincode = fromHex("ced155c72456255881793514edc5bd9447e7f74abb88c6d6b6480fd016ee8c85");

    hdkey.setKeyData(convertToKeyData(keydata));
    hdkey.setChainCode(convertToChainCode(chaincode));

    std::string derivationPath = "m/44'/1'/1'/0/1";
    CPPUNIT_ASSERT_NO_THROW(hdkey.setOrigin(derivationPath));

    CoinInfo useInfo;
    useInfo.setNetwork(NetworkType::BtcTestNet);
    hdkey.setUseInfo(useInfo);
    
    hdkey.setParentFingerprint(3910671603);

    // Encode and verify result
    auto expectedBytes = fromHex("a5035821026fe2355745bb2db3630bbc80ef5d58951c963c841f54170ba6e5c12be7fc12a6045820ced155c72456255881793514edc5bd9447e7f74abb88c6d6b6480fd016ee8c8505d99d71a1020106d99d70a1018a182cf501f501f500f401f4081ae9181cf3");
    auto expectedNewUR = std::string("ur:hdkey/onaxhdclaojlvoechgferkdpqdiabdrflawshlhdmdcemtfnlrctghchbdolvwsednvdztbgolaahdcxtottgostdkhfdahdlykkecbbweskrymwflvdylgerkloswtbrpfdbsticmwylklpahtantjsoyaoadamtantjooyadlecsdwykadykadykaewkadwkaycywlcscewfjnkpvllt");
    auto expectedLegacyBytes = fromHex("A5035821026FE2355745BB2DB3630BBC80EF5D58951C963C841F54170BA6E5C12BE7FC12A6045820CED155C72456255881793514EDC5BD9447E7F74ABB88C6D6B6480FD016EE8C8505D90131A1020106D90130A1018A182CF501F501F500F401F4081AE9181CF3");
    auto expectedLegacyUR = std::string("ur:crypto-hdkey/onaxhdclaojlvoechgferkdpqdiabdrflawshlhdmdcemtfnlrctghchbdolvwsednvdztbgolaahdcxtottgostdkhfdahdlykkecbbweskrymwflvdylgerkloswtbrpfdbsticmwylklpahtaadehoyaoadamtaaddyoyadlecsdwykadykadykaewkadwkaycywlcscewfihbdaehn");

    hdkey.setUseLegacyType(true);
    ValidateCborResults(hdkey, expectedLegacyBytes);
    ValidateUrEncoding(hdkey, expectedLegacyUR);
    hdkey.setUseLegacyType(false);
    ValidateCborResults(hdkey, expectedBytes);
    ValidateUrEncoding(hdkey, expectedNewUR);

    // Encode same example using deserialization method
    HDKey hdkey_bip32_key;
    CPPUNIT_ASSERT_NO_THROW(hdkey_bip32_key.deserializeFromBIP32("tpubDHW3GtnVrTatx38EcygoSf9UhUd9Dx1rht7FAL8unrMo8r2NWhJuYNqDFS7cZFVbDaxJkV94MLZAr86XFPsAPYcoHWJ7sWYsrmHDw5sKQ2K"));
    hdkey_bip32_key.setUseInfo(useInfo);
    hdkey_bip32_key.setOrigin(derivationPath);
    ValidateCborResults(hdkey_bip32_key, expectedBytes);
    ValidateUrEncoding(hdkey_bip32_key, expectedNewUR);
}

/**
 * @brief Test the encoding of the layer 1 test vector of NBCR-2023-002-multi-layer-sync paper
 * Source: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2023-002-multi-layer-sync.md
 * Key: xpub6CRQif2S43vtEYf5cZdMhrFpuBFkgFkALM6qhJZz7ws2cfEf1f8Jiv8dXSkizobckHyfH1mFDFZn46AJoh8d4FpB6ydFFg49yPkJF69GsHq
 * CDDL:
 * {
 *  1: false, 
 *  3: h'0204FA032947600AAE94A889DFC31B5C96AFCB7F7E23A189114B71E0254C4B88B5', 
 *  4: h'1D710954E58073F927F283C61BC1428AC30A3D86D04DACB3A78613401F781839', 
 *  5: 40305({2: 0}), 
 *  6: 40304({1: [44, true, 60, true, 0, true], 2: 1756817576}), 
 *  8: 1756817576, 
 *  9: "NGRAVE"
 * }
 * 
 * Legacy version:
 * {
 *  1: false, 
 *  3: h'0204FA032947600AAE94A889DFC31B5C96AFCB7F7E23A189114B71E0254C4B88B5', 
 *  4: h'1D710954E58073F927F283C61BC1428AC30A3D86D04DACB3A78613401F781839', 
 *  5: 305({2: 0}), 
 *  6: 304({1: [44, true, 60, true, 0, true], 2: 1756817576}), 
 *  8: 1756817576, 
 *  9: "NGRAVE"
 * }
 * 
 */
void TestHDKey::EncodeEthXpub(void)
{
    // Set data to encode
    HDKey hdkey;
    auto keydata = fromHex("0204FA032947600AAE94A889DFC31B5C96AFCB7F7E23A189114B71E0254C4B88B5");
    auto chaincode = fromHex("1D710954E58073F927F283C61BC1428AC30A3D86D04DACB3A78613401F781839");
    
    hdkey.setIsMaster(false);
    hdkey.setKeyData(convertToKeyData(keydata));
    hdkey.setChainCode(convertToChainCode(chaincode));

    std::string derivationPath = "m/44'/60'/0'";
    CPPUNIT_ASSERT_NO_THROW(hdkey.setOrigin(derivationPath, 1756817576));

    hdkey.setUseInfo(NetworkType::MainNet);
    hdkey.setParentFingerprint(1756817576);
    hdkey.setName("NGRAVE");

    // Encode and verify result
    auto expectedBytes = fromHex("A701F40358210204FA032947600AAE94A889DFC31B5C96AFCB7F7E23A189114B71E0254C4B88B50458201D710954E58073F927F283C61BC1428AC30A3D86D04DACB3A78613401F78183905D99D71A1020006D99D70A20186182CF5183CF500F5021A68B6E8A8081A68B6E8A809664E4752415645");
    auto expectedNewUR = std::string("ur:hdkey/osadwkaxhdclaoaazsaxdtflhnbkplmwpdldursrcwhhmtpesblbkbcnoyldbygrjsvtdagsgrloreaahdcxcajsasghvwlajkytdiwzlsswcwsefwlesrbkfslntigtpsqdoslnbwfzctkscsesahtantjsoyaoaeamtantjooeadlncsdwykcsfnykaeykaocyisrpvspdaycyisrpvspdasiyglflgmfphfferttovymh");
    auto expectedLegacyBytes = fromHex("A701F40358210204FA032947600AAE94A889DFC31B5C96AFCB7F7E23A189114B71E0254C4B88B50458201D710954E58073F927F283C61BC1428AC30A3D86D04DACB3A78613401F78183905D90131A1020006D90130A20186182CF5183CF500F5021A68B6E8A8081A68B6E8A809664E4752415645");
    auto expectedLegacyUR = std::string("ur:crypto-hdkey/osadwkaxhdclaoaazsaxdtflhnbkplmwpdldursrcwhhmtpesblbkbcnoyldbygrjsvtdagsgrloreaahdcxcajsasghvwlajkytdiwzlsswcwsefwlesrbkfslntigtpsqdoslnbwfzctkscsesahtaadehoyaoaeamtaaddyoeadlncsdwykcsfnykaeykaocyisrpvspdaycyisrpvspdasiyglflgmfphffeytmevoee");

    hdkey.setUseLegacyType(true);
    ValidateCborResults(hdkey, expectedLegacyBytes);
    ValidateUrEncoding(hdkey, expectedLegacyUR);
    hdkey.setUseLegacyType(false);
    ValidateCborResults(hdkey, expectedBytes);
    ValidateUrEncoding(hdkey, expectedNewUR);

    // Encode same example using deserialization method
    HDKey hdkey_bip32_key;
    CPPUNIT_ASSERT_NO_THROW(hdkey_bip32_key.deserializeFromBIP32("xpub6CRQif2S43vtEYf5cZdMhrFpuBFkgFkALM6qhJZz7ws2cfEf1f8Jiv8dXSkizobckHyfH1mFDFZn46AJoh8d4FpB6ydFFg49yPkJF69GsHq"));
    hdkey_bip32_key.setIsMaster(false);
    hdkey_bip32_key.setUseInfo(NetworkType::MainNet);
    hdkey_bip32_key.setOrigin(derivationPath, 1756817576);
    hdkey_bip32_key.setName("NGRAVE");
    ValidateCborResults(hdkey_bip32_key, expectedBytes);
    ValidateUrEncoding(hdkey_bip32_key, expectedNewUR);
}

/**
 * @brief Test the encoding of this CDDL example
 * {
 *  2: true, 
 *  3: h'00e8f32e723decf4051aefac8e2c93c9c5b214313817cdb01a1494b917c8436b35', 
 *  4: h'873dff81c02f525623fd1fe5167eac3a55a049de3d314bb42ee227ffed37d508', 
 *  6: 40304({1: [44, true, 60, true, 0, true]}), 
 *  7: 40304({1: [0, true, [0, 10], true]}), 
 *  9: "Test", 
 *  10: "This is an xpriv with hardened children"
 * }
 * 
 * Legacy version:
 * {
 *  2: true, 
 *  3: h'00e8f32e723decf4051aefac8e2c93c9c5b214313817cdb01a1494b917c8436b35', 
 *  4: h'873dff81c02f525623fd1fe5167eac3a55a049de3d314bb42ee227ffed37d508', 
 *  6: 304({1: [44, true, 60, true, 0, true]}), 
 *  7: 304({1: [0, true, [0, 10], true]}), 
 *  9: "Test", 
 *  10: "This is an xpriv with hardened children"
 * }
 * 
 */
void TestHDKey::EncodeEthXprvWithHardenedChildren(void)
{
    // Set data to encode
    HDKey hdkey;
    auto keydata = fromHex("00e8f32e723decf4051aefac8e2c93c9c5b214313817cdb01a1494b917c8436b35");
    auto chaincode = fromHex("873dff81c02f525623fd1fe5167eac3a55a049de3d314bb42ee227ffed37d508");

    hdkey.setIsPrivate(true);
    hdkey.setKeyData(convertToKeyData(keydata));
    hdkey.setChainCode(convertToChainCode(chaincode));

    std::string derivationPath = "m/44'/60'/0'";
    CPPUNIT_ASSERT_NO_THROW(hdkey.setOrigin(derivationPath));
    
    derivationPath = "m/0'/[0,10]'";
    CPPUNIT_ASSERT_NO_THROW(hdkey.setChildren(derivationPath));

    hdkey.setName("Test");
    hdkey.setNote("This is an xpriv with hardened children");

    // Encode and verify result
    auto expectedBytes = fromHex("A702F503582100E8F32E723DECF4051AEFAC8E2C93C9C5B214313817CDB01A1494B917C8436B35045820873DFF81C02F525623FD1FE5167EAC3A55A049DE3D314BB42EE227FFED37D50806D99D70A10186182CF5183CF500F507D99D70A1018400F582000AF50964546573740A78275468697320697320616E20787072697620776974682068617264656E6564206368696C6472656E");
    auto expectedNewUR = std::string("ur:hdkey/osaoykaxhdclaevswfdmjpfswpwkahcywspsmndwmusoskprbbehetchsnpfcybbmwrhchspfxjeecaahdcxltfszmlyrtdlgmhfcnzcctvwcmkbpsftgonbgauefsehgrqzdmvodizmweemtlayamtantjooyadlncsdwykcsfnykaeykattantjooyadlraeyklfaebkykasieghihjkjybkksdighisinjkcxinjkcxhsjtcxksjojpinkocxktinjyiscxishsjpieihjtihiecxiaisinjziejpihjtjzetfpzt");
    auto expectedLegacyBytes = fromHex("A702F503582100E8F32E723DECF4051AEFAC8E2C93C9C5B214313817CDB01A1494B917C8436B35045820873DFF81C02F525623FD1FE5167EAC3A55A049DE3D314BB42EE227FFED37D50806D90130A10186182CF5183CF500F507D90130A1018400F582000AF50964546573740A78275468697320697320616E20787072697620776974682068617264656E6564206368696C6472656E");
    auto expectedLegacyUR = std::string("ur:crypto-hdkey/osaoykaxhdclaevswfdmjpfswpwkahcywspsmndwmusoskprbbehetchsnpfcybbmwrhchspfxjeecaahdcxltfszmlyrtdlgmhfcnzcctvwcmkbpsftgonbgauefsehgrqzdmvodizmweemtlayamtaaddyoyadlncsdwykcsfnykaeykattaaddyoyadlraeyklfaebkykasieghihjkjybkksdighisinjkcxinjkcxhsjtcxksjojpinkocxktinjyiscxishsjpieihjtihiecxiaisinjziejpihjtlybdssfm");
    
    hdkey.setUseLegacyType(true);
    ValidateCborResults(hdkey, expectedLegacyBytes);
    ValidateUrEncoding(hdkey, expectedLegacyUR);
    hdkey.setUseLegacyType(false);
    ValidateCborResults(hdkey, expectedBytes);
    ValidateUrEncoding(hdkey, expectedNewUR);
}

/**
 * @brief Test the encoding of this CDDL example
 * {
 *  3: h'02a1f7b8c912d4e34a12c5b9f83e2ac5d3b4a7e12d3c9127f45b9e34a2f1d3c99b', 
 *  4: h'd3a7c9e2b134f7b89a12c45ed3a912f45bd7e123af9c5d3c45a12ed78a345d9f', 
 *  6: 40304({1: [600, false, [1, false, 2, true]]}), 
 *  7: 40304({1: [0, false, [], false]}),
 *  8: 908765, 
 *  9: "NGR@v3", 
 *  10: "test!$"
 * }
 * 
 * Legacy version:
 * {
 *  3: h'02a1f7b8c912d4e34a12c5b9f83e2ac5d3b4a7e12d3c9127f45b9e34a2f1d3c99b', 
 *  4: h'd3a7c9e2b134f7b89a12c45ed3a912f45bd7e123af9c5d3c45a12ed78a345d9f', 
 *  6: 304({1: [600, false, [1, false, 2, true]]}), 
 *  7: 304({1: [0, false, [], false]}),
 *  8: 908765, 
 *  9: "NGR@v3", 
 *  10: "test!$"
 * }
 * 
 */
void TestHDKey::EncodeRandomXpubWithChildren(void)
{
    // Set data to encode
    HDKey hdkey;
    auto keydata = fromHex("02a1f7b8c912d4e34a12c5b9f83e2ac5d3b4a7e12d3c9127f45b9e34a2f1d3c99b");
    auto chaincode = fromHex("d3a7c9e2b134f7b89a12c45ed3a912f45bd7e123af9c5d3c45a12ed78a345d9f");

    hdkey.setKeyData(convertToKeyData(keydata));
    hdkey.setChainCode(convertToChainCode(chaincode));

    std::string derivationPath = "m/600/<1;2'>";
    CPPUNIT_ASSERT_NO_THROW(hdkey.setOrigin(derivationPath));
    
    derivationPath = "m/0/[]";
    CPPUNIT_ASSERT_NO_THROW(hdkey.setChildren(derivationPath));

    hdkey.setParentFingerprint(908765);
    hdkey.setName("NGR@v3");
    hdkey.setNote("test!$");

    // Encode and verify result
    auto expectedBytes = fromHex("A703582102A1F7B8C912D4E34A12C5B9F83E2AC5D3B4A7E12D3C9127F45B9E34A2F1D3C99B045820D3A7C9E2B134F7B89A12C45ED3A912F45BD7E123AF9C5D3C45A12ED78A345D9F06D99D70A10183190258F48401F402F507D99D70A1018400F480F4081A000DDDDD09664E47524076330A66746573742124");
    auto expectedNewUR = std::string("ur:hdkey/osaxhdclaooyylrosobgtyvlgebgskrhyafmdrskteqzosvydpfnmediwkhpnneeoewntesondaahdcxteossovopaeeylronybgsshyteptbgwkhptsvycnpenshlfnfeoydmtsleeehlneamtantjooyadlscfaohdwklradwkaoykattantjooyadlraewklawkaycyaebtututasiyglflgmfzkoeobkiyjyihjkjycldkcsfzbknn");
    auto expectedLegacyBytes = fromHex("A703582102A1F7B8C912D4E34A12C5B9F83E2AC5D3B4A7E12D3C9127F45B9E34A2F1D3C99B045820D3A7C9E2B134F7B89A12C45ED3A912F45BD7E123AF9C5D3C45A12ED78A345D9F06D90130A10183190258F48401F402F507D90130A1018400F480F4081A000DDDDD09664E47524076330A66746573742124");
    auto expectedLegacyUR = std::string("ur:crypto-hdkey/osaxhdclaooyylrosobgtyvlgebgskrhyafmdrskteqzosvydpfnmediwkhpnneeoewntesondaahdcxteossovopaeeylronybgsshyteptbgwkhptsvycnpenshlfnfeoydmtsleeehlneamtaaddyoyadlscfaohdwklradwkaoykattaaddyoyadlraewklawkaycyaebtututasiyglflgmfzkoeobkiyjyihjkjycldkvegswkct");
    
    hdkey.setUseLegacyType(true);
    ValidateCborResults(hdkey, expectedLegacyBytes);
    ValidateUrEncoding(hdkey, expectedLegacyUR);
    hdkey.setUseLegacyType(false);
    ValidateCborResults(hdkey, expectedBytes);
    ValidateUrEncoding(hdkey, expectedNewUR);
}

/**
 * @brief Test the encoding of this CDDL example
 * {
 *  3: h'02e913d4c5a3f7b89c3e2a14c5b9f7d2a12345e9b34c12a78d3f5c9e1234b7a967', 
 *  5: 40305({1: 0}),
 *  6: 40304({1: [44, true, 0, true, 0, true]}),
 *  7: 40304({1: [160000, false, 45, false]})
 * }
 * 
 * Legacy version:
 * {
 *  3: h'02e913d4c5a3f7b89c3e2a14c5b9f7d2a12345e9b34c12a78d3f5c9e1234b7a967', 
 *  5: 305({1: 0}),
 *  6: 304({1: [44, true, 0, true, 0, true]}),
 *  7: 304({1: [160000, false, 45, false]})
 * }
 * 
 */
void TestHDKey::EncodeDerivedKeyWithBtcBIP44DefinedInUseInfo(void)
{
    // Set data to encode
    HDKey hdkey;
    auto keydata = fromHex("02e913d4c5a3f7b89c3e2a14c5b9f7d2a12345e9b34c12a78d3f5c9e1234b7a967");

    hdkey.setKeyData(convertToKeyData(keydata));

    std::string derivationPath = "m/44'/0'/0'";
    CPPUNIT_ASSERT_NO_THROW(hdkey.setOrigin(derivationPath));
    
    derivationPath = "m/160000/45";
    CPPUNIT_ASSERT_NO_THROW(hdkey.setChildren(derivationPath));

    hdkey.setUseInfo(CoinType::Bitcoin);

    // Encode and verify result
    auto expectedBytes = fromHex("A403582102E913D4C5A3F7B89C3E2A14C5B9F7D2A12345E9B34C12A78D3F5C9E1234B7A96705D99D71A1010006D99D70A10186182CF500F500F507D99D70A101841A00027100F4182DF4");
    auto expectedNewUR = std::string("ur:hdkey/oxaxhdclaowlbwtyskotylronsfmdrbbskrhyltdoycnfewlqdgsbgoslgfhhhnnbgeerlptioahtantjsoyadaeamtantjooyadlncsdwykaeykaeykattantjooyadlrcyaeaojsaewkcsdpwkoycfbesp");
    auto expectedLegacyBytes = fromHex("A403582102E913D4C5A3F7B89C3E2A14C5B9F7D2A12345E9B34C12A78D3F5C9E1234B7A96705D90131A1010006D90130A10186182CF500F500F507D90130A101841A00027100F4182DF4");
    auto expectedLegacyUR = std::string("ur:crypto-hdkey/oxaxhdclaowlbwtyskotylronsfmdrbbskrhyltdoycnfewlqdgsbgoslgfhhhnnbgeerlptioahtaadehoyadaeamtaaddyoyadlncsdwykaeykaeykattaaddyoyadlrcyaeaojsaewkcsdpwkeyiytevo");
    
    hdkey.setUseLegacyType(true);
    ValidateCborResults(hdkey, expectedLegacyBytes);
    ValidateUrEncoding(hdkey, expectedLegacyUR);
    hdkey.setUseLegacyType(false);
    ValidateCborResults(hdkey, expectedBytes);
    ValidateUrEncoding(hdkey, expectedNewUR);
}

/**
 * @brief Test the encoding of this CDDL example
 * {
 *   3: h'023a7c9d8e12b3f5a9c45d2a12345b78e9d3a7c12f5e34b9c12a7f45b9d3a12ebf'
 * }
 * 
 */
void TestHDKey::EncodeSingleKeyData(void)
{
    // Set data to encode
    HDKey hdkey;
    auto keydata = fromHex("023a7c9d8e12b3f5a9c45d2a12345b78e9d3a7c12f5e34b9c12a7f45b9d3a12ebf");

    hdkey.setKeyData(convertToKeyData(keydata));

    // Encode and verify result
    auto expectedBytes = fromHex("A1035821023A7C9D8E12B3F5A9C45D2A12345B78E9D3A7C12F5E34B9C12A7F45B9D3A12EBF");
    auto expectedNewUR = std::string("ur:hdkey/oyaxhdclaoftkentmnbgqdykptsshldrbgeehpkswlteossedlhyeerhsedrlbferhteoydmrsvyditiwk");
    auto expectedLegacyUR = std::string("ur:crypto-hdkey/oyaxhdclaoftkentmnbgqdykptsshldrbgeehpkswlteossedlhyeerhsedrlbferhteoydmrsvyditiwk");
    
    ValidateCborResults(hdkey, expectedBytes);
    hdkey.setUseLegacyType(true);
    ValidateUrEncoding(hdkey, expectedLegacyUR);
    hdkey.setUseLegacyType(false);
    ValidateUrEncoding(hdkey, expectedNewUR);
}

/**
 * @brief Test the encoding of this CDDL example
 * {
 *  1: false,
 *  2: true,
 *  3: h'005c3d2e93cfe413e9a1b3f6edc9314b3a9127e4c5d9f83acb4512e7c34a12d43e', 
 *  4: h'bd1345c7abf12ed3458af7c9e2b13a7c9f452ed3a123a7bd45f12e9d37c5b43c', 
 *  5: 40305({1: 60, 2: 4}),
 *  6: 40304({1: [44, true, 60, true, 0, true], 2: 4509876, 3: 3}), 
 *  7: 40304({1: [[0, 5], true, [], false], 2: 1080655, 3: 2}),
 *  8: 4509876, 
 *  9: "ZERO", 
 *  10: "Fully defined HDKey"
 * }
 * 
 * Legacy version:
 * {
 *  1: false,
 *  2: true,
 *  3: h'005c3d2e93cfe413e9a1b3f6edc9314b3a9127e4c5d9f83acb4512e7c34a12d43e', 
 *  4: h'bd1345c7abf12ed3458af7c9e2b13a7c9f452ed3a123a7bd45f12e9d37c5b43c', 
 *  5: 305({1: 60, 2: 4}),
 *  6: 304({1: [44, true, 60, true, 0, true], 2: 4509876, 3: 3}), 
 *  7: 304({1: [[0, 5], true, [], false], 2: 1080655, 3: 2}),
 *  8: 4509876, 
 *  9: "ZERO", 
 *  10: "Fully defined HDKey"
 * }
 * 
 */
void TestHDKey::EncodePrivateKeyFullyDefined(void)
{
    // Set data to encode
    HDKey hdkey;
    auto keydata = fromHex("005c3d2e93cfe413e9a1b3f6edc9314b3a9127e4c5d9f83acb4512e7c34a12d43e");
    auto chaincode = fromHex("bd1345c7abf12ed3458af7c9e2b13a7c9f452ed3a123a7bd45f12e9d37c5b43c");

    hdkey.setIsMaster(false);
    hdkey.setIsPrivate(true);
    hdkey.setKeyData(convertToKeyData(keydata));
    hdkey.setChainCode(convertToChainCode(chaincode));

    std::string derivationPath = "m/44'/60'/0'";
    CPPUNIT_ASSERT_NO_THROW(hdkey.setOrigin(derivationPath, 4509876, true));
    
    derivationPath = "m/[0,5]'/[]";
    CPPUNIT_ASSERT_NO_THROW(hdkey.setChildren(derivationPath, 1080655, true));

    hdkey.setUseInfo(CoinType::Ethereum, NetworkType::EthTestNetGorli);
    hdkey.setParentFingerprint(4509876);
    hdkey.setName("ZERO");
    hdkey.setNote("Fully defined HDKey");

    // Encode and verify result
    auto expectedBytes = fromHex("AA01F402F5035821005C3D2E93CFE413E9A1B3F6EDC9314B3A9127E4C5D9F83ACB4512E7C34A12D43E045820BD1345C7ABF12ED3458AF7C9E2B13A7C9F452ED3A123A7BD45F12E9D37C5B43C05D99D71A201183C020406D99D70A30186182CF5183CF500F5021A0044D0B4030307D99D70A30184820005F580F4021A00107D4F0302081A0044D0B409645A45524F0A7346756C6C7920646566696E65642048444B6579");
    auto expectedNewUR = std::string("ur:hdkey/pkadwkaoykaxhdclaehhfsdmmutkvebwwloyqdynwesoehgrftmedivesktayaftsbfebgvdsrgebgtyfmaahdcxrybwfestpywndmtefeleylsovopaftkenefedmteoycnosryfewndmntemskqzfnahtantjsoeadcsfnaoaaamtantjootadlncsdwykcsfnykaeykaocyaefytiqzaxaxattantjootadlrlfaeahyklawkaocyaebekigwaxaoaycyaefytiqzasiehtfegmgwbkjkfgkpjzjzkkcxieihiyinjtihiecxfdfygrihkksrpytolp");
    auto expectedLegacyBytes = fromHex("AA01F402F5035821005C3D2E93CFE413E9A1B3F6EDC9314B3A9127E4C5D9F83ACB4512E7C34A12D43E045820BD1345C7ABF12ED3458AF7C9E2B13A7C9F452ED3A123A7BD45F12E9D37C5B43C05D90131A201183C020406D90130A30186182CF5183CF500F5021A0044D0B4030307D90130A30184820005F580F4021A00107D4F0302081A0044D0B409645A45524F0A7346756C6C7920646566696E65642048444B6579");
    auto expectedLegacyUR = std::string("ur:crypto-hdkey/pkadwkaoykaxhdclaehhfsdmmutkvebwwloyqdynwesoehgrftmedivesktayaftsbfebgvdsrgebgtyfmaahdcxrybwfestpywndmtefeleylsovopaftkenefedmteoycnosryfewndmntemskqzfnahtaadehoeadcsfnaoaaamtaaddyotadlncsdwykcsfnykaeykaocyaefytiqzaxaxattaaddyotadlrlfaeahyklawkaocyaebekigwaxaoaycyaefytiqzasiehtfegmgwbkjkfgkpjzjzkkcxieihiyinjtihiecxfdfygrihkkchadbevl");
    
    hdkey.setUseLegacyType(true);
    ValidateCborResults(hdkey, expectedLegacyBytes);
    ValidateUrEncoding(hdkey, expectedLegacyUR);
    hdkey.setUseLegacyType(false);
    ValidateCborResults(hdkey, expectedBytes);
    ValidateUrEncoding(hdkey, expectedNewUR);
}

/**
 * @brief Test returning an error
 * 
 */
void TestHDKey::EncodeIncorrectMasterKeyWithoutChainCode(void)
{
    HDKey hdkey;
    auto keydata = fromHex("00e8f32e723decf4051aefac8e2c93c9c5b214313817cdb01a1494b917c8436b35");
    
    hdkey.setIsMaster(true);
    hdkey.setKeyData(convertToKeyData(keydata));
    // Missing chain code to hdkey

    ValidateUrEncodingException(hdkey, CborErrorTooFewItems);
}

/**
 * @brief Test returning an error
 * 
 */
void TestHDKey::EncodeIncorrectMasterKeyWithExcludedFields(void)
{
    HDKey hdkey;
    hdkey.deserializeFromBIP32("xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi");
    // Set fingerprint for master key not allowed
    hdkey.setParentFingerprint(0);

    ValidateUrEncodingException(hdkey, CborErrorIllegalType);
}

/**
 * @brief Test returning an error
 * 
 */
void TestHDKey::EncodeIncorrectParentFingerprintForSingleDerivationPath(void)
{
    HDKey hdkey;
    auto keydata = fromHex("026fe2355745bb2db3630bbc80ef5d58951c963c841f54170ba6e5c12be7fc12a6");
    auto chaincode = fromHex("ced155c72456255881793514edc5bd9447e7f74abb88c6d6b6480fd016ee8c85");

    hdkey.setKeyData(convertToKeyData(keydata));

    // Set single derivation path with different fingerprint 
    std::string derivationPath = "m/44'";
    CPPUNIT_ASSERT_NO_THROW(hdkey.setOrigin(derivationPath, 10101, false));

    hdkey.setParentFingerprint(202);

    ValidateUrEncodingException(hdkey, CborErrorImproperValue);
}

/**
 * @brief Test returning an error
 * 
 */
void TestHDKey::EncodeIncorrectBIP44InUseInfo(void)
{
    HDKey hdkey;
    auto keydata = fromHex("026fe2355745bb2db3630bbc80ef5d58951c963c841f54170ba6e5c12be7fc12a6");
    auto chaincode = fromHex("ced155c72456255881793514edc5bd9447e7f74abb88c6d6b6480fd016ee8c85");

    hdkey.setKeyData(convertToKeyData(keydata));

    // Set BIP44 different than Ethereum (60) specified in use info
    std::string derivationPath = "m/44'/777'/0'";
    CPPUNIT_ASSERT_NO_THROW(hdkey.setOrigin(derivationPath));

    hdkey.setUseInfo(CoinType::Ethereum);

    ValidateUrEncodingException(hdkey, CborErrorImproperValue);
}

/**
 * @brief Test returning an error
 * 
 */
void TestHDKey::EncodeIncorrectBIP44WithTooShortDerivation(void)
{
    HDKey hdkey;
    auto keydata = fromHex("026fe2355745bb2db3630bbc80ef5d58951c963c841f54170ba6e5c12be7fc12a6");
    auto chaincode = fromHex("ced155c72456255881793514edc5bd9447e7f74abb88c6d6b6480fd016ee8c85");

    hdkey.setKeyData(convertToKeyData(keydata));

    // Set BIP44 with too short derivation path
    std::string derivationPath = "m/44'";
    CPPUNIT_ASSERT_NO_THROW(hdkey.setOrigin(derivationPath));

    hdkey.setUseInfo(CoinType::Ethereum);

    ValidateUrEncodingException(hdkey, CborErrorImproperValue);
}

/**
 * @brief Test returning an error
 * 
 */
void TestHDKey::EncodeIncorrectBIP44WithInvalidChildType(void)
{
    HDKey hdkey;
    auto keydata = fromHex("026fe2355745bb2db3630bbc80ef5d58951c963c841f54170ba6e5c12be7fc12a6");
    auto chaincode = fromHex("ced155c72456255881793514edc5bd9447e7f74abb88c6d6b6480fd016ee8c85");

    // Set BIP44 with invalid child type, expecting a child index
    std::string derivationPath = "m/44'/[1,60]'";
    CPPUNIT_ASSERT_NO_THROW(hdkey.setOrigin(derivationPath));

    hdkey.setKeyData(convertToKeyData(keydata));
    hdkey.setUseInfo(CoinType::Ethereum);

    ValidateUrEncodingException(hdkey, CborErrorImproperValue);
}

/**
 * @brief Test returning an error
 * 
 */
void TestHDKey::EncodeIncorrectHardenedChildren(void)
{
    // Set data to encode
    HDKey hdkey;
    auto keydata = fromHex("005c3d2e93cfe413e9a1b3f6edc9314b3a9127e4c5d9f83acb4512e7c34a12d43e");

    hdkey.setKeyData(convertToKeyData(keydata));

    // Set hardened children without is_private defined 
    std::string derivationPath = "m/5'";
    CPPUNIT_ASSERT_NO_THROW(hdkey.setChildren(derivationPath));

    ValidateUrEncodingException(hdkey, CborErrorImproperValue);
}

/**
 * @brief Test to extract the source fingerprints from different xpub
 * 
 */
void TestHDKey::ExtractXPubParentFingerprints(void)
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Source fingerprint does not match after deserialization",
        static_cast<uint32_t>(1552725994),
        HDKey::extractParentFingerprint("xpub6CLDw1BZtqgJ6v7jKZDh7nnKGkTzgysHHcr9b9WUmVVrST5NHM5Gew57op52MwMZnVefPzaLf2f7Sc9qzsTNHADC3wDBMjkxaLG7KjVFXkC"));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Source fingerprint does not match after deserialization",
        static_cast<uint32_t>(305787212),
        HDKey::extractParentFingerprint("xpub6BnYDGDnB5xCv8Ltci3SQgbrgQMM1FfCLpmTAkRjQ87PQcnSXctVLEmz1sYbcYS3T4tTtybaG7skytEM2aXL9oZ1DYRLi44sUyw6TXpnf8j"));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Source fingerprint does not match after deserialization",
        static_cast<uint32_t>(1906394030),
        HDKey::extractParentFingerprint("xpub6CVDAP5Ae2wxTNoDXtFqBiwyWU13ejtf16LJXbwMXMmW8i8HdDpWdaC75ss8c1oAmsFFvHXvmJi5MCU1nJZUkvJ3ZsHkCzHRwczDheGWrp3"));
}

/**
 * @brief Test to extract the source fingerprints from different xprv
 * 
 */
void TestHDKey::ExtractXPrvParentFingerprints(void)
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Source fingerprint does not match after deserialization",
        static_cast<uint32_t>(3632322520),
        HDKey::extractParentFingerprint("xprvA41z7zogVVwxVSgdKUHDy1SKmdb533PjDz7J6N6mV6uS3ze1ai8FHa8kmHScGpWmj4WggLyQjgPie1rFSruoUihUZREPSL39UNdE3BBDu76"));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Source fingerprint does not match after deserialization",
        static_cast<uint32_t>(3635104055),
        HDKey::extractParentFingerprint("xprv9zFnWC6h2cLgpmSA46vutJzBcfJ8yaJGg8cX1e5StJh45BBciYTRXSd25UEPVuesF9yog62tGAQtHjXajPPdbRCHuWS6T8XA2ECKADdw4Ef"));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Source fingerprint does not match after deserialization",
        static_cast<uint32_t>(832899000),
        HDKey::extractParentFingerprint("xprvA2nrNbFZABcdryreWet9Ea4LvTJcGsqrMzxHx98MMrotbir7yrKCEXw7nadnHM8Dq38EGfSh6dqA9QWTyefMLEcBYJUuekgW4BYPJcr9E7j"));
}

/**
 * @brief Test to get the source fingerprints from invalid extended keys
 * 
 * Source: https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki
 * 
 */
void TestHDKey::ExtractParentFingerprintFromInvalidKeys(void)
{
    // invalid string length
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Deserialization should return an error with a zero fingerprint (invalid string length)",
        static_cast<uint32_t>(0),
        HDKey::extractParentFingerprint("xpub6CLDw1BZtqgJ6v7jKZDh7nnKGkTzgysHHcr9b9WUmVVrST5NHM5Gew57op52MwMZnVefPzaLf2f7Sc9qzsTNHADC3wDBMjkxaLG7KjVFXkCINCORRECT"));

    // pubkey version / prvkey mismatch
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Deserialization should return an error with a zero fingerprint (pubkey version / prvkey mismatch)",
        static_cast<uint32_t>(0),
        HDKey::extractParentFingerprint("xpub661MyMwAqRbcEYS8w7XLSVeEsBXy79zSzH1J8vCdxAZningWLdN3zgtU6LBpB85b3D2yc8sfvZU521AAwdZafEz7mnzBBsz4wKY5fTtTQBm"));

    // prvkey version / pubkey mismatch
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Deserialization should return an error with a zero fingerprint (prvkey version / pubkey mismatch)",
        static_cast<uint32_t>(0),
        HDKey::extractParentFingerprint("xprv9s21ZrQH143K24Mfq5zL5MhWK9hUhhGbd45hLXo2Pq2oqzMMo63oStZzFGTQQD3dC4H2D5GBj7vWvSQaaBv5cxi9gafk7NF3pnBju6dwKvH"));

    // invalid pubkey prefix 04
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Deserialization should return an error with a zero fingerprint (invalid pubkey prefix 04)",
        static_cast<uint32_t>(0),
        HDKey::extractParentFingerprint("xpub661MyMwAqRbcEYS8w7XLSVeEsBXy79zSzH1J8vCdxAZningWLdN3zgtU6Txnt3siSujt9RCVYsx4qHZGc62TG4McvMGcAUjeuwZdduYEvFn"));

    // invalid prvkey prefix 04
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Deserialization should return an error with a zero fingerprint (invalid prvkey prefix 04)",
        static_cast<uint32_t>(0),
        HDKey::extractParentFingerprint("xprv9s21ZrQH143K24Mfq5zL5MhWK9hUhhGbd45hLXo2Pq2oqzMMo63oStZzFGpWnsj83BHtEy5Zt8CcDr1UiRXuWCmTQLxEK9vbz5gPstX92JQ"));

    // invalid pubkey prefix 01
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Deserialization should return an error with a zero fingerprint (invalid pubkey prefix 01)",
        static_cast<uint32_t>(0),
        HDKey::extractParentFingerprint("xpub661MyMwAqRbcEYS8w7XLSVeEsBXy79zSzH1J8vCdxAZningWLdN3zgtU6N8ZMMXctdiCjxTNq964yKkwrkBJJwpzZS4HS2fxvyYUA4q2Xe4"));

    // invalid prvkey prefix 01
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Deserialization should return an error with a zero fingerprint (invalid prvkey prefix 01)",
        static_cast<uint32_t>(0),
        HDKey::extractParentFingerprint("xprv9s21ZrQH143K24Mfq5zL5MhWK9hUhhGbd45hLXo2Pq2oqzMMo63oStZzFAzHGBP2UuGCqWLTAPLcMtD9y5gkZ6Eq3Rjuahrv17fEQ3Qen6J"));

    // zero depth with non-zero parent fingerprint
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Deserialization should return an error with a zero fingerprint (zero depth with non-zero parent fingerprint)",
        static_cast<uint32_t>(0),
        HDKey::extractParentFingerprint("xprv9s2SPatNQ9Vc6GTbVMFPFo7jsaZySyzk7L8n2uqKXJen3KUmvQNTuLh3fhZMBoG3G4ZW1N2kZuHEPY53qmbZzCHshoQnNf4GvELZfqTUrcv"));

    // zero depth with non-zero parent fingerprint
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Deserialization should return an error with a zero fingerprint (zero depth with non-zero parent fingerprint)",
        static_cast<uint32_t>(0),
        HDKey::extractParentFingerprint("xpub661no6RGEX3uJkY4bNnPcw4URcQTrSibUZ4NqJEw5eBkv7ovTwgiT91XX27VbEXGENhYRCf7hyEbWrR3FewATdCEebj6znwMfQkhRYHRLpJ"));

    // zero depth with non-zero index
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Deserialization should return an error with a zero fingerprint (zero depth with non-zero index)",
        static_cast<uint32_t>(0),
        HDKey::extractParentFingerprint("xprv9s21ZrQH4r4TsiLvyLXqM9P7k1K3EYhA1kkD6xuquB5i39AU8KF42acDyL3qsDbU9NmZn6MsGSUYZEsuoePmjzsB3eFKSUEh3Gu1N3cqVUN"));

    // zero depth with non-zero index
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Deserialization should return an error with a zero fingerprint (zero depth with non-zero index)",
        static_cast<uint32_t>(0),
        HDKey::extractParentFingerprint("xpub661MyMwAuDcm6CRQ5N4qiHKrJ39Xe1R1NyfouMKTTWcguwVcfrZJaNvhpebzGerh7gucBvzEQWRugZDuDXjNDRmXzSZe4c7mnTK97pTvGS8"));

    // unknown extended key version
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Deserialization should return an error with a zero fingerprint (unknown extended key version)",
        static_cast<uint32_t>(0),
        HDKey::extractParentFingerprint("DMwo58pR1QLEFihHiXPVykYB6fJmsTeHvyTp7hRThAtCX8CvYzgPcn8XnmdfHGMQzT7ayAmfo4z3gY5KfbrZWZ6St24UVf2Qgo6oujFktLHdHY4"));

    // unknown extended key version
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Deserialization should return an error with a zero fingerprint (unknown extended key version)",
        static_cast<uint32_t>(0),
        HDKey::extractParentFingerprint("DMwo58pR1QLEFihHiXPVykYB6fJmsTeHvyTp7hRThAtCX8CvYzgPcn8XnmdfHPmHJiEDXkTiJTVV9rHEBUem2mwVbbNfvT2MTcAqj3nesx8uBf9"));

    // private key 0 not in 1..n-1
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Deserialization should return an error with a zero fingerprint (private key 0 not in 1..n-1)",
        static_cast<uint32_t>(0),
        HDKey::extractParentFingerprint("xprv9s21ZrQH143K24Mfq5zL5MhWK9hUhhGbd45hLXo2Pq2oqzMMo63oStZzF93Y5wvzdUayhgkkFoicQZcP3y52uPPxFnfoLZB21Teqt1VvEHx"));

    // private key n not in 1..n-1
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Deserialization should return an error with a zero fingerprint (private key n not in 1..n-1)",
        static_cast<uint32_t>(0),
        HDKey::extractParentFingerprint("xprv9s21ZrQH143K24Mfq5zL5MhWK9hUhhGbd45hLXo2Pq2oqzMMo63oStZzFAzHGBP2UuGCqWLTAPLcMtD5SDKr24z3aiUvKr9bJpdrcLg1y3G"));

    // invalid pubkey 020000000000000000000000000000000000000000000000000000000000000007
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Deserialization should return an error with a zero fingerprint (invalid pubkey)",
        static_cast<uint32_t>(0),
        HDKey::extractParentFingerprint("xpub661MyMwAqRbcEYS8w7XLSVeEsBXy79zSzH1J8vCdxAZningWLdN3zgtU6Q5JXayek4PRsn35jii4veMimro1xefsM58PgBMrvdYre8QyULY"));

    // invalid checksum
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Deserialization should return an error with a zero fingerprint (invalid checksum)",
        static_cast<uint32_t>(0),
        HDKey::extractParentFingerprint("xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHL"));
}
