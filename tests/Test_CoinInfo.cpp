#include "Test_CoinInfo.h"
#include "Test_UrHelpers.h"

void TestCoinInfo::setUp(void)
{

}

void TestCoinInfo::tearDown(void)
{

}

void TestCoinInfo::EncodeBtcMainnet(void)
{
    CoinInfo coinInfo;
    coinInfo.setType(CoinType::Bitcoin);
    coinInfo.setNetwork(NetworkType::MainNet);
    auto expectedBytes = fromHex("A201000200");
    auto expectedLegacyUR = std::string("ur:crypto-coin-info/oeadaeaoaefggwfmbn");
    auto expectedNewUR = "ur:coin-info/oeadaeaoaefggwfmbn";

    ValidateCborResults(coinInfo, expectedBytes);
    coinInfo.setUseLegacyType(true);
    ValidateUrEncoding(coinInfo, expectedLegacyUR);
    coinInfo.setUseLegacyType(false);
    ValidateUrEncoding(coinInfo, expectedNewUR);
}

void TestCoinInfo::EncodeOnlyEthereum(void)
{
    CoinInfo coinInfo;
    coinInfo.setType(CoinType::Ethereum);
    auto expectedBytes = fromHex("A101183C");
    auto expectedLegacyUR = std::string("ur:crypto-coin-info/oyadcsfnksdadlmd");
    auto expectedNewUR = std::string("ur:coin-info/oyadcsfnksdadlmd");

    ValidateCborResults(coinInfo, expectedBytes);
    coinInfo.setUseLegacyType(true);
    ValidateUrEncoding(coinInfo, expectedLegacyUR);
    coinInfo.setUseLegacyType(false);
    ValidateUrEncoding(coinInfo, expectedNewUR);
}

void TestCoinInfo::EncodeOnlyTestnet(void)
{
    CoinInfo coinInfo;
    coinInfo.setNetwork(NetworkType::EthTestNetRopsten);
    auto expectedBytes = fromHex("A10201");
    auto expectedLegacyUR = std::string("ur:crypto-coin-info/oyaoadidsgrfgy");
    auto expectedNewUR = std::string("ur:coin-info/oyaoadidsgrfgy");

    ValidateCborResults(coinInfo, expectedBytes);
    coinInfo.setUseLegacyType(true);
    ValidateUrEncoding(coinInfo, expectedLegacyUR);
    coinInfo.setUseLegacyType(false);
    ValidateUrEncoding(coinInfo, expectedNewUR);
}

void TestCoinInfo::EncodeEmpty(void)
{
    CoinInfo coinInfo;
    auto expectedBytes = fromHex("");
    auto expectedLegacyUR = std::string("ur:crypto-coin-info/aeaeaeae"); // CBOR = '' and CRC32 = '00000000'
    auto expectedNewUR = std::string("ur:coin-info/aeaeaeae");

    ValidateCborResults(coinInfo, expectedBytes);
    coinInfo.setUseLegacyType(true);
    ValidateUrEncoding(coinInfo, expectedLegacyUR);
    coinInfo.setUseLegacyType(false);
    ValidateUrEncoding(coinInfo, expectedNewUR);
}

void TestCoinInfo::DecodeNotSupported(void)
{
    CoinInfo coinInfo;

    auto urToDecode = std::string("ur:crypto-coin-info/oeadaeaoaefggwfmbn");
    ValidateUrDecodingException(coinInfo, urToDecode, CborErrorUnimplementedValidation);

    urToDecode = std::string("ur:coin-info/oeadaeaoaefggwfmbn");
    ValidateUrDecodingException(coinInfo, urToDecode, CborErrorUnimplementedValidation);
}