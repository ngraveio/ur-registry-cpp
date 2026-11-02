#pragma once

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/intent.h"

class TestIntent : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestIntent);

    CPPUNIT_TEST(testValidIntent);
    CPPUNIT_TEST(testIntentWrongParamTupleLength);
    CPPUNIT_TEST(testIntentEncodingMaxUint16);
    CPPUNIT_TEST(testIntentEncodingOutOfRange);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void) {}
    void tearDown(void) {}

    void testValidIntent();
    void testIntentWrongParamTupleLength();
    void testIntentEncodingMaxUint16();
    void testIntentEncodingOutOfRange();

    static void Compare(const Intent &decodedIntent, const Intent &expectedIntent);
};
