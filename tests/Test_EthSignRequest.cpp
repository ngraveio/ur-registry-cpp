#include "Test_EthSignRequest.h"
#include "Test_Keypath.h"
#include "Test_UrHelpers.h"

void TestEthSignRequest::setUp(void)
{

}

void TestEthSignRequest::tearDown(void)
{
    
}

EthAddress convertToEthAddress(const std::vector<uint8_t>& vec) {
    if (vec.size() != ETH_ADDRESS_SIZE) {
        throw std::invalid_argument("Vector size does not match ETH_ADDRESS_SIZE.");
    }

    EthAddress result;
    std::copy(vec.begin(), vec.end(), result.begin());
    return result;
}

void TestEthSignRequest::Compare(const EthSignRequest& ur_type, const EthSignRequest& expected_ur_type)
{
    // Check for sign data
    std::vector<uint8_t> decoded_signData = ur_type.getSignData();
    std::vector<uint8_t> expected_signData = expected_ur_type.getSignData();
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Sign data empty mismatch", 
        expected_signData.empty(), 
        decoded_signData.empty()
    );
    if (!decoded_signData.empty()) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Sign data size mismatch",
            expected_signData.size(),
            decoded_signData.size()
        );

        for (size_t i = 0; i < decoded_signData.size(); i++) {
            auto decoded_byte = decoded_signData[i];
            auto expected_byte = expected_signData[i];

            CPPUNIT_ASSERT_EQUAL_MESSAGE(
                "Sign data value mismatch at index " + std::to_string(i),
                expected_byte,
                decoded_byte
            );
        }
    }

    // Check for data type
    DataType decoded_dataType = ur_type.getDataType();
    DataType expected_dataType = expected_ur_type.getDataType();
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Data type mismatch",
        static_cast<int>(expected_dataType),
        static_cast<int>(decoded_dataType)
    );

    // Check for chain id
    uint32_t decoded_chainID = ur_type.getChainID();
    uint32_t expected_chainID = expected_ur_type.getChainID();
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Chain ID mismatch",
        expected_chainID,
        decoded_chainID
    );

    // Check for derivation path
    Keypath decoded_derivationPath = ur_type.getDerivationPath().value();
    Keypath expected_derivationPath = expected_ur_type.getDerivationPath().value();
    TestKeypath::Compare(decoded_derivationPath, expected_derivationPath);

    // Check for request ID (if present)
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Request ID presence mismatch",
        expected_ur_type.getRequestID().has_value(), 
        ur_type.getRequestID().has_value()
    );
    if (ur_type.getRequestID().has_value()){
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Request ID value mismatch",
            expected_ur_type.getRequestID().value().str(), 
            ur_type.getRequestID().value().str()
        );
    }

    // Check for address (if present)
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Address presence mismatch",
        expected_ur_type.getAddress().has_value(), 
        ur_type.getAddress().has_value()
    );
    if (ur_type.getAddress().has_value()){
        EthAddress decoded_address = ur_type.getAddress().value();
        EthAddress expected_address = expected_ur_type.getAddress().value();
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Address empty mismatch", 
            expected_address.empty(), 
            decoded_address.empty()
        );
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Sign data size mismatch",
            expected_address.size(),
            decoded_address.size()
        );

        for (size_t i = 0; i < decoded_address.size(); i++) {
            auto decoded_byte = decoded_address[i];
            auto expected_byte = expected_address[i];

            CPPUNIT_ASSERT_EQUAL_MESSAGE(
                "Sign data value mismatch at index " + std::to_string(i),
                expected_byte,
                decoded_byte
            );
        }
    }

    // Check for origin (if present)
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "Origin presence mismatch",
        expected_ur_type.getOrigin().has_value(), 
        ur_type.getOrigin().has_value()
    );
    if (ur_type.getOrigin().has_value()){
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Origin value mismatch",
            expected_ur_type.getOrigin().value(), 
            ur_type.getOrigin().value()
        );
    }
}

/**
 * @brief Test the decoding of the unique EIP-4527 test vector provided by Keystone team in ur-registry-eth package
 * Source: 
 * - https://eips.ethereum.org/EIPS/eip-4527
 * - https://github.com/KeystoneHQ/keystone-sdk-base/blob/7f62721aaf7cdf25cc04f70f67da3a8658f2fcbe/packages/ur-registry-eth/__tests__/EthSignRequest.test.ts
 * CDDL:
 * {
 *    1: 37(h'9B1DEB4D3B7D4BAD9BDD2B0D7B3DCB6D'), 
 *    2: h'F849808609184E72A00082271094000000000000000000000000000000000000000080A47F7465737432000000000000000000000000000000000000000000000000000000600057808080', 
 *    3: 1, 
 *    4: 1, 
 *    5: 304({1: [44, true, 1, true, 1, true, 0, false, 1, false], 2: 305419896}), 
 *    7: "metamask"
 * }
 * 
 */
