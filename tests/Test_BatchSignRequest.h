#pragma once

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/batch_sign_request.h"

class TestBatchSignRequest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestBatchSignRequest);

    CPPUNIT_TEST(testBatchSignRequestDecoding);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void) {};
    void tearDown(void) {};
    
    void testBatchSignRequestDecoding();

    void Compare(const BatchSignRequest& decodedBatchSignRequest, const BatchSignRequest& expectedBatchSignRequest);
};
