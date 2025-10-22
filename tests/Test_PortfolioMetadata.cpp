#include "Test_PortfolioMetadata.h"
#include "Test_UrHelpers.h"

void TestPortfolioMetadata::setUp(void)
{
}

void TestPortfolioMetadata::tearDown(void)
{
}

std::vector<uint8_t> TestPortfolioMetadata::createValidSyncId(void)
{
    // Create a 16-byte sync ID
    return std::vector<uint8_t>{
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
}

void TestPortfolioMetadata::EncodeAllFields(void)
{
    PortfolioMetadata metadata;
    metadata.setSyncId(createValidSyncId());
    metadata.setLanguage("en");
    metadata.setFwVersion("1.7-2.rc");
    metadata.setDevice("NGRAVE");

    // A4                                      # map(4)
    //    01                                   # unsigned(1) sync_id
    //    50                                   # bytes(16)
    //       0102030405060708090A0B0C0D0E0F10 # sync_id bytes
    //    02                                   # unsigned(2) language
    //    62                                   # text(2)
    //       656E                              # "en"
    //    03                                   # unsigned(3) fw_version
    //    68                                   # text(8)
    //       312E372D322E7263                  # "1.7-2.rc"
    //    04                                   # unsigned(4) device
    //    66                                   # text(6)
    //       4E4752415645                      # "NGRAVE"

    auto expectedBytes = fromHex("A401500102030405060708090A0B0C0D0E0F100262656E0368312E372D322E72630466"
                                 "4E4752415645");
    auto expectedUR = "ur:portfolio-metadata/oxadgdadaoaxaaahamatayasbkbdbnbtbabsbeaoidihjtaxisehdmemdpeydmjpiaaaiyglflgmfphffejkuejscf";

    ValidateCborResults(metadata, expectedBytes);
    ValidateUrEncoding(metadata, expectedUR);
}

void TestPortfolioMetadata::EncodeOnlySyncId(void)
{
    PortfolioMetadata metadata;
    metadata.setSyncId(createValidSyncId());

    // A1                                      # map(1)
    //    01                                   # unsigned(1)
    //    50                                   # bytes(16)
    //       0102030405060708090A0B0C0D0E0F10 # sync_id bytes

    auto expectedBytes = fromHex("A101500102030405060708090A0B0C0D0E0F10");
    auto expectedUR = "ur:portfolio-metadata/oyadgdadaoaxaaahamatayasbkbdbnbtbabsbewnsniyrs";

    ValidateCborResults(metadata, expectedBytes);
    ValidateUrEncoding(metadata, expectedUR);
}

void TestPortfolioMetadata::EncodeOnlyLanguage(void)
{
    PortfolioMetadata metadata;
    metadata.setLanguage("fr");

    // A1                                      # map(1)
    //    02                                   # unsigned(2)
    //    62                                   # text(2)
    //       6672                              # "fr"

    auto expectedBytes = fromHex("A102626672");
    auto expectedUR = "ur:portfolio-metadata/oyaoidiyjpvdmugetk";

    ValidateCborResults(metadata, expectedBytes);
    ValidateUrEncoding(metadata, expectedUR);
}

void TestPortfolioMetadata::EncodeOnlyFwVersion(void)
{
    PortfolioMetadata metadata;
    metadata.setFwVersion("1.0.0");

    // A1                                      # map(1)
    //    03                                   # unsigned(3)
    //    65                                   # text(5)
    //       312E302E30                        # "1.0.0"

    auto expectedBytes = fromHex("A10365312E302E30");
    auto expectedUR = "ur:portfolio-metadata/oyaxihehdmdydmdyrsbtdtdk";

    ValidateCborResults(metadata, expectedBytes);
    ValidateUrEncoding(metadata, expectedUR);
}

void TestPortfolioMetadata::EncodeOnlyDevice(void)
{
    PortfolioMetadata metadata;
    metadata.setDevice("ZERO");

    // A1                                      # map(1)
    //    04                                   # unsigned(4)
    //    64                                   # text(4)
    //       5A45524F                          # "ZERO"

    auto expectedBytes = fromHex("A104645A45524F");
    auto expectedUR = "ur:portfolio-metadata/oyaaiehtfegmgwemnshgah";

    ValidateCborResults(metadata, expectedBytes);
    ValidateUrEncoding(metadata, expectedUR);
}

void TestPortfolioMetadata::EncodeEmpty(void)
{
    PortfolioMetadata metadata;
    auto expectedBytes = fromHex("");
    auto expectedUR = "ur:portfolio-metadata/aeaeaeae"; // CBOR = '' and CRC32 = '00000000'

    ValidateCborResults(metadata, expectedBytes);
    ValidateUrEncoding(metadata, expectedUR);
}

void TestPortfolioMetadata::InvalidSyncIdSize(void)
{
    PortfolioMetadata metadata;
    std::vector<uint8_t> invalidSyncId{0x01, 0x02, 0x03}; // Only 3 bytes instead of 16

    CPPUNIT_ASSERT_THROW(metadata.setSyncId(invalidSyncId), CborException);
    try
    {
        metadata.setSyncId(invalidSyncId);
    }
    catch (const CborException &e)
    {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Error code different than expected",
            CborErrorImproperValue,
            e.errorCode());
    }
}

void TestPortfolioMetadata::InvalidLanguageCode(void)
{
    PortfolioMetadata metadata;

    // Test language code that's too long (should be 2 chars per ISO 639-1)
    auto invalidLongCode = "eng";
    auto invalidCharsCode = "1n";

    // Test invalid language code with wrong length
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw on language code that's too long",
        metadata.setLanguage(invalidLongCode),
        CborException);
    try
    {
        metadata.setLanguage(invalidLongCode);
    }
    catch (const CborException &e)
    {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Error code different than expected",
            CborErrorImproperValue,
            e.errorCode());
    }

    // Test invalid language code with invalid characters
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw on language code with invalid characters",
        metadata.setLanguage(invalidCharsCode),
        CborException);
    try
    {
        metadata.setLanguage(invalidCharsCode);
    }
    catch (const CborException &e)
    {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Error code different than expected",
            CborErrorImproperValue,
            e.errorCode());
    }
}

void TestPortfolioMetadata::EmptyLanguageCode(void)
{
    PortfolioMetadata metadata;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw on empty language code",
        metadata.setLanguage(""),
        CborException);
    try
    {
        metadata.setLanguage("");
    }
    catch (const CborException &e)
    {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Error code different than expected",
            CborErrorImproperValue,
            e.errorCode());
    }
}

void TestPortfolioMetadata::EmptyFwVersion(void)
{
    PortfolioMetadata metadata;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw on empty firmware version",
        metadata.setFwVersion(""),
        CborException);
    try
    {
        metadata.setFwVersion("");
    }
    catch (const CborException &e)
    {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Error code different than expected",
            CborErrorImproperValue,
            e.errorCode());
    }
}

void TestPortfolioMetadata::EmptyDevice(void)
{
    PortfolioMetadata metadata;

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw on empty device name",
        metadata.setDevice(""),
        CborException);
    try
    {
        metadata.setDevice("");
    }
    catch (const CborException &e)
    {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            "Error code different than expected",
            CborErrorImproperValue,
            e.errorCode());
    }
}
