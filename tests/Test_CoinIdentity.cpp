#include "Test_CoinIdentity.h"
#include "Test_UrHelpers.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestCoinIdentity);

void TestCoinIdentity::setUp() {
}

void TestCoinIdentity::tearDown() {
}

void TestCoinIdentity::Compare(const CoinIdentity& decoded, const CoinIdentity& expected) {
    // Compare curve
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Curve mismatch",
        static_cast<uint8_t>(expected.getCurve()),
        static_cast<uint8_t>(decoded.getCurve())
    );

    // Compare type
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Type mismatch",
        expected.getType(),
        decoded.getType()
    );

    // Compare subtypes
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Subtype presence mismatch",
        expected.getSubTypes().has_value(),
        decoded.getSubTypes().has_value()
    );

    if (decoded.getSubTypes().has_value()) {
        const auto& decoded_subtypes = decoded.getSubTypes().value();
        const auto& expected_subtypes = expected.getSubTypes().value();

        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Subtype count mismatch",
            expected_subtypes.size(),
            decoded_subtypes.size()
        );

        for (size_t i = 0; i < decoded_subtypes.size(); i++) {
            CPPUNIT_ASSERT_EQUAL_MESSAGE(
                "Subtype type mismatch at index " + std::to_string(i),
                static_cast<int>(expected_subtypes[i].getType()),
                static_cast<int>(decoded_subtypes[i].getType())
            );

            switch (decoded_subtypes[i].getType()) {
                case SubTypeExp::Type::UINT32:
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(
                        "Uint32 subtype value mismatch at index " + std::to_string(i),
                        expected_subtypes[i].getUint32Value(),
                        decoded_subtypes[i].getUint32Value()
                    );
                    break;
                case SubTypeExp::Type::STRING:
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(
                        "String subtype value mismatch at index " + std::to_string(i),
                        expected_subtypes[i].getStringValue(),
                        decoded_subtypes[i].getStringValue()
                    );
                    break;
                case SubTypeExp::Type::HEX_STRING:
                    CPPUNIT_ASSERT_MESSAGE(
                        "Hex string subtype value mismatch at index " + std::to_string(i),
                        expected_subtypes[i].getHexValue() == decoded_subtypes[i].getHexValue()
                    );
                    break;
            }
        }
    }
}

