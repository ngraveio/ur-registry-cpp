#pragma once

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../src/keypath.h"

class TestKeypath : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestKeypath);
    CPPUNIT_TEST(EncodeAndDecodeAllFieldsAndTypes);
    CPPUNIT_TEST(EncodeAndDecodeDerivationPathAndDepth);
    CPPUNIT_TEST(EncodeAndDecodeDerivationPathAndFingerprint);
    CPPUNIT_TEST(EncodeAndDecodeEmptyDerivationPath);
    CPPUNIT_TEST(EncodeAndDecodeOnlyDerivationPath);
    CPPUNIT_TEST(EncodeAndDecodeKeypathComponents);
    CPPUNIT_TEST(EncodeIncorrectDerivationPath);
    CPPUNIT_TEST(EncodeIncorrectFingerprint);
    CPPUNIT_TEST(EncodeIncorrectChildIndex);
    CPPUNIT_TEST(EncodeIncorrectChildRangeIndex);
    CPPUNIT_TEST(DecodeRandomOrderMapIndex);
    CPPUNIT_TEST(DecodeIncorrectUrType);
    CPPUNIT_TEST(DecodeUnknownMapIndex);
    CPPUNIT_TEST(DecodeExcludedValue0ForFingerprint);
    CPPUNIT_TEST(DecodeImproperValueForChildIndex);
    CPPUNIT_TEST(DecodeImproperValueWithoutChildComponents);
    CPPUNIT_TEST(DecodeFuzzer);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);
    static void Compare(const Keypath &ur_type, const Keypath &expected_ur_type);

protected:
    void EncodeAndDecodeAllFieldsAndTypes(void);
    void EncodeAndDecodeDerivationPathAndDepth(void);
    void EncodeAndDecodeDerivationPathAndFingerprint(void);
    void EncodeAndDecodeEmptyDerivationPath(void);
    void EncodeAndDecodeOnlyDerivationPath(void);
    void EncodeAndDecodeKeypathComponents(void);
    void EncodeIncorrectDerivationPath(void);
    void EncodeIncorrectFingerprint(void);
    void EncodeIncorrectChildIndex(void);
    void EncodeIncorrectChildRangeIndex(void);
    void DecodeRandomOrderMapIndex(void);
    void DecodeIncorrectUrType(void);
    void DecodeUnknownMapIndex(void);
    void DecodeExcludedValue0ForFingerprint(void);
    void DecodeImproperValueForChildIndex(void);
    void DecodeImproperValueWithoutChildComponents(void);
    void DecodeFuzzer(void);

private:
};
