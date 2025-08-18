#pragma once

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "libs/ur-registry/src/eth_sign_request.h"

class TestEthSignRequest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestEthSignRequest);
    CPPUNIT_TEST(DecodeSignRequestKeystoneTestCase);
    CPPUNIT_TEST(DecodeAllFields);
    CPPUNIT_TEST(DecodeMinimumFieldsWithRecentKeypathTag);
    CPPUNIT_TEST(DecodeRandomOrderMapIndex);
    CPPUNIT_TEST(DecodeMissingDataTypeWithDefaultValue);
    CPPUNIT_TEST(DecodeMissingChainIdWithDefaultValue);
    CPPUNIT_TEST(DecodeMissingMandatoryFields);
    CPPUNIT_TEST(DecodeIncorrectTags);
    CPPUNIT_TEST(DecodeNotUniqueMapKeys);
    CPPUNIT_TEST(DecodeIncorrectSize);
    CPPUNIT_TEST(DecodeFuzzer);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);
    static void Compare(const EthSignRequest& ur_type, const EthSignRequest& expected_ur_type);

protected:
    void DecodeSignRequestKeystoneTestCase(void);
    void DecodeAllFields(void);
    void DecodeMinimumFieldsWithRecentKeypathTag(void);
    void DecodeRandomOrderMapIndex(void);
    void DecodeMissingDataTypeWithDefaultValue(void);
    void DecodeMissingChainIdWithDefaultValue(void);
    void DecodeMissingMandatoryFields(void);
    void DecodeIncorrectTags(void);
    void DecodeNotUniqueMapKeys(void);
    void DecodeIncorrectSize(void);
    void DecodeFuzzer(void);

private:

};
