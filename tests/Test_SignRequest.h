#pragma once


#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ur-registry/src/sign_request.h"

class TestSignRequest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestSignRequest);
    CPPUNIT_TEST(validSignRequestWithBytesStringAddress);
    CPPUNIT_TEST(validSignRequestWithTextStringAddress);
    CPPUNIT_TEST(signRequestMissingMandatoryFields);
    CPPUNIT_TEST(signRequestDecoderFuzzingTests);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);
    
    void validSignRequestWithBytesStringAddress();
    void validSignRequestWithTextStringAddress();
    void signRequestMissingMandatoryFields();
    void signRequestDecoderFuzzingTests();

    static void Compare(const SignRequest& decodedSignRequest, const SignRequest& expectedSignRequest);
    
};
