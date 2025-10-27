#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <string>

#include "Test_SignRequest.h"
#include "../src/sign_request.h"
#include "Test_UrHelpers.h"
#include "Test_CoinIdentity.h"
#include "Test_Keypath.h"

void TestSignRequest::setUp(void)
{
}

void TestSignRequest::tearDown(void)
{
}

void TestSignRequest::validSignRequestWithBytesStringAddress()
{
    // sign-request with a coin-identity(eliptic-curve: Secp256k1, type: 60) and sign-data (in hex): 010203
    SignRequest expectedSignRequest(CoinIdentity(EllipticCurve::Secp256k1, 60), fromHex("010203"));

    expectedSignRequest.setRequestID(Uuid("c8a23dbcb80148fda5acb6ffd7f39e92"));
    expectedSignRequest.setOrigin("liquidapp");
    expectedSignRequest.setTxtype(1);
    expectedSignRequest.setDerivationPath(Keypath("m/<64;64'>"));
    expectedSignRequest.setAddress(std::vector<uint8_t>{0x01, 0x02, 0x03, 0x04, 0x05});

    expectedSignRequest.setIntent(
        Intent(
            "$BATCH.", {
                           {"TX(0)", 5},
                           {"TX(1)", 5},
                           {"TX(2)", 5},
                           {"SIG(1)", 5},
                           {"SIG(2)", 5},
                           {"SIG(3)", 5},
                       }));

    /* CBOR */
    // A7                                      # map(7)
    //   01                                   # unsigned(1)
    //   D8 25                                # tag(37)
    //      50                                # bytes(16)
    //         C8A23DBCB80148FDA5ACB6FFD7F39E92 # "?=\xBC\xB8\u0001H\xFD\xA5\xAC\xB6\xFF\xD7\xF3\x9E\x92"
    //   02                                   # unsigned(2)
    //   D9 A1B9                              # tag(41401)
    //      A2                                # map(2)
    //         01                             # unsigned(1)
    //         08                             # unsigned(8)
    //         02                             # unsigned(2)
    //         18 3C                          # unsigned(60)
    //   03                                   # unsigned(3)
    //   D9 9D70                              # tag(40304)
    //      A1                                # map(1)
    //         01                             # unsigned(1)
    //         81                             # array(1)
    //            84                          # array(4)
    //               18 40                    # unsigned(64)
    //               F4                       # primitive(20)
    //               18 40                    # unsigned(64)
    //               F5                       # primitive(21)
    //   04                                   # unsigned(4)
    //   43                                   # bytes(3)
    //      010203                            # "\u0001\u0002\u0003"
    //   05                                   # unsigned(5)
    //   69                                   # text(9)
    //      6C6971756964617070                # "liquidapp"
    //   06                                   # unsigned(6)
    //   01                                   # unsigned(1)
    //   07                                   # unsigned(7)
    //   45                                   # bytes(5)
    //      0102030405                        # "\u0001\u0002\u0003\u0004\u0005"
    //   08                                   # unsigned(8)
    //      D9 A1C7                              # tag(41415)
    //         A1                                # map(1)
    //            01                             # unsigned(1)
    //            A2                             # map(2)
    //               01                          # unsigned(1)
    //               67                          # text(7)
    //                  2442415443482E           # "$BATCH."
    //               02                          # unsigned(2)
    //               86                          # array(6)
    //                  82                       # array(2)
    //                     65                    # text(5)
    //                        5458283029         # "TX(0)"
    //                     05                    # unsigned(5)
    //                  82                       # array(2)
    //                     65                    # text(5)
    //                        5458283129         # "TX(1)"
    //                     05                    # unsigned(5)
    //                  82                       # array(2)
    //                     65                    # text(5)
    //                        5458283229         # "TX(2)"
    //                     05                    # unsigned(5)
    //                  82                       # array(2)
    //                     66                    # text(6)
    //                        534947283129       # "SIG(1)"
    //                     05                    # unsigned(5)
    //                  82                       # array(2)
    //                     66                    # text(6)
    //                        534947283229       # "SIG(2)"
    //                     05                    # unsigned(5)
    //                  82                       # array(2)
    //                     66                    # text(6)
    //                        534947283329       # "SIG(3)"
    //                     05                    # unsigned(5)

    const std::string testSignRequestUr = "ur:sign-request/pdadtpdagdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhoeadayaocsfnaxtantjooyadlylrcsfzwkcsfzykaafxadaoaxahinjzinjskpiniehsjojoamadatfeadaoaxaaahaytaoystoyadoeadiodkfwfpghfxfddmaolnlfihghhddedydtahlfihghhddeehdtahlfihghhddeeydtahlfiygugafldeehdtahlfiygugafldeeydtahlfiygugafldeeodtahldvortso";
    SignRequest testSignRequest;
    testSignRequest.fromUr(testSignRequestUr);

    Compare(testSignRequest, expectedSignRequest);
}

