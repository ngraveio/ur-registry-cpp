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
    QByteArray expectedBytes = QByteArray::fromHex("A201000200");
    QString expectedLegacyUR = "ur:crypto-coin-info/oeadaeaoaefggwfmbn";
    QString expectedNewUR = "ur:coin-info/oeadaeaoaefggwfmbn";

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
    QByteArray expectedBytes = QByteArray::fromHex("A101183C");
    QString expectedLegacyUR = "ur:crypto-coin-info/oyadcsfnksdadlmd";
    QString expectedNewUR = "ur:coin-info/oyadcsfnksdadlmd";

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
    QByteArray expectedBytes = QByteArray::fromHex("A10201");
    QString expectedLegacyUR = "ur:crypto-coin-info/oyaoadidsgrfgy";
    QString expectedNewUR = "ur:coin-info/oyaoadidsgrfgy";

    ValidateCborResults(coinInfo, expectedBytes);
    coinInfo.setUseLegacyType(true);
    ValidateUrEncoding(coinInfo, expectedLegacyUR);
    coinInfo.setUseLegacyType(false);
    ValidateUrEncoding(coinInfo, expectedNewUR);
}

void TestCoinInfo::EncodeEmpty(void)
{
    CoinInfo coinInfo;
    QByteArray expectedBytes = QByteArray::fromHex("");
    QString expectedLegacyUR = "ur:crypto-coin-info/aeaeaeae"; // CBOR = '' and CRC32 = '00000000'
    QString expectedNewUR = "ur:coin-info/aeaeaeae";

    ValidateCborResults(coinInfo, expectedBytes);
    coinInfo.setUseLegacyType(true);
    ValidateUrEncoding(coinInfo, expectedLegacyUR);
    coinInfo.setUseLegacyType(false);
    ValidateUrEncoding(coinInfo, expectedNewUR);
}

void TestCoinInfo::DecodeNotSupported(void)
{
    CoinInfo coinInfo;

    QString urToDecode = "ur:crypto-coin-info/oeadaeaoaefggwfmbn";
    ValidateUrDecodingException(coinInfo, urToDecode, CborErrorUnimplementedValidation);

    urToDecode = "ur:coin-info/oeadaeaoaefggwfmbn";
    ValidateUrDecodingException(coinInfo, urToDecode, CborErrorUnimplementedValidation);
}