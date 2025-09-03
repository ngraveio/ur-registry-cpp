#pragma once

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../src/portfolio_metadata.h"

class TestPortfolioMetadata : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestPortfolioMetadata);
    CPPUNIT_TEST(EncodeAllFields);
    CPPUNIT_TEST(EncodeOnlySyncId);
    CPPUNIT_TEST(EncodeOnlyLanguage);
    CPPUNIT_TEST(EncodeOnlyFwVersion);
    CPPUNIT_TEST(EncodeOnlyDevice);
    CPPUNIT_TEST(EncodeEmpty);
    CPPUNIT_TEST(InvalidSyncIdSize);
    CPPUNIT_TEST(InvalidLanguageCode);
    CPPUNIT_TEST(EmptyLanguageCode);
    CPPUNIT_TEST(EmptyFwVersion);
    CPPUNIT_TEST(EmptyDevice);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);

protected:
    void EncodeAllFields(void);
    void EncodeOnlySyncId(void);
    void EncodeOnlyLanguage(void);
    void EncodeOnlyFwVersion(void);
    void EncodeOnlyDevice(void);
    void EncodeEmpty(void);
    void InvalidSyncIdSize(void);
    void InvalidLanguageCode(void);
    void EmptyLanguageCode(void);
    void EmptyFwVersion(void);
    void EmptyDevice(void);

private:
    std::vector<uint8_t> createValidSyncId(void);
};