void TestSignRequest::validSignRequestWithTextStringAddress()
{
    // sign-request with a coin-identity(eliptic-curve: Secp256k1, type: 60) and sign-data: f4f1f0
    SignRequest expectedSignRequest(CoinIdentity(EllipticCurve::Secp256k1, 60), fromHex("f4f1f0"));
    expectedSignRequest.setAddress("0102030405");

    /* CBOR */
    // A3                         # map(3)
    //    02                      # unsigned(2)
    //    D9 A1B9                 # tag(41401)
    //       A2                   # map(2)
    //          01                # unsigned(1)
    //          08                # unsigned(8)
    //          02                # unsigned(2)
    //          18 3C             # unsigned(60)
    //    04                      # unsigned(4)
    //    43                      # bytes(3)
    //       F4F1F0               # "\xF4\xF1\xF0"
    //    07                      # unsigned(7)
    //    6A                      # text(10)
    //       30313032303330343035 # "0102030405"

    const std::string testSignRequestUr = "ur:sign-request/otaotaoyrhoeadayaocsfnaafxwkwnwtatimdyehdyeydyeodyeedyecrnnlfpvl";
    SignRequest testSignRequest;
    testSignRequest.fromUr(testSignRequestUr);

    Compare(testSignRequest, expectedSignRequest);
}

void TestSignRequest::signRequestMissingMandatoryFields()
{
    /* missing coin-identity field */
    /* CBOR */
    // A2                        # map(2)
    //   04                      # unsigned(4)
    //   43                      # bytes(3)
    //      F4F1F0               # "\xF4\xF1\xF0"
    //   07                      # unsigned(7)
    //   6A                      # text(10)
    //      30313032303330343035 # "0102030405"

    std::string singRequestUr = "ur:sign-request/oeaafxwkwnwtatimdyehdyeydyeodyeedyechhhtcxty";
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Throws on missing coin-identity field",
        SignRequest().fromUr(singRequestUr),
        CborException);

    /* missing sign-data field */
    /* CBOR */
    // A2                         # map(2)
    //    02                      # unsigned(2)
    //    D9 A1B9                 # tag(41401)
    //       A2                   # map(2)
    //          01                # unsigned(1)
    //          08                # unsigned(8)
    //          02                # unsigned(2)
    //          18 3C             # unsigned(60)
    //    07                      # unsigned(7)
    //    6A                      # text(10)
    //       30313032303330343035 # "0102030405"

    singRequestUr = "ur:sign-request/oeaotaoyrhoeadayaocsfnatimdyehdyeydyeodyeedyeccarhtabk";
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Throws on missing sign-data field",
        SignRequest().fromUr(singRequestUr),
        CborException);
}

