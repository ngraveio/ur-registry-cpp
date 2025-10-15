#include "Test_Keypath.h"
#include "Test_UrHelpers.h"

void TestKeypath::setUp(void)
{

}

void TestKeypath::tearDown(void)
{

}

static void ValidateDerivationPathException(Keypath keypath, const std::string& derivationPath, const CborError error_code)
{
    CPPUNIT_ASSERT_THROW(keypath.setDerivationPath(derivationPath), CborException);
    try {
        keypath.setDerivationPath(derivationPath);
    } catch (const CborException& e) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Set Derivation Path should return an error",
            error_code, 
            e.errorCode()
        );
    }
}

void TestKeypath::Compare(const Keypath& ur_type, const Keypath& expected_ur_type)
{
    // Check for components
    std::vector<KeyPathComponent> decoded_components = ur_type.getKeyPathComponent();
    std::vector<KeyPathComponent> expected_components = expected_ur_type.getKeyPathComponent();
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Components empty mismatch", 
        expected_components.empty(), 
        decoded_components.empty()
    );
    if (!decoded_components.empty()) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Components size mismatch",
            expected_components.size(),
            decoded_components.size()
        );

        for (size_t i = 0; i < decoded_components.size(); ++i) {
            const KeyPathComponent& decoded = decoded_components[i];
            const KeyPathComponent& expected = expected_components[i];

            CPPUNIT_ASSERT_EQUAL_MESSAGE(
                "Component type mismatch at index " + std::to_string(i),
                static_cast<int>(expected.getType()),
                static_cast<int>(decoded.getType())
            );

            switch(decoded.getType()) {
                case KeyPathComponent::Type::ChildIndex: {
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(
                        "Child index mismatch at index " + std::to_string(i),
                        expected.getChildIndexValue(),
                        decoded.getChildIndexValue()  
                    );
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(
                        "Child hardened mismatch at index " + std::to_string(i),
                        expected.getChildIndexIsHardened(),
                        decoded.getChildIndexIsHardened()  
                    );
                    break;
                }
                case KeyPathComponent::Type::ChildRange: {
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(
                        "Child range low index mismatch at index " + std::to_string(i),
                        expected.getLowIndexValue(),
                        decoded.getLowIndexValue()  
                    );
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(
                        "Child range high index mismatch at index " + std::to_string(i),
                        expected.getHighIndexValue(),
                        decoded.getHighIndexValue() 
                    );
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(
                        "Child range hardened mismatch at index " + std::to_string(i),
                        expected.getChildRangeIsHardened(),
                        decoded.getChildRangeIsHardened()  
                    );
                    break;
                }
                case KeyPathComponent::Type::ChildWildcard: {
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(
                        "Child wildcard hardened mismatch at index " + std::to_string(i),
                        expected.getChildWildcardIsHardened(),
                        decoded.getChildWildcardIsHardened()
                    );
                    break;
                }
                case KeyPathComponent::Type::ChildPair: {
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(
                        "Child pair external index mismatch at index " + std::to_string(i),
                        expected.getExternalAddressIndexValue(),  
                        decoded.getExternalAddressIndexValue()  
                    );
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(
                        "Child pair external hardened mismatch at index " + std::to_string(i),
                        expected.getExternalIndexIsHardened(),  
                        decoded.getExternalIndexIsHardened()  
                    );
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(
                        "Child pair internal index mismatch at index " + std::to_string(i),
                        expected.getInternalAddressIndexValue(),  
                        decoded.getInternalAddressIndexValue()  
                    );
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(
                        "Child pair internal hardened mismatch at index " + std::to_string(i),
                        expected.getInternalIndexIsHardened(),  
                        decoded.getInternalIndexIsHardened()  
                    );
                    break;
                }
                default:
                    break;
            }
        }
    };

    // Check for fingerprint (if present)
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Fingerprint presence mismatch",
        expected_ur_type.getSourceFingerprint().has_value(), 
        ur_type.getSourceFingerprint().has_value()
    );
    if (ur_type.getSourceFingerprint().has_value()){
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Fingerprint value mismatch",
            expected_ur_type.getSourceFingerprint().value(), 
            ur_type.getSourceFingerprint().value()
        );
    }

    // Check for depth (if present)
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Depth presence mismatch",
        expected_ur_type.getDepth().has_value(), 
        ur_type.getDepth().has_value()
    );
    if (ur_type.getDepth().has_value()){
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Depth value mismatch",
            expected_ur_type.getDepth().value(), 
            ur_type.getDepth().value()
        );
    }
}