void TestEthSignRequest::DecodeSignRequestKeystoneTestCase(void)
{
    EthSignRequest expected_ethsignrequest;
    std::string request_id = "9b1deb4d-3b7d-4bad-9bdd-2b0d7b3dcb6d";
    auto rlpSignatureData = fromHex("f849808609184e72a00082271094000000000000000000000000000000000000000080a47f7465737432000000000000000000000000000000000000000000000000000000600057808080");
    expected_ethsignrequest.setRequestID(Uuid(request_id));
    expected_ethsignrequest.setSignData(rlpSignatureData);
    expected_ethsignrequest.setChainId(1);
    expected_ethsignrequest.setDataType(DataType::EthTransactionData);
    expected_ethsignrequest.setOrigin("metamask");
    CPPUNIT_ASSERT_NO_THROW(expected_ethsignrequest.setDerivationPath("m/44'/1'/1'/0/1", 305419896));

    auto urToDecode = std::string("ur:eth-sign-request/oladtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaohdgryagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaaxadaaadahtaaddyoeadlecsdwykadykadykaewkadwkaocybgeehfksatisjnihjyhsjnhsjkjetlnndant");

    EthSignRequest decoded_ethsignrequest;
    CPPUNIT_ASSERT_NO_THROW(decoded_ethsignrequest.fromUr(urToDecode));
    Compare(decoded_ethsignrequest, expected_ethsignrequest);
}

/**
 * @brief Test the decoding of this CDDL example
 * {
 *    1: 37(h'3ee99776a2734c73a96f80540fde57bc'), 
 *    2: h'02f86a82a4b1028083cdfe608301d13d94912ce59144191c1204e64559fe8253a0e49e654880b844a9059cbb000000000000000000000000371398af172609f57f0f13be4c1aaf48acceb59d0000000000000000000000000000000000000000000000001bc16d674ec80000c0', 
 *    3: 3, 
 *    4: 250, 
 *    5: 304({1: [44, true, 60, true, 0, true, 0, false, 4, false], 2: 108764539, 3: 5}), 
 *    6: h'4838B106FCe9647Bdf1E7877BF73cE8B0BAD5f97',
 *    7: "NGRAVE LIQUID"
 * }
 * 
 */
void TestEthSignRequest::DecodeAllFields(void)
{
    EthSignRequest expected_ethsignrequest;
    std::string request_id = "3ee99776-a273-4c73-a96f-80540fde57bc";
    auto rlpSignatureData = fromHex("02f86a82a4b1028083cdfe608301d13d94912ce59144191c1204e64559fe8253a0e49e654880b844a9059cbb000000000000000000000000371398af172609f57f0f13be4c1aaf48acceb59d0000000000000000000000000000000000000000000000001bc16d674ec80000c0");
    auto address = fromHex("4838B106FCe9647Bdf1E7877BF73cE8B0BAD5f97");
    expected_ethsignrequest.setRequestID(Uuid(request_id));
    expected_ethsignrequest.setSignData(rlpSignatureData);
    expected_ethsignrequest.setChainId(250);
    expected_ethsignrequest.setDataType(DataType::EthRawBytes);
    expected_ethsignrequest.setAddress(convertToEthAddress(address));
    expected_ethsignrequest.setOrigin("NGRAVE LIQUID");
    CPPUNIT_ASSERT_NO_THROW(expected_ethsignrequest.setDerivationPath("m/44'/60'/0'/0/4", 108764539, 5));

    auto urToDecode = std::string("ur:eth-sign-request/osadtpdagdfmwlmskooejkgsjkptjllaghbsuehgrfaohdjnaoyaimlfoxpaaolalssnzehnlsadttfsmwmedwvwmefycfcebgaavafehkzelfgunbvennihfdlarofyptahnsrkaeaeaeaeaeaeaeaeaeaeaeaeembwmkpechdsasyklbbsbwrngscypefdpstorentaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaecwsejnioglspaeaertaxaxaacszsahtaaddyotadlecsdwykcsfnykaeykaewkaawkaocyamkgntkgaxahamghfdetpaamztwliekgurckksktrsjktolubdpmhemsatjnglflgmfphffecxgsgagygogafyfmhhmyet");

    EthSignRequest decoded_ethsignrequest;
    CPPUNIT_ASSERT_NO_THROW(decoded_ethsignrequest.fromUr(urToDecode));
    Compare(decoded_ethsignrequest, expected_ethsignrequest); 
}

/**
 * @brief Test the decoding of this CDDL example
 * {
 *    2: h'02eb83aa36a709843b9aca008502540be400825208940c54fccd2e384b4bb6f2e405bf5cbc15a017aafb8080c0', 
 *    3: 2, 
 *    4: 43444, 
 *    5: 40304({1: [44, true, 60, true, 0, true, 1, false, 40, false]})
 * }
 * 
 */