void TestSignRequest::signRequestDecoderFuzzingTests()
{
    SignRequest signRequest;

    /* Invalid UUID tag */
    auto invalidUr = std::string("ur:sign-request/osadtpdsgdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhoeadayaocsfnaxtantjooyadlylrcsfzwkcsfzykaafxadaoaxahinjzinjskpiniehsjojoamadatfeadaoaxaaahbtfrguia");
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorInappropriateTagForType, "1. Wrong UUID tag UR Fuzzer failed.");
    /* Wrong UUID type */
    invalidUr = "ur:sign-request/osadtpdakscxfxetfpeyeofyfwfxfwetdyeheeetfgfyfpecfpfxfwenfgfgfyemfgeoesfeeseyaotaoyrhoeadayaocsfnaxtantjooyadlylrcsfzwkcsfzykaafxadaoaxahinjzinjskpiniehsjojoamadatfeadaoaxaaahcegtcyks";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorIllegalType, "2. Wrong UUID type UR Fuzzer failed.");
    /* Wrong UUID length */
    invalidUr = "ur:sign-request/osadtpdagyspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhoeadayaocsfnaxtantjooyadlylrcsfzwkcsfzykaafxadaoaxahinjzinjskpiniehsjojoamadatfeadaoaxaaahayhstytl";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorImproperValue, "3. Wrong UUID size UR Fuzzer failed.");

    /* Wrong coin-identity tag */
    invalidUr = "ur:sign-request/osadtpdagdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrdoeadayaocsfnaxtantjooyadlylrcsfzwkcsfzykaafxadaoaxahinjzinjskpiniehsjojoamadatfeadaoaxaaahsetdtllb";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorInappropriateTagForType, "4. Wrong coin-identity tag UR Fuzzer failed.");
    /* Wrong coin-identity type */
    invalidUr = "ur:sign-request/osadtpdagdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhfycxdydygdaxtantjooyadlylrcsfzwkcsfzykaafxadaoaxahinjzinjskpiniehsjojoamadatfeadaoaxaaahnsfrfxht";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorIllegalType, "5. Wrong coin-identity type UR Fuzzer failed.");
    /* coin-identity with missing field */
    invalidUr = "ur:sign-request/osadtpdagdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhoyaocsfnaxtantjooyadlylrcsfzwkcsfzykaafxadaoaxahinjzinjskpiniehsjojoamadatfeadaoaxaaahmtolcpla";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorTooFewItems, "6. Wrong coin-identity tag UR Fuzzer failed.");
    /* Corrupted coin-identity */
    invalidUr = "ur:sign-request/osadtpdagdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhoeadaybsyannlfcsfnaxtantjooyadlylrcsfzwkcsfzykaafxadaoaxahinjzinjskpiniehsjojoamadatfeadaoaxaaahihgmahlt";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorUnknownType, "7. Corrupted coin-identity UR Fuzzer failed.");

    /* Wrong keypath tag */
    invalidUr = "ur:sign-request/osadtpdagdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhoeadayaocsfnaxtantjsoyadlylrcsfzwkcsfzykaafxadaoaxahinjzinjskpiniehsjojoamadatfeadaoaxaaahtyyapmhe";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorInappropriateTagForType, "8. Wrong keypath tag UR Fuzzer failed.");
    /* Wrong keypath type */
    invalidUr = "ur:sign-request/osadtpdagdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhoeadayaocsfnaxtantjofxfdbgctaafxadaoaxahinjzinjskpiniehsjojoamadatfeadaoaxaaahktdkvysb";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorIllegalType, "9. Wrong keypath type UR Fuzzer failed.");
    /* Empty keypath map */
    invalidUr = "ur:sign-request/osadtpdagdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhfycxdydygdaxtantjonbaafxadaoaxahinjzinjskpiniehsjojoamadatfeadaoaxaaahpesfbsfe";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorIllegalType, "10. Empty keypath map UR Fuzzer failed.");
    /* Corrupted keypath */
    invalidUr = "ur:sign-request/osadtpdagdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhfycxdydygdaxtantjonbaehkmkoxwkfxadaoaxahinjzinjskpiniehsjojoamadatfeadaoaxaaahwkbedtby";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorIllegalType, "11. Corrupted keypath map UR Fuzzer failed.");

    /* Wrong sign-data type */
    invalidUr = "ur:sign-request/osadtpdagdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhoeadayaocsfnaxtantjooyadlylrcsfzwkcsfzykaaiydyehdyeydyeoahinjzinjskpiniehsjojoamadatfeadaoaxaaahswdnmkbd";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorIllegalType, "12. Wrong sign-data type UR Fuzzer failed.");
    /* Wrong sign-data length */
    invalidUr = "ur:sign-request/osadtpdagdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhoeadayaocsfnaxtantjooyadlylrcsfzwkcsfzykaafyadaoaxahinjzinjskpiniehsjojoamadatimdyehdyeydyeodyeedyecdraejzeo";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorIllegalType, "13. Wrong sign-data size UR Fuzzer failed.");

    /* Wrong origin type */
    invalidUr = "ur:sign-request/osadtpdagdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhoeadayaocsfnaxtantjooyadlylrcsfzwkcsfzykaafxadaoaxahfyadaoaxaaamadatfeadaoaxaaahwygyhylf";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorIllegalType, "14. Wrong origin type UR Fuzzer failed.");

    /* Wrong txtype type */
    invalidUr = "ur:sign-request/osadtpdagdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhoeadayaocsfnaxtantjooyadlylrcsfzwkcsfzykaafxadaoaxahinjzinjskpiniehsjojoamhsehatfeadaoaxaaahidgskpti";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorIllegalType, "15. Wrong txtype type UR Fuzzer failed.");

    /* Wrong address type */
    invalidUr = "ur:sign-request/osadtpdagdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhoeadayaocsfnaxtantjooyadlylrcsfzwkcsfzykaafxadaoaxahinjzinjskpiniehsjojoamadatbsctrhckfx";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorIllegalType, "16. Wrong address type UR Fuzzer failed.");
    /* Wrong address bytes string length */
    invalidUr = "ur:sign-request/osadtpdagdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhoeadayaocsfnaxtantjooyadlylrcsfzwkcsfzykaafxadaoaxahinjzinjskpiniehsjojoamadatfgadaoaxaaahstcebdie";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorUnexpectedEOF, "17. Wrong address bytes string size UR Fuzzer failed.");
    /* Wrong address text string length */
    invalidUr = "ur:sign-request/osadtpdagdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhoeadayaocsfnaxtantjooyadlylrcsfzwkcsfzykaafxadaoaxahinjzinjskpiniehsjojoamadatjedyehdyeydyeodyeedyeccfvsrdgl";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorUnexpectedEOF, "18. Wrong address text string size UR Fuzzer failed.");

    /* Empty map: {} */
    invalidUr = "ur:sign-request/nbaatygsih";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorImproperValue, "19. Fuzzer failed");
    /* Empty content */
    invalidUr = "ur:sign-request/";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorIO, "20. Fuzzer failed");

    /* Random string */
    invalidUr = "ur:sign-request/osadtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaohdgryagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaaxadaaadahtaaddyoeadlecsdwykadykadykaewkadwkaocybgeehfksamgraeaeaeaeaeaeaeaeaeaeaeatisjnihjyhsjnhsjkjelkdtwpfhadded";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorIO, "21. Fuzzer failed");

    /* Invalid UR with random min bytewords */
    invalidUr = "ur:sign-request/frkigrpmndutdnbtkgfssblaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeae";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorIO, "22. Fuzzer failed");

    /* Valid UR with random min bytewords */
    invalidUr = "ur:sign-request/pezmtlioaeqzeyjssfaslpfghklgdyfn";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorImproperValue, "23. Fuzzer failed");

    /* Invalid CRC32 checksum in UR */
    invalidUr = "ur:sign-request/oladtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaohdgryagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaaxadaaadahtaaddyoeadlecsdwykadykadykaewkadwkaocybgeehfksatisjnihjyhsjnhsjkjetlmhdant";
    ValidateUrDecodingException(signRequest, invalidUr, CborErrorIO, "24. Fuzzer failed");
}