void ValidateDerivationPath(const Keypath& keypath, std::string derivationPath)
{
    std::string retDerivationPath = keypath.getDerivationPath();
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Returned derivation path is not the expected one",
        derivationPath,  
        retDerivationPath                     
    );
}

/**
 * @brief Test the encoding and decoding of this CDDL example
 * {
 *   1: [1, true, 2, false, [3, 4], false, [5, 6], true, [], false, [], true, [7, false, 8, true], [9, true, 0, false]],
 *   2: 123456789,
 *   3: 8
 * }
 * 
 */
void TestKeypath::EncodeAndDecodeAllFieldsAndTypes(void)
{
    // Validate encoding 
    Keypath encode_keypath;
    std::string derivationPath = "m/1'/2/[3,4]/[5,6]'/[]/[]'/<7;8'>/<9';0>";
    CPPUNIT_ASSERT_NO_THROW(encode_keypath.setDerivationPath(derivationPath));

    encode_keypath.setSourceFingerprint(123456789);
    encode_keypath.setDepth();

    auto expectedBytes = fromHex("A3018E01F502F4820304F4820506F580F480F58407F408F58409F500F4021A075BCD150308");
    auto expectedLegacyUR = std::string("ur:crypto-keypath/otadmnadykaowklfaxaawklfahamyklawklayklratwkayyklrasykaewkaocyathpsnbzaxayknjtvwty");
    auto expectedNewUR = std::string("ur:keypath/otadmnadykaowklfaxaawklfahamyklawklayklratwkayyklrasykaewkaocyathpsnbzaxayknjtvwty");

    ValidateDerivationPath(encode_keypath, derivationPath);
    ValidateCborResults(encode_keypath, expectedBytes);
    encode_keypath.setUseLegacyType(true);
    ValidateUrEncoding(encode_keypath, expectedLegacyUR);
    encode_keypath.setUseLegacyType(false);
    ValidateUrEncoding(encode_keypath, expectedNewUR);

    // Validate decoding
    Keypath decode_keypath;
    CPPUNIT_ASSERT_NO_THROW(decode_keypath.fromUr(expectedLegacyUR));
    Compare(decode_keypath, encode_keypath);
}


/**
 * @brief Test the encoding and decoding of this CDDL example
 * {
 *   1: [44, true, 200, false, 50, false, [0, 100], false],
 *   3: 4
 * }
 * 
 */
void TestKeypath::EncodeAndDecodeDerivationPathAndDepth(void)
{
    Keypath encode_keypath;
    std::string derivationPath = "m/44'/200/50/[0,100]";
    CPPUNIT_ASSERT_NO_THROW(encode_keypath.setDerivationPath(derivationPath));

    encode_keypath.setDepth();

    auto expectedBytes = fromHex("A20188182CF518C8F41832F482001864F40304");
    auto expectedLegacyUR = std::string("ur:crypto-keypath/oeadlocsdwykcsspwkcseywklfaecsiewkaxaarsonnbkk");
    auto expectedNewUR = std::string("ur:keypath/oeadlocsdwykcsspwkcseywklfaecsiewkaxaarsonnbkk");

    ValidateDerivationPath(encode_keypath, derivationPath);
    ValidateCborResults(encode_keypath, expectedBytes);
    encode_keypath.setUseLegacyType(true);
    ValidateUrEncoding(encode_keypath, expectedLegacyUR);
    encode_keypath.setUseLegacyType(false);
    ValidateUrEncoding(encode_keypath, expectedNewUR);

    // Validate decoding
    Keypath decode_keypath;
    CPPUNIT_ASSERT_NO_THROW(decode_keypath.fromUr(expectedLegacyUR));
    Compare(decode_keypath, encode_keypath);
}

/**
 * @brief Test the encoding and decoding of this CDDL example
 * {
 *   1: [[44, true, 44, true], 76500, false, 0, false, [], false],
 *   2: 1
 * }
 * 
 */