void TestEthSignRequest::DecodeMinimumFieldsWithRecentKeypathTag(void)
{
    EthSignRequest expected_ethsignrequest;
    auto rlpSignatureData = fromHex("02eb83aa36a709843b9aca008502540be400825208940c54fccd2e384b4bb6f2e405bf5cbc15a017aafb8080c0");
    expected_ethsignrequest.setSignData(rlpSignatureData);
    expected_ethsignrequest.setChainId(43444);
    expected_ethsignrequest.setDataType(DataType::EthTypedData);
    CPPUNIT_ASSERT_NO_THROW(expected_ethsignrequest.setDerivationPath("m/44'/60'/0'/1/40"));

    auto urToDecode = std::string("ur:eth-sign-request/oxaohddpaowmlspkenosaslrfrnysgaelpaoghbdveaelfgmaymwbnghztsndmetgrgrrpwzveahrshhrfbznbchpkzolalartaxaoaacfptqzahtantjooyadlecsdwykcsfnykaeykadwkcsdewkylrygwti");

    EthSignRequest decoded_ethsignrequest;
    CPPUNIT_ASSERT_NO_THROW(decoded_ethsignrequest.fromUr(urToDecode));
    Compare(decoded_ethsignrequest, expected_ethsignrequest); 
}

/**
 * @brief Test the decoding of this CDDL example
 * {
 *    7: "NGRAVE LIQUID",
 *    3: 4,
 *    2: h'02eb83aa36a7098459682f0085151a53460e825208940c54fccd2e384b4bb6f2e405bf5cbc15a017aafb8080c0',
 *    6: h'95222290DD7278Aa3Ddd389Cc1E1d165CC4BAfe5',
 *    1: 37(h'3ee99776a2734c73a96f80540fde57bc'),  
 *    5: 304({1: [44, true, 10, true, 1, true, [20, true, 3, false]], 2: 590023}), 
 *    4: 2
 * }
 * 
 */
void TestEthSignRequest::DecodeRandomOrderMapIndex(void)
{
    EthSignRequest expected_ethsignrequest;
    std::string request_id = "3ee99776-a273-4c73-a96f-80540fde57bc";
    auto rlpSignatureData = fromHex("02eb83aa36a7098459682f0085151a53460e825208940c54fccd2e384b4bb6f2e405bf5cbc15a017aafb8080c0");
    auto address = fromHex("95222290DD7278Aa3Ddd389Cc1E1d165CC4BAfe5");
    expected_ethsignrequest.setRequestID(Uuid(request_id));
    expected_ethsignrequest.setSignData(rlpSignatureData);
    expected_ethsignrequest.setChainId(2);
    expected_ethsignrequest.setDataType(DataType::EthTypedTransaction);
    expected_ethsignrequest.setAddress(convertToEthAddress(address));
    expected_ethsignrequest.setOrigin("NGRAVE LIQUID");
    CPPUNIT_ASSERT_NO_THROW(expected_ethsignrequest.setDerivationPath("m/44'/10'/1'/<20';3>", 590023));

    auto urToDecode = std::string("ur:eth-sign-request/osatjnglflgmfphffecxgsgagygogafyaxaaaohddpaowmlspkenosaslrhkisdlaelpbzcygufgbalfgmaymwbnghztsndmetgrgrrpwzveahrshhrfbznbchpkzolalartamghmdcpcpmhutjpkspkfsutetnssevyttihsfgrpevwadtpdagdfmwlmskooejkgsjkptjllaghbsuehgrfahtaaddyoeadltcsdwykbkykadyklrbbykaxwkaocyaeasaestaaaoynkkwdws");

    EthSignRequest decoded_ethsignrequest;
    CPPUNIT_ASSERT_NO_THROW(decoded_ethsignrequest.fromUr(urToDecode));
    Compare(decoded_ethsignrequest, expected_ethsignrequest); 
}

/**
 * @brief Test the decoding of this CDDL example
 * {
 *    2: h'02eb83aa36a709843b9aca008502540be400825208940c54fccd2e384b4bb6f2e405bf5cbc15a017aafb8080c0', 
 *    4: 43444, 
 *    5: 304({1: [44, true, 60, true, 0, true, 1, false, 40, false]}),
 *    6: h'a377aa6822603Cc52e4e9ed6eaa045c46Ef6b3E2'
 * }
 * 
 */
void TestEthSignRequest::DecodeMissingDataTypeWithDefaultValue(void)
{
    EthSignRequest expected_ethsignrequest;
    auto rlpSignatureData = fromHex("02eb83aa36a709843b9aca008502540be400825208940c54fccd2e384b4bb6f2e405bf5cbc15a017aafb8080c0");
    auto address = fromHex("a377aa6822603Cc52e4e9ed6eaa045c46Ef6b3E2");
    expected_ethsignrequest.setSignData(rlpSignatureData);
    expected_ethsignrequest.setChainId(43444);
    // Default data type value not specified in the CBOR payload
    expected_ethsignrequest.setDataType(DataType::EthTransactionData);
    expected_ethsignrequest.setAddress(convertToEthAddress(address));
    CPPUNIT_ASSERT_NO_THROW(expected_ethsignrequest.setDerivationPath("m/44'/60'/0'/1/40"));

    auto urToDecode = std::string("ur:eth-sign-request/oxaohddpaowmlspkenosaslrfrnysgaelpaoghbdveaelfgmaymwbnghztsndmetgrgrrpwzveahrshhrfbznbchpkzolalartaacfptqzahtaaddyoyadlecsdwykcsfnykaeykadwkcsdewkamghotktpkiscphnfnskdmglnntbwdnbfessjtynqdvoayhyrlat");

    EthSignRequest decoded_ethsignrequest;
    CPPUNIT_ASSERT_NO_THROW(decoded_ethsignrequest.fromUr(urToDecode));
    Compare(decoded_ethsignrequest, expected_ethsignrequest); 
}