void TestSignRequest::Compare(const SignRequest &decodedSignRequest, const SignRequest &expectedSignRequest)
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "RequestID presence mismatch",
        decodedSignRequest.getRequestID().has_value(),
        expectedSignRequest.getRequestID().has_value());

    if (decodedSignRequest.getRequestID().has_value())
    {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "RequestID mismatch",
            decodedSignRequest.getRequestID().value().str(),
            expectedSignRequest.getRequestID().value().str());
    }

    TestCoinIdentity::Compare(decodedSignRequest.getCoinIdentity(), expectedSignRequest.getCoinIdentity());

    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Derivation path presence mismatch",
        decodedSignRequest.getDerivationPath().has_value(),
        expectedSignRequest.getDerivationPath().has_value());

    if (decodedSignRequest.getDerivationPath().has_value())
        TestKeypath::Compare(decodedSignRequest.getDerivationPath().value(), expectedSignRequest.getDerivationPath().value());

    const BytesVector decodedSignData = decodedSignRequest.getSignData();
    const BytesVector expectedSignData = expectedSignRequest.getSignData();

    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Sign data size mismatch",
        decodedSignData.size(),
        expectedSignData.size());

    for (size_t i = 0; i < decodedSignData.size(); i++)
    {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Sign data value mismatch at index " + std::to_string(i),
            decodedSignData[i],
            expectedSignData[i]);
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Origin presence mismatch",
        decodedSignRequest.getOrigin().has_value(),
        expectedSignRequest.getOrigin().has_value());

    if (decodedSignRequest.getOrigin().has_value())
    {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Origin mismatch",
            decodedSignRequest.getOrigin().value(),
            expectedSignRequest.getOrigin().value());
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Txtype presence mismatch",
        decodedSignRequest.getTxtype().has_value(),
        expectedSignRequest.getTxtype().has_value());

    if (decodedSignRequest.getTxtype().has_value())
    {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Txtype mismatch",
            decodedSignRequest.getTxtype().value(),
            expectedSignRequest.getTxtype().value());
    }

    const auto decodedAddress = decodedSignRequest.getAddress();
    const auto expectedAddress = decodedSignRequest.getAddress();

    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Address presence mismatch",
        std::holds_alternative<std::monostate>(decodedAddress),
        std::holds_alternative<std::monostate>(expectedAddress));

    // Address can be a text string or a bytes string
    if (std::holds_alternative<std::string>(decodedAddress))
    {
        CPPUNIT_ASSERT_MESSAGE(
            "Address string type mismatch",
            std::holds_alternative<std::string>(expectedAddress));
        auto decodedAddressStrPtr = std::get_if<std::string>(&decodedAddress);
        CPPUNIT_ASSERT(decodedAddressStrPtr != nullptr);

        auto expectedAddressStrPtr = std::get_if<std::string>(&expectedAddress);
        CPPUNIT_ASSERT(expectedAddressStrPtr != nullptr);

        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Address text string mismatch",
            *decodedAddressStrPtr,
            *expectedAddressStrPtr);
    }
    else if (std::holds_alternative<BytesVector>(decodedAddress))
    {
        CPPUNIT_ASSERT_MESSAGE(
            "Address string type mismatch",
            std::holds_alternative<BytesVector>(expectedAddress));
        auto decodedAddressBytes = std::get_if<BytesVector>(&decodedAddress);
        CPPUNIT_ASSERT(decodedAddressBytes != nullptr);

        auto expectedAddressBytes = std::get_if<BytesVector>(&expectedAddress);
        CPPUNIT_ASSERT(expectedAddressBytes != nullptr);

        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Address bytes string size mismatch",
            decodedAddressBytes->size(),
            expectedAddressBytes->size());

        for (size_t i = 0; i < decodedSignData.size(); i++)
        {
            CPPUNIT_ASSERT_EQUAL_MESSAGE(
                "Address bytes string value mismatch at index " + std::to_string(i),
                decodedAddressBytes->at(i),
                expectedAddressBytes->at(i));
        }
    }
}