void TestKeypath::EncodeAndDecodeDerivationPathAndFingerprint(void)
{
    Keypath encode_keypath;
    std::string derivationPath = "m/<44';44'>/76500/0/[]";
    CPPUNIT_ASSERT_NO_THROW(encode_keypath.setDerivationPath(derivationPath));

    encode_keypath.setSourceFingerprint(1);

    auto expectedBytes = fromHex("A2018784182CF5182CF51A00012AD4F400F480F40201");
    auto expectedLegacyUR = std::string("ur:crypto-keypath/oeadltlrcsdwykcsdwykcyaeaddrtywkaewklawkaoadnscsptbk");
    auto expectedNewUR = std::string("ur:keypath/oeadltlrcsdwykcsdwykcyaeaddrtywkaewklawkaoadnscsptbk");

    ValidateDerivationPath(encode_keypath, derivationPath);
    ValidateCborResults(encode_keypath, expectedBytes);
    encode_keypath.setUseLegacyType(true);
    ValidateUrEncoding(encode_keypath, expectedLegacyUR);
    encode_keypath.setUseLegacyType(false);
    ValidateUrEncoding(encode_keypath, expectedNewUR);

    // Validate decoding
    Keypath decode_keypath;
    CPPUNIT_ASSERT_NO_THROW(decode_keypath.fromUr(expectedLegacyUR));
    Compare(decode_keypath, encode_keypath);
}

/**
 * @brief Test the encoding and decoding of this CDDL example
 * {
 *   1: [],
 *   2: 912348765,
 *   3: 0
 * }
 * 
 */
void TestKeypath::EncodeAndDecodeEmptyDerivationPath(void)
{
    Keypath encode_keypath;
    std::string derivationPath = "m";
    CPPUNIT_ASSERT_NO_THROW(encode_keypath.setDerivationPath(derivationPath));

    encode_keypath.setSourceFingerprint(912348765);
    encode_keypath.setDepth();

    auto expectedBytes = fromHex("A30180021A3661565D0300");
    auto expectedLegacyUR = std::string("ur:crypto-keypath/otadlaaocyenhshfhlaxaeuohtswft");
    auto expectedNewUR = std::string("ur:keypath/otadlaaocyenhshfhlaxaeuohtswft");

    ValidateDerivationPath(encode_keypath, derivationPath);
    ValidateCborResults(encode_keypath, expectedBytes);
    encode_keypath.setUseLegacyType(true);
    ValidateUrEncoding(encode_keypath, expectedLegacyUR);
    encode_keypath.setUseLegacyType(false);
    ValidateUrEncoding(encode_keypath, expectedNewUR);

    // Validate decoding
    Keypath decode_keypath;
    CPPUNIT_ASSERT_NO_THROW(decode_keypath.fromUr(expectedLegacyUR));
    Compare(decode_keypath, encode_keypath);
}

/**
 * @brief Test the encoding and decoding of this CDDL example
 * {
 *   1: [[64, false, 64, true]]
 * }
 * 
 */
void TestKeypath::EncodeAndDecodeOnlyDerivationPath(void)
{
    Keypath encode_keypath;
    std::string derivationPath = "m/<64;64'>";
    CPPUNIT_ASSERT_NO_THROW(encode_keypath.setDerivationPath(derivationPath));

    auto expectedBytes = fromHex("A10181841840F41840F5");
    auto expectedLegacyUR = std::string("ur:crypto-keypath/oyadlylrcsfzwkcsfzykrtgeqzjt");
    auto expectedNewUR = std::string("ur:keypath/oyadlylrcsfzwkcsfzykrtgeqzjt");

    ValidateDerivationPath(encode_keypath, derivationPath);
    ValidateCborResults(encode_keypath, expectedBytes);
    encode_keypath.setUseLegacyType(true);
    ValidateUrEncoding(encode_keypath, expectedLegacyUR);
    encode_keypath.setUseLegacyType(false);
    ValidateUrEncoding(encode_keypath, expectedNewUR);

    // Validate decoding
    Keypath decode_keypath;
    CPPUNIT_ASSERT_NO_THROW(decode_keypath.fromUr(expectedLegacyUR));
    Compare(decode_keypath, encode_keypath);
}

/**
 * @brief Test the encoding and decoding of keypath components for this CDDL example
 * {
 *   1: [98, true, [2, 6], false, [], true, [78200, true, 0, true]]
 * }
 * 
 */
