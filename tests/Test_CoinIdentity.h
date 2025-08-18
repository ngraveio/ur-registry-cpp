#ifndef TEST_COINIDENTITY_H
#define TEST_COINIDENTITY_H

#include <cppunit/extensions/HelperMacros.h>
#include "libs/ur-registry/src/coin_identity.h"

class TestCoinIdentity : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(TestCoinIdentity);
    CPPUNIT_TEST(TestAllFields);
    CPPUNIT_TEST(TestMinimumFields);
    CPPUNIT_TEST(TestInvalidType);
    CPPUNIT_TEST(TestInvalidCurve);
    CPPUNIT_TEST(TestMissingMandatoryFields);
    CPPUNIT_TEST(TestNotUniqueMapKeys);
    CPPUNIT_TEST(TestInvalidSubtypes);
    CPPUNIT_TEST(TestEncodingUaiExamples);
    CPPUNIT_TEST(TestInvalidUaiStrings);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();
    static void Compare(const CoinIdentity& decoded, const CoinIdentity& expected);

protected:
    void TestAllFields();
    void TestInvalidType();
    void TestInvalidCurve();
    void TestMinimumFields();
    void TestMissingMandatoryFields();
    void TestNotUniqueMapKeys();
    void TestInvalidSubtypes();
    void TestEncodingUaiExamples();
    void TestInvalidUaiStrings();
};

#endif // TEST_COINIDENTITY_H