void TestCoinIdentity::TestAllFields() {
    // Create a CoinIdentity with all fields
    CoinIdentity identity(EllipticCurve::Secp256k1, 60);  // 60 is ETH in SLIP44

    // Add subtypes of different types
    std::vector<SubTypeExp> subtypes;
    subtypes.emplace_back(static_cast<uint32_t>(1));  // uint32
    subtypes.emplace_back(std::string("test"));  // string
    std::vector<uint8_t> hexData{0x01, 0x02, 0x03};
    subtypes.emplace_back(hexData);  // hex string
    identity.setSubTypes(subtypes);

    // A3                                      # map(3)
    //    01                                   # unsigned(1) curve
    //    08                                   # unsigned(8) secp256k1
    //    02                                   # unsigned(2) type
    //    18 3C                                # unsigned(60) ETH
    //    03                                   # unsigned(3) subtype
    //    83                                   # array(3)
    //       01                                # unsigned(1)
    //       64                                # text(4)
    //          74657374                       # "test"
    //       D9 0107                           # tag(263)
    //          43                             # bytes(3)
    //             010203                      # "\x01\x02\x03"

    QByteArray expectedBytes = QByteArray::fromHex("A3010802183C0383016474657374D9010743010203");
    QString expectedUR = "ur:coin-identity/otadayaocsfnaxlsadiejyihjkjytaadatfxadaoaxreurrewl";

    // Test encoding
    ValidateCborResults(identity, expectedBytes);
    ValidateUrEncoding(identity, expectedUR);

    // Test encoding from UAI
    CoinIdentity coinid_uai;
    CPPUNIT_ASSERT_NO_THROW(coinid_uai.setCoinIdentity("uai://secp256k1.60.1.test.0x010203"));
    ValidateCborResults(coinid_uai, expectedBytes);
    ValidateUrEncoding(coinid_uai, expectedUR);

    
    // Test toUaiStr()
    CPPUNIT_ASSERT(coinid_uai.toUaiStr() == "uai://secp256k1.60.1.test.0x010203");
    
    // Test decoding
    CoinIdentity decoded;
    CPPUNIT_ASSERT_NO_THROW(decoded.fromUr(expectedUR.toStdString()));
    Compare(decoded, identity);

    // Verify map size
    CPPUNIT_ASSERT_EQUAL(size_t(3), identity.getMapSize());  // curve, type, and subtypes

    // Verify subtypes
    CPPUNIT_ASSERT(identity.getSubTypes().has_value());
    const auto& storedSubtypes = identity.getSubTypes().value();
    CPPUNIT_ASSERT_EQUAL(size_t(3), storedSubtypes.size());

    // Check first subtype (uint32)
    CPPUNIT_ASSERT_EQUAL(SubTypeExp::Type::UINT32, storedSubtypes[0].getType());
    CPPUNIT_ASSERT_EQUAL(static_cast<uint32_t>(1), storedSubtypes[0].getUint32Value());

    // Check second subtype (string)
    CPPUNIT_ASSERT_EQUAL(SubTypeExp::Type::STRING, storedSubtypes[1].getType());
    CPPUNIT_ASSERT_EQUAL(std::string("test"), storedSubtypes[1].getStringValue());

    // Check third subtype (hex string)
    CPPUNIT_ASSERT_EQUAL(SubTypeExp::Type::HEX_STRING, storedSubtypes[2].getType());
    CPPUNIT_ASSERT(hexData == storedSubtypes[2].getHexValue());
}

void TestCoinIdentity::TestMinimumFields() {
    // Create a CoinIdentity with only mandatory fields
    CoinIdentity identity(EllipticCurve::Ed25519, 148);  // 148 is XLM in SLIP44

    // A2                                      # map(2)
    //    01                                   # unsigned(1) curve
    //    06                                   # unsigned(6) Ed25519
    //    02                                   # unsigned(2) type
    //    18 94                                # unsigned(148) XLM
    QByteArray expectedBytes = QByteArray::fromHex("A20106021894");
    QString expectedUR = "ur:coin-identity/oeadamaocsmwluvtkefz";

    // Test encoding
    ValidateCborResults(identity, expectedBytes);
    ValidateUrEncoding(identity, expectedUR);

    // Test encoding from UAI
    CoinIdentity coinid_uai;
    CPPUNIT_ASSERT_NO_THROW(coinid_uai.setCoinIdentity("uai://Ed25519.148"));
    ValidateCborResults(coinid_uai, expectedBytes);
    ValidateUrEncoding(coinid_uai, expectedUR);

    // Test decoding
    CoinIdentity decoded;
    CPPUNIT_ASSERT_NO_THROW(decoded.fromUr(expectedUR.toStdString()));
    Compare(decoded, identity);
}

void TestCoinIdentity::TestInvalidType() {
    // Create a CoinIdentity
    CoinIdentity identity(EllipticCurve::P256, 0);

    // Test setting type with high bit set (should throw)
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw on type with high bit set",
        identity.setType(0x80000000),
        CborException
    );

    // Test decoding type with high bit set
    // A2                                      # map(2)
    //    01                                   # unsigned(1) curve
    //    01                                   # unsigned(1) P256
    //    02                                   # unsigned(2) type
    //    1A 80000000                          # unsigned(0x80000000)
    QString urError = "ur:coin-identity/oeadadaocylaaeaeaesffldpzc"; // Contains type = 0x80000000
    ValidateUrDecodingException(identity, urError, CborErrorImproperValue, "Type with high bit set failed");
}