void TestKeypath::EncodeAndDecodeKeypathComponents(void)
{
    Keypath encode_keypath;

    KeyPathComponent index(98, true);
    encode_keypath.addKeyPathComponent(index);

    KeyPathComponent range(2, 6, false);
    encode_keypath.addKeyPathComponent(range);

    KeyPathComponent wildcard(true);
    encode_keypath.addKeyPathComponent(wildcard);

    KeyPathComponent childPair(78200, true, 0, true);
    encode_keypath.addKeyPathComponent(childPair);

    std::string expectedDerivationPath = "m/98'/[2,6]/[]'/<78200';0'>";
    auto expectedBytes = fromHex("A101871862F5820206F480F5841A00013178F500F5");
    auto expectedLegacyUR = std::string("ur:crypto-keypath/oyadltcsidyklfaoamwklayklrcyaeadehksykaeyknlbtuomn");
    auto expectedNewUR = std::string("ur:keypath/oyadltcsidyklfaoamwklayklrcyaeadehksykaeyknlbtuomn");

    ValidateDerivationPath(encode_keypath, expectedDerivationPath);
    ValidateCborResults(encode_keypath, expectedBytes);
    encode_keypath.setUseLegacyType(true);
    ValidateUrEncoding(encode_keypath, expectedLegacyUR);
    encode_keypath.setUseLegacyType(false);
    ValidateUrEncoding(encode_keypath, expectedNewUR);

    // Validate decoding
    Keypath decode_keypath;
    CPPUNIT_ASSERT_NO_THROW(decode_keypath.fromUr(expectedLegacyUR));
    Compare(decode_keypath, encode_keypath);
}

/**
 * @brief Test returning an error when the derivation path is incorrect
 * 
 */
void TestKeypath::EncodeIncorrectDerivationPath(void)
{
    Keypath keypath;
    ValidateDerivationPathException(keypath, "/76/0'/4", CborErrorIO);
    ValidateDerivationPathException(keypath, "m/6/7432'/1nc0rr3ct", CborErrorIO);
}

/**
 * @brief Test returning an error when the fingerprint is defined as 0
 * 
 */
void TestKeypath::EncodeIncorrectFingerprint(void)
{
    Keypath keypath;
    std::string derivationPath = "m/45'/0";
    CPPUNIT_ASSERT_NO_THROW(keypath.setDerivationPath(derivationPath));

    keypath.setSourceFingerprint(0);

    ValidateUrEncodingException(keypath, CborErrorExcludedValue);
}

/**
 * @brief Test returning an error when the child index is superior or equal than 0x80000000 (= 2147483648)
 * 
 */
void TestKeypath::EncodeIncorrectChildIndex(void)
{
    Keypath keypath;
    std::string derivationPath = "m/50000'/249000/2147483648";
    CPPUNIT_ASSERT_NO_THROW(keypath.setDerivationPath(derivationPath));

    ValidateUrEncodingException(keypath, CborErrorImproperValue);
}

/**
 * @brief Test returning an error when the low index is superior than the high index
 * 
 */
void TestKeypath::EncodeIncorrectChildRangeIndex(void)
{
    Keypath keypath;
    std::string derivationPath = "m/78400/89'/9'/[76,34]";
    CPPUNIT_ASSERT_NO_THROW(keypath.setDerivationPath(derivationPath));

    ValidateUrEncodingException(keypath, CborErrorImproperValue);
}

/**
 * @brief Test decoding map index in a random order
 * {
 *   2: 654378,
 *   3: 4,
 *   1: [1, true, 2, false, 600, true, 67, true]
 * }
 * 
 */
void TestKeypath::DecodeRandomOrderMapIndex(void)
{
    Keypath keypath;
    std::string derivationPath = "m/1'/2/600'/67'";
    CPPUNIT_ASSERT_NO_THROW(keypath.setDerivationPath(derivationPath));

    keypath.setSourceFingerprint(654378);
    keypath.setDepth();

    auto urToDecode = std::string("ur:keypath/otaocyaeasztdraxaaadloadykaowkcfaohdykcsfxyklnspbnkn");

    Keypath decode_keypath;
    CPPUNIT_ASSERT_NO_THROW(decode_keypath.fromUr(urToDecode));
    Compare(decode_keypath, keypath);
}

/**
 * @brief Test returning an error due to wrong UR type
 * 
 */
void TestKeypath::DecodeIncorrectUrType(void)
{
    Keypath keypath;
    auto urError = std::string("ur:unknown/otaocyaeasztdraxaaadloadykaowkcfaohdykcsfxyklnspbnkn");

    ValidateUrDecodingException(keypath, urError, CborErrorUnknownType);
}