/**
 * @brief Test the decoding of this CDDL example
 * {
 *    2: h'02eb83aa36a709843b9aca008502540be400825208940c54fccd2e384b4bb6f2e405bf5cbc15a017aafb8080c0', 
 *    3: 2, 
 *    5: 304({1: [44, true, 60, true, 0, true, 1, false, 40, false]}),
 *    7: "missing chain id"
 * }
 * 
 */
void TestEthSignRequest::DecodeMissingChainIdWithDefaultValue(void)
{
    EthSignRequest expected_ethsignrequest;
    auto rlpSignatureData = fromHex("02eb83aa36a709843b9aca008502540be400825208940c54fccd2e384b4bb6f2e405bf5cbc15a017aafb8080c0");
    expected_ethsignrequest.setSignData(rlpSignatureData);
    // Default chain id value not specified in the CBOR payload
    expected_ethsignrequest.setChainId(1);
    expected_ethsignrequest.setDataType(DataType::EthTypedData);
    expected_ethsignrequest.setOrigin("missing chain id");
    CPPUNIT_ASSERT_NO_THROW(expected_ethsignrequest.setDerivationPath("m/44'/60'/0'/1/40"));

    auto urToDecode = std::string("ur:eth-sign-request/oxaohddpaowmlspkenosaslrfrnysgaelpaoghbdveaelfgmaymwbnghztsndmetgrgrrpwzveahrshhrfbznbchpkzolalartaxaoahtaaddyoyadlecsdwykcsfnykaeykadwkcsdewkatjojninjkjkinjtiocxiaishsinjtcxinieteaxlbfz");

    EthSignRequest decoded_ethsignrequest;
    CPPUNIT_ASSERT_NO_THROW(decoded_ethsignrequest.fromUr(urToDecode));
    Compare(decoded_ethsignrequest, expected_ethsignrequest); 
}

/**
 * @brief Tests returning errors when one of the mandatory field is missing
 * 1.
 * {
 *    3: 2, 
 *    4: 43444, 
 *    5: 304({1: [44, true, 60, true, 0, true, 1, false, 40, false]}),
 *    7: "missing sign data"
 * }
 * 
 * 1.
 * {
 *    1: 37(h'3ee99776a2734c73a96f80540fde57bc'), 
 *    2: h'02eb83aa36a709843b9aca008502540be400825208940c54fccd2e384b4bb6f2e405bf5cbc15a017aafb8080c0', 
 *    3: 2, 
 *    4: 43444, 
 *    7: "missing derivation path"
 * }
 * 
 */
void TestEthSignRequest::DecodeMissingMandatoryFields(void)
{
    EthSignRequest ethsignrequest;
    auto urError = std::string("ur:eth-sign-request/oxaxaoaacfptqzahtaaddyoyadlecsdwykcsfnykaeykadwkcsdewkatjsjninjkjkinjtiocxjkiniojtcxiehsjyhslpayvtte");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorTooFewItems, "1. Missing mandatory field failed");

    urError = std::string("ur:eth-sign-request/onadtpdagdfmwlmskooejkgsjkptjllaghbsuehgrfaohddpaowmlspkenosaslrfrnysgaelpaoghbdveaelfgmaymwbnghztsndmetgrgrrpwzveahrshhrfbznbchpkzolalartaxaoaacfptqzatktjninjkjkinjtiocxieihjpinkohsjyinjljtcxjohsjyistbweecrd");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorTooFewItems, "2. Missing mandatory field failed");
}

/**
 * @brief Tests returning errors when an embedded tag is incorrect
 * 1.
 * {
 *    1: 66(h'3ee99776a2734c73a96f80540fde57bc'), 
 *    2: h'02f86a82a4b1028083cdfe608301d13d94912ce59144191c1204e64559fe8253a0e49e654880b844a9059cbb000000000000000000000000371398af172609f57f0f13be4c1aaf48acceb59d0000000000000000000000000000000000000000000000001bc16d674ec80000c0', 
 *    3: 3, 
 *    4: 250, 
 *    5: 304({1: [44, true, 60, true, 0, true, 0, false, 4, false]}), 
 *    7: "incorrect uuid tag"
 * }
 * 
 * 2.
 * {
 *    1: 37(h'3ee99776a2734c73a96f80540fde57bc'), 
 *    2: h'02f86a82a4b1028083cdfe608301d13d94912ce59144191c1204e64559fe8253a0e49e654880b844a9059cbb000000000000000000000000371398af172609f57f0f13be4c1aaf48acceb59d0000000000000000000000000000000000000000000000001bc16d674ec80000c0', 
 *    3: 3, 
 *    4: 250, 
 *    5: 305({1: [44, true, 60, true, 0, true, 0, false, 4, false]}), 
 *    7: "incorrect keypath tag"
 * }
 * 
 */
