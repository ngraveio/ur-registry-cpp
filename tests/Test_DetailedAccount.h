#pragma once

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class TestDetailedAccount : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestDetailedAccount);
    CPPUNIT_TEST(testDetailedAccountWithHDKey);
    CPPUNIT_TEST(testDetailedAccountWithOutputDesc);
    CPPUNIT_TEST(testDetailedAccountWithUninitAccount);
    CPPUNIT_TEST(testDetailedAccountWithLegacyFlag);

    CPPUNIT_TEST(UaiExampleBtcTpubNativeSegwit);
    CPPUNIT_TEST(UaiExampleDogeXpubLegacy);
    CPPUNIT_TEST(UaiExampleEthUsdc);
    CPPUNIT_TEST(UaiExampleEthRangeIndex);
    CPPUNIT_TEST(UaiExampleEthPairIndexAndWildcardIndex);
    CPPUNIT_TEST(UaiExampleSolanaUsdc);
    CPPUNIT_TEST(UaiExampleMultiversxUsdc);
    CPPUNIT_TEST(UaiExampleLtubLegacy);
    CPPUNIT_TEST(UaiExampleBtcZpubNativeSegwit);
    CPPUNIT_TEST(InvalidPubKey);
    CPPUNIT_TEST(InvalidUai);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    /* detailed-account with a hdkey account and tokenids list */
    void testDetailedAccountWithHDKey();
    /* detailed-account with an output-descriptor account and tokenids array list */
    void testDetailedAccountWithOutputDesc();
    /* invalid detailed-account with uninitialized account field */
    void testDetailedAccountWithUninitAccount();
    /* invalid detailed-account with legacy flag */
    void testDetailedAccountWithLegacyFlag();

    void UaiExampleBtcTpubNativeSegwit();
    void UaiExampleDogeXpubLegacy();
    void UaiExampleEthUsdc();
    void UaiExampleEthRangeIndex();
    void UaiExampleEthPairIndexAndWildcardIndex();
    void UaiExampleSolanaUsdc();
    void UaiExampleMultiversxUsdc();
    void UaiExampleLtubLegacy();
    void UaiExampleBtcZpubNativeSegwit();
    void InvalidPubKey();
    void InvalidUai();

private:
};
