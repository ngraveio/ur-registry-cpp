#pragma once

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class TestUrSignatures : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestUrSignatures);
    CPPUNIT_TEST(EncodeSignatureKeystoneTestCase);
    CPPUNIT_TEST(EncodeSignatureWithoutOriginAndSignatureAbove65B);
    CPPUNIT_TEST(EncodeSignatureWithInvalidRequestId);
    CPPUNIT_TEST(EncodeSignatureWithSizeBelow65B);
    CPPUNIT_TEST(EncodeSignatureWithoutRequestId);
    CPPUNIT_TEST(EncodeBatchSignResponse);
    CPPUNIT_TEST(EncodeCosmosSignatureKeystoneTestcase);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void EncodeSignatureKeystoneTestCase();
    void EncodeSignatureWithoutOriginAndSignatureAbove65B();
    void EncodeSignatureWithInvalidRequestId();
    void EncodeSignatureWithSizeBelow65B();
    void EncodeSignatureWithoutRequestId();
    void EncodeBatchSignResponse();
    void EncodeCosmosSignatureKeystoneTestcase();

private:

};