void TestEthSignRequest::DecodeIncorrectTags(void)
{
    EthSignRequest ethsignrequest;
    auto urError = std::string("ur:eth-sign-request/oladtpfwgdfmwlmskooejkgsjkptjllaghbsuehgrfaohdjnaoyaimlfoxpaaolalssnzehnlsadttfsmwmedwvwmefycfcebgaavafehkzelfgunbvennihfdlarofyptahnsrkaeaeaeaeaeaeaeaeaeaeaeaeembwmkpechdsasyklbbsbwrngscypefdpstorentaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaecwsejnioglspaeaertaxaxaacszsahtaaddyoyadlecsdwykcsfnykaeykaewkaawkatjpinjtiajljpjpihiajycxkpkpiniecxjyhsiohsprdkwp");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorInappropriateTagForType, "1. Incorrect tag failed");
    
    urError = std::string("ur:eth-sign-request/oladtpdagdfmwlmskooejkgsjkptjllaghbsuehgrfaohdjnaoyaimlfoxpaaolalssnzehnlsadttfsmwmedwvwmefycfcebgaavafehkzelfgunbvennihfdlarofyptahnsrkaeaeaeaeaeaeaeaeaeaeaeaeembwmkpechdsasyklbbsbwrngscypefdpstorentaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaecwsejnioglspaeaertaxaxaacszsahtaadehoyadlecsdwykcsfnykaeykaewkaawkatkpinjtiajljpjpihiajycxjeihkkjohsjyiscxjyhsiowshkbszm");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorInappropriateTagForType, "2. Incorrect tag failed");
}

/**
 * @brief Tests returning errors when the map key is not unique
 * 1.
 * {
 *    1: 37(h'9B1DEB4D3B7D4BAD9BDD2B0D7B3DCB6D'), 
 *    2: h'02f86a82a4b1028083cdfe608301d13d94912ce59144191c1204e64559fe8253a0e49e654880b844a9059cbb000000000000000000000000371398af172609f57f0f13be4c1aaf48acceb59d0000000000000000000000000000000000000000000000001bc16d674ec80000c0', 
 *    1: 37(h'9B1DEB4D3B7D4BAD9BDD2B0D7B3DCB6D'),
 *    3: 3, 
 *    4: 250, 
 *    5: 304({1: [44, true, 60, true, 0, true, 0, false, 4, false], 2: 108764539, 3: 5}), 
 *    7: "Duplicated request id"
 * }
 * 
 * 2.
 * { 
 *    2: h'02f86a82a4b1028083cdfe608301d13d94912ce59144191c1204e64559fe8253a0e49e654880b844a9059cbb000000000000000000000000371398af172609f57f0f13be4c1aaf48acceb59d0000000000000000000000000000000000000000000000001bc16d674ec80000c0', 
 *    3: 3, 
 *    4: 250, 
 *    2: h'02f86a82a4b1028083cdfe608301d13d94912ce59144191c1204e64559fe8253a0e49e654880b844a9059cbb000000000000000000000000371398af172609f57f0f13be4c1aaf48acceb59d0000000000000000000000000000000000000000000000001bc16d674ec80000c0',
 *    5: 304({1: [44, true, 60, true, 0, true, 0, false, 4, false]}), 
 *    6: h'4838B106FCe9647Bdf1E7877BF73cE8B0BAD5f97',
 *    7: "Duplicated sign data"
 * }
 * 
 * 3.
 * {
 *    1: 37(h'3ee99776a2734c73a96f80540fde57bc'), 
 *    2: h'02f86a82a4b1028083cdfe608301d13d94912ce59144191c1204e64559fe8253a0e49e654880b844a9059cbb000000000000000000000000371398af172609f57f0f13be4c1aaf48acceb59d0000000000000000000000000000000000000000000000001bc16d674ec80000c0', 
 *    3: 3, 
 *    4: 250, 
 *    5: 304({1: [44, true, 60, true, 0, true, 0, false, 4, false], 2: 108764539}), 
 *    7: "Duplicated data type",
 *    3: 3
 * }
 * 
 * 4.
 * {
 *    2: h'02f86a82a4b1028083cdfe608301d13d94912ce59144191c1204e64559fe8253a0e49e654880b844a9059cbb000000000000000000000000371398af172609f57f0f13be4c1aaf48acceb59d0000000000000000000000000000000000000000000000001bc16d674ec80000c0', 
 *    3: 3, 
 *    4: 1, 
 *    4: 1, 
 *    5: 304({1: [44, true, 60, true, 0, true, 0, false, 4, false]}), 
 *    7: "Duplicated chain id"
 * }
 * 
 * 5.
 * {
 *    1: 37(h'3ee99776a2734c73a96f80540fde57bc'), 
 *    2: h'02f86a82a4b1028083cdfe608301d13d94912ce59144191c1204e64559fe8253a0e49e654880b844a9059cbb000000000000000000000000371398af172609f57f0f13be4c1aaf48acceb59d0000000000000000000000000000000000000000000000001bc16d674ec80000c0', 
 *    3: 3, 
 *    4: 250, 
 *    5: 304({1: [44, true, 60, true, 0, true, 0, false, 4, false], 2: 2, 3: 5}), 
 *    6: h'4838B106FCe9647Bdf1E7877BF73cE8B0BAD5f97',
 *    5: 304({1: [44, true, 60, true, 0, true, 0, false, 4, false], 2: 2, 3: 5})
 * }
 * 
 * 6.
 * {
 *    2: h'02f86a82a4b1028083cdfe608301d13d94912ce59144191c1204e64559fe8253a0e49e654880b844a9059cbb000000000000000000000000371398af172609f57f0f13be4c1aaf48acceb59d0000000000000000000000000000000000000000000000001bc16d674ec80000c0', 
 *    3: 3, 
 *    4: 250, 
 *    5: 304({1: [44, true, 60, true, 0, true, 0, false, 4, false], 2: 108764539}), 
 *    6: h'4838B106FCe9647Bdf1E7877BF73cE8B0BAD5f97',
 *    6: h'4838B106FCe9647Bdf1E7877BF73cE8B0BAD5f97',
 *    7: "Duplicated address"
 * }
 * 
 * 7.
 * {
 *    2: h'02f86a82a4b1028083cdfe608301d13d94912ce59144191c1204e64559fe8253a0e49e654880b844a9059cbb000000000000000000000000371398af172609f57f0f13be4c1aaf48acceb59d0000000000000000000000000000000000000000000000001bc16d674ec80000c0', 
 *    3: 3, 
 *    4: 250, 
 *    5: 304({1: [44, true, 60, true, 0, true, 0, false, 4, false], 2: 108764539}), 
 *    6: h'4838B106FCe9647Bdf1E7877BF73cE8B0BAD5f97',
 *    7: "Duplicated origin",
 *    7: "Duplicated origin"
 * }
 * 
 */
