#pragma once

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../src/coininfo.h"

class TestCoinInfo : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCoinInfo);
    CPPUNIT_TEST(EncodeBtcMainnet);
    CPPUNIT_TEST(EncodeOnlyEthereum);
    CPPUNIT_TEST(EncodeOnlyTestnet);
    CPPUNIT_TEST(EncodeEmpty);
    CPPUNIT_TEST(DecodeNotSupported);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);

protected:
    void EncodeBtcMainnet(void);
    void EncodeOnlyEthereum(void);
    void EncodeOnlyTestnet(void);
    void EncodeEmpty(void);
    void DecodeNotSupported(void);

private:

};