void TestCoinIdentity::TestInvalidCurve() {
    CoinIdentity identity;

    // A2                                      # map(2)
    //    01                                   # unsigned(1) curve
    //    09                                   # unsigned(9) invalid curve value
    //    02                                   # unsigned(2) type
    //    00                                   # unsigned(0)
    QString urError = "ur:coin-identity/oeadasaoaegannahls";
    ValidateUrDecodingException(identity, urError, CborErrorImproperValue, "Invalid curve value failed");
}

void TestCoinIdentity::TestMissingMandatoryFields() {
    CoinIdentity identity;

    // A2                                      # map(2)
    //    02                                   # unsigned(2) type
    //    18 3C                                # unsigned(60) ETH
    //    03                                   # unsigned(3) subtype
    //    81                                   # array(1)
    //       64                                # text(4)
    //          74657374                       # "test"
    QString urError = "ur:coin-identity/oeaocsfnaxlyiejyihjkjyjpmwinkb";
    ValidateUrDecodingException(identity, urError, CborErrorTooFewItems, "Map missing the 'curve' mandatory field");

    // A2                                      # map(2)
    //    01                                   # unsigned(1) curve
    //    01                                   # unsigned(1) P256
    //    03                                   # unsigned(3) subtype
    //    81                                   # array(1)
    //       64                                # text(4)
    //          74657374                       # "test"
    urError = "ur:coin-identity/oeadadaxlyiejyihjkjyvekgbdft";
    ValidateUrDecodingException(identity, urError, CborErrorTooFewItems, "Map missing the 'type' mandatory field");
}

void TestCoinIdentity::TestNotUniqueMapKeys() {
    CoinIdentity identity;

    // A3                                      # map(3)
    //    01                                   # unsigned(1) curve
    //    01                                   # unsigned(1) P256
    //    02                                   # unsigned(2) type
    //    00                                   # unsigned(0)
    //    01                                   # unsigned(1) duplicate curve
    //    01                                   # unsigned(1) P256
    QString urError = "ur:coin-identity/otadadaoaeadadtaamdass";
    ValidateUrDecodingException(identity, urError, CborErrorMapKeysNotUnique, "Duplicate curve key failed");

    // A3                                      # map(3)
    //    01                                   # unsigned(1) curve
    //    01                                   # unsigned(1) P256
    //    02                                   # unsigned(2) type
    //    00                                   # unsigned(0)
    //    02                                   # unsigned(2) duplicate type
    //    00                                   # unsigned(0)
    urError = "ur:coin-identity/otadadaoaeaoaelpdwfgme";
    ValidateUrDecodingException(identity, urError, CborErrorMapKeysNotUnique, "Duplicate type key failed");

    // A3                                      # map(3)
    //    01                                   # unsigned(1) curve
    //    01                                   # unsigned(1) P256
    //    03                                   # unsigned(3) subtype
    //    81                                   # array(1)
    //       01                                # unsigned(1)
    //    03                                   # unsigned(3) duplicate subtype
    //    81                                   # array(1)
    //       01                                # unsigned(1)
    urError = "ur:coin-identity/otadadaxlyadaxlyadqzlebkly";
    ValidateUrDecodingException(identity, urError, CborErrorMapKeysNotUnique, "Duplicate subtype key failed");
}