void TestEthSignRequest::DecodeNotUniqueMapKeys(void)
{
    EthSignRequest ethsignrequest;
    auto urError = std::string("ur:eth-sign-request/osadtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaohdjnaoyaimlfoxpaaolalssnzehnlsadttfsmwmedwvwmefycfcebgaavafehkzelfgunbvennihfdlarofyptahnsrkaeaeaeaeaeaeaeaeaeaeaeaeembwmkpechdsasyklbbsbwrngscypefdpstorentaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaecwsejnioglspaeaertadtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaxaxaacszsahtaaddyotadlecsdwykcsfnykaeykaewkaawkaocyamkgntkgaxahatkpfykpjojziniahsjyihiecxjpihjskpihjkjycxiniejoinykts");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorMapKeysNotUnique, "1. Map key not unique failed");
    
    urError = std::string("ur:eth-sign-request/osaohdgryagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaaxaxaacszsaohdgryagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaahtaaddyoyadlecsdwykcsfnykaeykaewkaawkamghfdetpaamztwliekgurckksktrsjktolubdpmhemsatjyfykpjojziniahsjyihiecxjkiniojtcxiehsjyhstebejoey");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorMapKeysNotUnique, "2. Map key not unique failed");

    urError = std::string("ur:eth-sign-request/osadtpdagdfmwlmskooejkgsjkptjllaghbsuehgrfaohdjnaoyaimlfoxpaaolalssnzehnlsadttfsmwmedwvwmefycfcebgaavafehkzelfgunbvennihfdlarofyptahnsrkaeaeaeaeaeaeaeaeaeaeaeaeembwmkpechdsasyklbbsbwrngscypefdpstorentaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaecwsejnioglspaeaertaxaxaacszsahtaaddyoeadlecsdwykcsfnykaeykaewkaawkaocyamkgntkgatjyfykpjojziniahsjyihiecxiehsjyhscxjykkjoihaxaxiewswsrs");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorMapKeysNotUnique, "3. Map key not unique failed");

    urError = std::string("ur:eth-sign-request/olaohdjnaoyaimlfoxpaaolalssnzehnlsadttfsmwmedwvwmefycfcebgaavafehkzelfgunbvennihfdlarofyptahnsrkaeaeaeaeaeaeaeaeaeaeaeaeembwmkpechdsasyklbbsbwrngscypefdpstorentaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaecwsejnioglspaeaertaxaxaaadaaadahtaaddyoyadlecsdwykcsfnykaeykaewkaawkatjkfykpjojziniahsjyihiecxiaishsinjtcxiniepeynpygw");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorMapKeysNotUnique, "4. Map key not unique failed");

    urError = std::string("ur:eth-sign-request/osadtpdagdfmwlmskooejkgsjkptjllaghbsuehgrfaohdjnaoyaimlfoxpaaolalssnzehnlsadttfsmwmedwvwmefycfcebgaavafehkzelfgunbvennihfdlarofyptahnsrkaeaeaeaeaeaeaeaeaeaeaeaeembwmkpechdsasyklbbsbwrngscypefdpstorentaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaecwsejnioglspaeaertaxaxaacszsahtaaddyotadlecsdwykcsfnykaeykaewkaawkaoaoaxahamghfdetpaamztwliekgurckksktrsjktolubdpmhemsahtaaddyotadlecsdwykcsfnykaeykaewkaawkaoaoaxahkoahfewe");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorMapKeysNotUnique, "5. Map key not unique failed");

    urError = std::string("ur:eth-sign-request/osaohdjnaoyaimlfoxpaaolalssnzehnlsadttfsmwmedwvwmefycfcebgaavafehkzelfgunbvennihfdlarofyptahnsrkaeaeaeaeaeaeaeaeaeaeaeaeembwmkpechdsasyklbbsbwrngscypefdpstorentaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaecwsejnioglspaeaertaxaxaacszsahtaaddyoeadlecsdwykcsfnykaeykaewkaawkaocyamkgntkgamghfdetpaamztwliekgurckksktrsjktolubdpmhemsatjpfykpjojziniahsjyihiecxhsieiejpihjkjkamkgntkgamghfdetpaamztwliekgurckksktrsjktolubdpmhemsmylfvdyk");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorMapKeysNotUnique, "6. Map key not unique failed");

    urError = std::string("ur:eth-sign-request/osaohdjnaoyaimlfoxpaaolalssnzehnlsadttfsmwmedwvwmefycfcebgaavafehkzelfgunbvennihfdlarofyptahnsrkaeaeaeaeaeaeaeaeaeaeaeaeembwmkpechdsasyklbbsbwrngscypefdpstorentaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaecwsejnioglspaeaertaxaxaacszsahtaaddyoeadlecsdwykcsfnykaeykaewkaawkaocyamkgntkgamghfdetpaamztwliekgurckksktrsjktolubdpmhemsatjsfykpjojziniahsjyihiecxjljpinioinjtatjsfykpjojziniahsjyihiecxjljpinioinjteegmlktk");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorMapKeysNotUnique, "7. Map key not unique failed");
}