/**
 * @brief Test returning an error when the map index is unknown
 * {
 *   1: [78, true, [2, 6], false, [], true], 
 *   8: "unknown index" 
 * }
 * 
 */
void TestKeypath::DecodeUnknownMapIndex(void)
{
    Keypath keypath;
    auto urError = std::string("ur:keypath/oeadlncsglyklfaoamwklaykayjnkpjtjejtjlktjtcxinjtieihksgyrdoyws");
    
    ValidateUrDecodingException(keypath, urError, CborErrorUnknownType);
}

/**
 * @brief Test returning an error when the fingerprint is equal to 0
 * {
 *   1: [8009, true, 4, true, 0, false], 
 *   2: 0 
 * }
 * 
 */
void TestKeypath::DecodeExcludedValue0ForFingerprint(void)
{
    Keypath keypath;
    auto urError = std::string("ur:keypath/oeadlncfctgaykaaykaewkaoaegrdavych");

    ValidateUrDecodingException(keypath, urError, CborErrorExcludedValue);
}

/**
 * @brief Test returning an error when the child index is superior or equal than 0x80000000 (= 2147483648)
 * {
 *   1: [8009, false, 2147483699, false]
 * }
 * 
 */
void TestKeypath::DecodeImproperValueForChildIndex(void)
{
    Keypath keypath;
    auto urError = std::string("ur:keypath/oyadlrcfctgawkcylaaeaeeowktohttafx");

    ValidateUrDecodingException(keypath, urError, CborErrorImproperValue);
}

/**
 * @brief Test returning an error when the child index is superior or equal than 0x80000000 (= 2147483648)
 * {
 *   2: 754329056,
 *   3: 0
 * }
 * 
 */
void TestKeypath::DecodeImproperValueWithoutChildComponents(void)
{
    Keypath keypath;
    auto urError = std::string("ur:keypath/oeaocydwyndavtaxaebswlswva");

    ValidateUrDecodingException(keypath, urError, CborErrorTooFewItems);
}

/**
 * @brief Fuzzing of the decoder with several malformed keypath UR types 
 * 
 */