void TestCoinIdentity::TestInvalidSubtypes() {
    CoinIdentity identity;

    // A3                                      # map(3)
    //    01                                   # unsigned(1) curve
    //    01                                   # unsigned(1) P256
    //    02                                   # unsigned(2) type
    //    00                                   # unsigned(0)
    //    03                                   # unsigned(3) subtype
    //    80                                   # array(0) empty array
    QString urError = "ur:coin-identity/otadadaoaeaxlajsmywkwt";
    ValidateUrDecodingException(identity, urError, CborErrorImproperValue, "Empty subtype array failed");

    // A3                                      # map(3)
    //    01                                   # unsigned(1) curve
    //    01                                   # unsigned(1) P256
    //    02                                   # unsigned(2) type
    //    00                                   # unsigned(0)
    //    03                                   # unsigned(3) subtype
    //    81                                   # array(1)
    //       F4                                # primitive(20) boolean false - invalid type
    urError = "ur:coin-identity/otadadaoaeaxlywksfahmdcl";
    ValidateUrDecodingException(identity, urError, CborErrorIllegalType, "Invalid subtype type failed");

    // A3                                      # map(3)
    //    01                                   # unsigned(1) curve
    //    01                                   # unsigned(1) P256
    //    02                                   # unsigned(2) type
    //    00                                   # unsigned(0)
    //    03                                   # unsigned(3) subtype
    //    81                                   # array(1)
    //       D9 0108                           # tag(264) wrong tag for hex string
    //          43                             # bytes(3)
    //             010203                      # "\x01\x02\x03"
    urError = "ur:coin-identity/otadadaoaeaxlytaadayfxadaoaxryondmoe";
    ValidateUrDecodingException(identity, urError, CborErrorInappropriateTagForType, "Invalid hex string tag failed");
}