/**
 * @brief Tests returning errors when the size is incorrect for specific fields
 * 1.
 * {
 *    1: 37(h'0009845E3A'), 
 *    2: h'02f86a82a4b1028083cdfe608301d13d94912ce59144191c1204e64559fe8253a0e49e654880b844a9059cbb000000000000000000000000371398af172609f57f0f13be4c1aaf48acceb59d0000000000000000000000000000000000000000000000001bc16d674ec80000c0', 
 *    3: 3, 
 *    4: 250, 
 *    5: 304({1: [44, true, 60, true, 0, true, 0, false, 4, false]}), 
 *    7: "incorrect uuid size"
 * }
 * 
 * 2.
 * {
 *    1: 37(h'3ee99776a2734c73a96f80540fde57bc'), 
 *    2: h'02f86a82a4b1028083cdfe608301d13d94912ce59144191c1204e64559fe8253a0e49e654880b844a9059cbb000000000000000000000000371398af172609f57f0f13be4c1aaf48acceb59d0000000000000000000000000000000000000000000000001bc16d674ec80000c0', 
 *    3: 3, 
 *    4: 250, 
 *    5: 304({1: [44, true, 60, true, 0, true, 0, false, 4, false]}), 
 *    6: h'4838B106FCe9647Bdf1E7877BF73cE8B0BAD5f97FFFFFFFF',
 *    7: "incorrect address size"
 * }
 * 
 */
void TestEthSignRequest::DecodeIncorrectSize(void)
{
    EthSignRequest ethsignrequest;
    auto urError = std::string("ur:eth-sign-request/oladtpdafeaeaslrhyftaohdjnaoyaimlfoxpaaolalssnzehnlsadttfsmwmedwvwmefycfcebgaavafehkzelfgunbvennihfdlarofyptahnsrkaeaeaeaeaeaeaeaeaeaeaeaeembwmkpechdsasyklbbsbwrngscypefdpstorentaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaecwsejnioglspaeaertaxaxaacszsahtaaddyoyadlecsdwykcsfnykaeykaewkaawkatjkinjtiajljpjpihiajycxkpkpiniecxjkinknihplcedkrl");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorImproperValue, "1. Incorrect size failed");
    
    urError = std::string("ur:eth-sign-request/osadtpdagdfmwlmskooejkgsjkptjllaghbsuehgrfaohdjnaoyaimlfoxpaaolalssnzehnlsadttfsmwmedwvwmefycfcebgaavafehkzelfgunbvennihfdlarofyptahnsrkaeaeaeaeaeaeaeaeaeaeaeaeembwmkpechdsasyklbbsbwrngscypefdpstorentaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaecwsejnioglspaeaertaxaxaacszsahtaaddyoyadlecsdwykcsfnykaeykaewkaawkamhdcsfdetpaamztwliekgurckksktrsjktolubdpmhemszmzmzmzmatkoinjtiajljpjpihiajycxhsieiejpihjkjkcxjkinknihstsoyagw");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorImproperValue, "2. Incorrect size failed");
}

/**
 * @brief Fuzzing of the decoder with several malformed eth-sign-request UR types 
 * 
 */