void TestKeypath::DecodeFuzzer(void)
{
    Keypath keypath;

    /* {1: [1]} */
    auto urError = std::string("ur:keypath/oyadlyadlatogmts");
    ValidateUrDecodingException(keypath, urError, CborErrorIllegalType, "1. Fuzzer failed");

    /* {1: [[]]} */
    urError = std::string("ur:keypath/oyadlylacyjsvyhs");
    ValidateUrDecodingException(keypath, urError, CborErrorIllegalType, "2. Fuzzer failed");

    /* {1: [[0]]} */
    urError = std::string("ur:keypath/oyadlylyaewnrpzevl");
    ValidateUrDecodingException(keypath, urError, CborErrorIllegalType, "3. Fuzzer failed");

    /* {1: [[643, true, 9]]} */
    urError = std::string("ur:keypath/oyadlylscfaolsykashpoxdygm");
    ValidateUrDecodingException(keypath, urError, CborErrorIllegalType, "4. Fuzzer failed");

    /* {1: [[4, 900]]} */
    urError = std::string("ur:keypath/oyadlylscfaolsykashpoxdygm");
    ValidateUrDecodingException(keypath, urError, CborErrorIllegalType, "5. Fuzzer failed");

    /* {1: [[4, 900]]} */
    urError = std::string("ur:keypath/oyadlylfaacfaxlrvabzbnbg");
    ValidateUrDecodingException(keypath, urError, CborErrorIllegalType, "6. Fuzzer failed");

    /* {1: [[87, 900, true, false]]} */
    urError = std::string("ur:keypath/oyadlylrcshgcfaxlrykwkcaehaedw");
    ValidateUrDecodingException(keypath, urError, CborErrorIllegalType, "7. Fuzzer failed");

    /* {1: [[7, 90000, true, false, 8765]]} */
    urError = std::string("ur:keypath/oyadlylpatcyaeadhemhykwkcfcpfsspvaghbn");
    ValidateUrDecodingException(keypath, urError, CborErrorIllegalType, "8. Fuzzer failed");

    /* {1: [[1, 2], false, true, 6, 56, false]} */
    urError = std::string("ur:keypath/oyadlnlfadaowkykamcsetwkecfxaode");
    ValidateUrDecodingException(keypath, urError, CborErrorIllegalType, "9. Fuzzer failed");

    /* {1: [[1, "abc"], true]} */
    urError = std::string("ur:keypath/oyadlflfadiahsidiaykvoztbdfp");
    ValidateUrDecodingException(keypath, urError, CborErrorIllegalType, "10. Fuzzer failed");

    /* {1: [[0, "incorrect", 7, false]]} */
    urError = std::string("ur:keypath/oyadlylraeininjtiajljpjpihiajyatwkstwnjsbe");
    ValidateUrDecodingException(keypath, urError, CborErrorIllegalType, "11. Fuzzer failed");

    /* {1: [[], "keypath"]} */
    urError = std::string("ur:keypath/oyadlflaiojeihkkjohsjyisaytasegm");
    ValidateUrDecodingException(keypath, urError, CborErrorIllegalType, "12. Fuzzer failed");

    /* Fingerprint out of range: {1: [[], true], 2: 4294967298} */
    urError = std::string("ur:keypath/oeadlflaykaosagscxgwtohyfmdaaohsbeaeaeaebgenmhdr");
    ValidateUrDecodingException(keypath, urError, CborErrorIllegalType, "13. Fuzzer failed");

    /* Depth out of range: {2: 43256889, 1: [[7, 9], true], 3: 4900} */
    urError = std::string("ur:keypath/otaocyaomwbnesadlflfatasykaxcfbwdkfppmidmt");
    ValidateUrDecodingException(keypath, urError, CborErrorImproperValue, "14. Fuzzer failed");

    /* {2: true, 1: [5, true], 3: 9} */
    urError = std::string("ur:keypath/otaoykadlfahykaxasnlcfcpry");
    ValidateUrDecodingException(keypath, urError, CborErrorIllegalType, "15. Fuzzer failed");

    /* {2: 23, 1: [987, false], 3: false} */
    urError = std::string("ur:keypath/otaochadlfcfaxuywkaxwkpmjkrtqz");
    ValidateUrDecodingException(keypath, urError, CborErrorIllegalType, "16. Fuzzer failed");

    /* {1: 6} */
    urError = std::string("ur:keypath/oyadamtslskneh");
    ValidateUrDecodingException(keypath, urError, CborErrorIllegalType, "17. Fuzzer failed");

    /* {} */
    urError = std::string("ur:keypath/nbaatygsih");
    ValidateUrDecodingException(keypath, urError, CborErrorImproperValue, "18. Fuzzer failed");

    /* Duplicated components: {1: [2, true], 1: [456, false]} */
    urError = std::string("ur:keypath/oeadlfaoykadlfcfadspwklrtosgos");
    ValidateUrDecodingException(keypath, urError, CborErrorMapKeysNotUnique, "19. Fuzzer failed");

    /* Duplicated fingerprint: {1: [0, false], 2: 600, 2: 1} */
    urError = std::string("ur:keypath/otadlfaewkaoadaocfaohdpkmkgubw");
    ValidateUrDecodingException(keypath, urError, CborErrorMapKeysNotUnique, "20. Fuzzer failed");

    /* Duplicated depth: {1: [0, false], 3: 1, 3: 4} */
    urError = std::string("ur:keypath/otadlfaewkaxadaxaaimlaykhh");
    ValidateUrDecodingException(keypath, urError, CborErrorMapKeysNotUnique, "21. Fuzzer failed");

    /* Empty content */
    urError = std::string("ur:keypath/");
    ValidateUrDecodingException(keypath, urError, CborErrorIO, "22. Fuzzer failed");

    /* random string */
    urError = std::string("ur:keypath/randomstring");
    ValidateUrDecodingException(keypath, urError, CborErrorIO, "23. Fuzzer failed");

    /* random invalid min bytewords */
    urError = std::string("ur:keypath/rtotbw");
    ValidateUrDecodingException(keypath, urError, CborErrorIO, "24. Fuzzer failed");

    /* random valid min bytewords of 0xFF324A568C65540009876B432E */
    urError = std::string("ur:keypath/zmeygehflkihghaeasltjefxdmrkbdnsrt");
    ValidateUrDecodingException(keypath, urError, CborErrorUnexpectedBreak, "25. Fuzzer failed");
}