void TestCoinIdentity::TestEncodingUaiExamples() {
    // UAI examples from NBCR-2024-001
    // Ref: https://github.com/ngraveio/Research/blob/main/papers/nbcr-2024-001-unique-asset-id.md
    CoinIdentity coinid_uai;

    // Bitcoin (BTC)
    // A2    # map(2)
    //   01 # unsigned(1)
    //   08 # unsigned(8)
    //   02 # unsigned(2)
    //   00 # unsigned(0)
    CPPUNIT_ASSERT_NO_THROW(coinid_uai.setCoinIdentity("uai://secp256k1.0"));
    QByteArray expectedBytes = QByteArray::fromHex("A201080200");
    QString expectedUR = "ur:coin-identity/oeadayaoaefdhhjlqz";
    ValidateCborResults(coinid_uai, expectedBytes);
    ValidateUrEncoding(coinid_uai, expectedUR);

    // Bitcoin (BTC) Taproot accounts
    // Same result
    CPPUNIT_ASSERT_NO_THROW(coinid_uai.setCoinIdentity("uai://secp256k1.0@tr/86h/0h/0h"));
    ValidateCborResults(coinid_uai, expectedBytes);
    ValidateUrEncoding(coinid_uai, expectedUR);

    // Ethereum (ETH)
    // A3       # map(3)
    //   01    # unsigned(1)
    //   08    # unsigned(8)
    //   02    # unsigned(2)
    //   18 3C # unsigned(60)
    //   03    # unsigned(3)
    //   81    # array(1)
    //      01 # unsigned(1)
    CPPUNIT_ASSERT_NO_THROW(coinid_uai.setCoinIdentity("uai://secp256k1.60.1"));
    expectedBytes = QByteArray::fromHex("A3010802183C038101");
    expectedUR = "ur:coin-identity/otadayaocsfnaxlyadeecfytwd";
    ValidateCborResults(coinid_uai, expectedBytes);
    ValidateUrEncoding(coinid_uai, expectedUR);

    // NFT on Ethereum
    // Same result
    CPPUNIT_ASSERT_NO_THROW(coinid_uai.setCoinIdentity("uai://secp256k1.60.1:0x495f947276749Ce646f68AC8c248420045cb7b5e.30215980622330187411918288900688501299580125367569939549692495859506871271425"));
    ValidateCborResults(coinid_uai, expectedBytes);
    ValidateUrEncoding(coinid_uai, expectedUR);

    // USDC on Ethereum account 2
    // Same result
    CPPUNIT_ASSERT_NO_THROW(coinid_uai.setCoinIdentity("uai://secp256k1.60.1:0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48/44h/60h/0h/0/0/1?master_fingerprint=934670036"));
    ValidateCborResults(coinid_uai, expectedBytes);
    ValidateUrEncoding(coinid_uai, expectedUR);

    // Ethereum accounts 1 to 3
    // Same result
    CPPUNIT_ASSERT_NO_THROW(coinid_uai.setCoinIdentity("uai://secp256k1.60.1/44h/60h/0h/0/0/[0,2]?master_fingerprint=934670036"));
    ValidateCborResults(coinid_uai, expectedBytes);
    ValidateUrEncoding(coinid_uai, expectedUR);

    // Polygon (POL)
    // A3       # map(3)
    //   01    # unsigned(1)
    //   08    # unsigned(8)
    //   02    # unsigned(2)
    //   18 3C # unsigned(60)
    //   03    # unsigned(3)
    //   81    # array(1)
    //      18 89 # unsigned(137)
    CPPUNIT_ASSERT_NO_THROW(coinid_uai.setCoinIdentity("uai://secp256k1.60.137"));
    expectedBytes = QByteArray::fromHex("A3010802183C03811889");
    expectedUR = "ur:coin-identity/otadayaocsfnaxlycsldntlgjlmn";
    ValidateCborResults(coinid_uai, expectedBytes);
    ValidateUrEncoding(coinid_uai, expectedUR);

    // USDC on Polygon
    // Same result
    CPPUNIT_ASSERT_NO_THROW(coinid_uai.setCoinIdentity("uai://secp256k1.60.137:0x3c499c542cEF5E3811e1192ce70d8cC03d5c3359"));
    ValidateCborResults(coinid_uai, expectedBytes);
    ValidateUrEncoding(coinid_uai, expectedUR);

    // Solana (SOL)
    // A2         # map(2)
    //    01      # unsigned(1)
    //    06      # unsigned(6)
    //    02      # unsigned(2)
    //    19 01F5 # unsigned(501)
    CPPUNIT_ASSERT_NO_THROW(coinid_uai.setCoinIdentity("uai://ed25519.501"));
    expectedBytes = QByteArray::fromHex("A20106021901F5");
    expectedUR = "ur:coin-identity/oeadamaocfadykiekohhny";
    ValidateCborResults(coinid_uai, expectedBytes);
    ValidateUrEncoding(coinid_uai, expectedUR);

    // USDC on Solana
    // Same result
    CPPUNIT_ASSERT_NO_THROW(coinid_uai.setCoinIdentity("uai://ed25519.501:EPjFWdd5AufqSSqeM2qN1xzybapC8G4wEGGkZwyTDt1v"));
    ValidateCborResults(coinid_uai, expectedBytes);
    ValidateUrEncoding(coinid_uai, expectedUR);

    // Tezos (XTZ) based on ed25519
    // A2    # map(2)
    //   01 # unsigned(1)
    //   06 # unsigned(6)
    //   02 # unsigned(2)
    //   19 06C1 # unsigned(1729)
    CPPUNIT_ASSERT_NO_THROW(coinid_uai.setCoinIdentity("uai://ed25519.1729"));
    expectedBytes = QByteArray::fromHex("A20106021906C1");
    expectedUR = "ur:coin-identity/oeadamaocfamsebklsfmvs";
    ValidateCborResults(coinid_uai, expectedBytes);
    ValidateUrEncoding(coinid_uai, expectedUR);

    // Tezos (XTZ) based on secp256k1
    // A2    # map(2)
    //   01 # unsigned(1)
    //   08 # unsigned(6)
    //   02 # unsigned(2)
    //   19 06C1 # unsigned(1729)
    CPPUNIT_ASSERT_NO_THROW(coinid_uai.setCoinIdentity("uai://secp256k1.1729"));
    expectedBytes = QByteArray::fromHex("A20108021906C1");
    expectedUR = "ur:coin-identity/oeadayaocfamsereqdlald";
    ValidateCborResults(coinid_uai, expectedBytes);
    ValidateUrEncoding(coinid_uai, expectedUR);

    // Neo
    // A2         # map(2)
    //   01      # unsigned(1)
    //   01      # unsigned(1)
    //   02      # unsigned(2)
    //   19 0378 # unsigned(888)
    CPPUNIT_ASSERT_NO_THROW(coinid_uai.setCoinIdentity("uai://p256.888"));
    expectedBytes = QByteArray::fromHex("A2010102190378");
    expectedUR = "ur:coin-identity/oeadadaocfaxksktincamd";
    ValidateCborResults(coinid_uai, expectedBytes);
    ValidateUrEncoding(coinid_uai, expectedUR);

    // Polkadot
    // A2         # map(2)
    //    01      # unsigned(1)
    //    04      # unsigned(4)
    //    02      # unsigned(2)
    //    19 0162 # unsigned(354)
    CPPUNIT_ASSERT_NO_THROW(coinid_uai.setCoinIdentity("uai://x25519.354"));
    expectedBytes = QByteArray::fromHex("A2010402190162");
    expectedUR = "ur:coin-identity/oeadaaaocfadidjoutasca";
    ValidateCborResults(coinid_uai, expectedBytes);
    ValidateUrEncoding(coinid_uai, expectedUR);
    
    // USDC on MultiversX
    // A2         # map(2)
    //    01      # unsigned(1)
    //    06      # unsigned(6)
    //    02      # unsigned(2)
    //    19 01FC # unsigned(508)
    CPPUNIT_ASSERT_NO_THROW(coinid_uai.setCoinIdentity("uai://ed25519.508:USDC-c76f1f"));
    expectedBytes = QByteArray::fromHex("A20106021901FC");
    expectedUR = "ur:coin-identity/oeadamaocfadztcapkvefm";
    ValidateCborResults(coinid_uai, expectedBytes);
    ValidateUrEncoding(coinid_uai, expectedUR);
}