void TestEthSignRequest::DecodeFuzzer(void)
{
    EthSignRequest ethsignrequest;

    /* Invalid types */
    auto urError = std::string("ur:eth-sign-request/oxadaeaoahaxamaaatzswfimgd");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorIllegalType, "1. Fuzzer failed");

    /* Not enough map keys */
    urError = std::string("ur:eth-sign-request/oyadaefmvturaa");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorImproperValue, "2. Fuzzer failed");

    /* Invalid map key */
    urError = std::string("ur:eth-sign-request/osadtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaohdgryagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaaxadaaadahtaaddyoeadlecsdwykadykadykaewkadwkaocybgeehfksatisjnihjyhsjnhsjkjebkjlinjtkohsjziniecxjnhsjocxjeihkkaomttamo");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorUnknownType, "3. Fuzzer failed");

    /* Number of map key incorrectly defined in CBOR */
    urError = std::string("ur:eth-sign-request/onadtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaohdgryagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaaxadaaadahtaaddyoeadlecsdwykadykadykaewkadwkao");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorIO, "4. Fuzzer failed");

    /* Key index type corrupted */
    urError = std::string("ur:eth-sign-request/oladtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaohdgryagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaotadaaadahtaaddyoeadlecsdwykadykadykaewkadwkaocybgeehfksatisjnihjyhsjnhsjkjespgegtpt");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorIllegalType, "5. Fuzzer failed");

    /* Request id type corrupted */
    urError = std::string("ur:eth-sign-request/oladvsdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaohdgryagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaaxadaaadahtaaddyoeadlecsdwykadykadykaewkadwkaocybgeehfksatisjnihjyhsjnhsjkjebwpyylva");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorIllegalType, "6. Fuzzer failed");

    /* Sign data byte size corrupted */
    urError = std::string("ur:eth-sign-request/oladtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaohdbsyagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaaxadaaadahtaaddyoeadlecsdwykadykadykaewkadwkaocybgeehfksatisjnihjyhsjnhsjkjepmrdftpe");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorUnknownType, "7. Fuzzer failed");

    /* Sign data type corrupted */
    urError = std::string("ur:eth-sign-request/oladtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnpahdgryagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaaxadaaadahtaaddyoeadlecsdwykadykadykaewkadwkaocybgeehfksatisjnihjyhsjnhsjkjeehmygada");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorIllegalType, "8. Fuzzer failed");

    /* Data type type corrupted */
    urError = std::string("ur:eth-sign-request/oladtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaohdgryagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaaxhnaaadahtaaddyoeadlecsdwykadykadykaewkadwkaocybgeehfksatisjnihjyhsjnhsjkjesamtzmgt");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorIllegalType, "9. Fuzzer failed");

    /* Chain id type corrupted */
    urError = std::string("ur:eth-sign-request/oladtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaohdgryagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaaxadjoadahtaaddyoeadlecsdwykadykadykaewkadwkaocybgeehfksatisjnihjyhsjnhsjkjedsndhdhl");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorIllegalType, "10. Fuzzer failed");

    /* Derivation path type corrupted */
    urError = std::string("ur:eth-sign-request/oladtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaohdgryagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaaxadaaksahtaaddyoeadlecsdwykadykadykaewkadwkaocybgeehfksatisjnihjyhsjnhsjkjevetefdvt");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorIllegalType, "11. Fuzzer failed");

    /* Address type corrupted */
    urError = std::string("ur:eth-sign-request/oladtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaohdgryagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaaxadaaadahpaaddyoeadlecsdwykadykadykaewkadwkaocybgeehfksatisjnihjyhsjnhsjkjeaegmosdn");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorIllegalType, "12. Fuzzer failed");

    /* Origin type corrupted */
    urError = std::string("ur:eth-sign-request/osadtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaohdgryagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaaxadaaadahtaaddyoeadlecsdwykadykadykaewkadwkaocybgeehfksameoaeaeaeaeaeaeaeaeaeaeaeatisjnihjyhsjnhsjkjeuegsehkn");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorIllegalType, "13. Fuzzer failed");

    /* Empty map: {} */
    urError = std::string("ur:eth-sign-request/nbaatygsih");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorImproperValue, "14. Fuzzer failed");

    /* Empty content */
    urError = std::string("ur:eth-sign-request/");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorIO, "15. Fuzzer failed");

    /* Random string */
    urError = std::string("ur:eth-sign-request/osadtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaohdgryagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaaxadaaadahtaaddyoeadlecsdwykadykadykaewkadwkaocybgeehfksamgraeaeaeaeaeaeaeaeaeaeaeatisjnihjyhsjnhsjkjelkdtwpfhadded");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorIO, "16. Fuzzer failed");

    /* Invalid UR with random min bytewords */
    urError = std::string("ur:eth-sign-request/frkigrpmndutdnbtkgfssblaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeae");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorIO, "17. Fuzzer failed");

    /* Valid UR with random min bytewords */
    urError = std::string("ur:eth-sign-request/pezmtlioaeqzeyjssfaslpfghklgdyfn");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorImproperValue, "18. Fuzzer failed");

    /* Invalid CRC32 checksum in UR */
    urError = std::string("ur:eth-sign-request/oladtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaohdgryagalalnascsgljpnbaelfdibemwaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaelaoxlbjyihjkjyeyaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaehnaehglalalaaxadaaadahtaaddyoeadlecsdwykadykadykaewkadwkaocybgeehfksatisjnihjyhsjnhsjkjetlmhdant");
    ValidateUrDecodingException(ethsignrequest, urError, CborErrorIO, "19. Fuzzer failed");
}