void TestCoinIdentity::TestInvalidUaiStrings() {
    CoinIdentity invalid_uai;
    // Missing uai:// prefix
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw invalid UAI string",
        invalid_uai.setCoinIdentity("ed25519.508:USDC-c76f1f"),
        CborException
    );

    // Missing curve
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw invalid UAI string",
        invalid_uai.setCoinIdentity("uai://508"),
        CborException
    );

    // Invalid curve name
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw invalid UAI string",
        invalid_uai.setCoinIdentity("uai://unknown.508"),
        CborException
    );

    // Missing type
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw invalid UAI string",
        invalid_uai.setCoinIdentity("uai://secp256k1"),
        CborException
    );

    // Invalid type
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw invalid UAI string",
        invalid_uai.setCoinIdentity("uai://secp256k1.invalid"),
        CborException
    );

    // Invalid separator
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw invalid UAI string",
        invalid_uai.setCoinIdentity("uai://secp256k1;56"),
        CborException
    );

    // Invalid derivation path
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw invalid UAI string",
        invalid_uai.setCoinIdentity("uai://secp256k1.0/56h/abcd"),
        CborException
    );

    // Invalid master fingerprint text
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw invalid UAI string",
        invalid_uai.setCoinIdentity("uai://secp256k1.0?master=54"),
        CborException
    );

    // Invalid master fingerprint value
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw invalid UAI string",
        invalid_uai.setCoinIdentity("uai://secp256k1.0?master_fingerprint=123f"),
        CborException
    );

    // Excessive backtracking test -> Can lead to infinite loop if 
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw invalid UAI string",
        invalid_uai.setCoinIdentity("uai://secp256k1.60.137:0x7ceb23fd6bc0add59e62ac25578270cff1b9f619/44'/60'/0'/0/0?=master_fingerprint=123456789"),
        CborException
    );
}