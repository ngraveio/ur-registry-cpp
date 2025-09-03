#pragma once

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../src/portfolio_coin.h"
#include "../src/coin_identity.h"
#include "../src/detailed_account.h"
#include "../src/hdkey.h"
#include "../src/output_descriptor.h"

class TestPortfolioCoin : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestPortfolioCoin);
    CPPUNIT_TEST(EncodeWithHDKey);
    CPPUNIT_TEST(EncodeWithOutputDescriptor);
    CPPUNIT_TEST(EncodeWithMasterFingerprint);
    CPPUNIT_TEST(EncodeWithMultipleAccounts);
    CPPUNIT_TEST(EncodeSolUaiAccounts);
    CPPUNIT_TEST(EncodePolUaiAccounts);
    CPPUNIT_TEST(EncodeBtcUaiAccounts);
    CPPUNIT_TEST(EncodeBothHDKeyAndOutputDescriptorFromUai);
    CPPUNIT_TEST(AddDifferentUaiCoinIdentity);
    CPPUNIT_TEST(AddDifferentUaiMasterFingerprint);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void EncodeWithHDKey();
    void EncodeWithOutputDescriptor();
    void EncodeWithMasterFingerprint();
    void EncodeWithMultipleAccounts();
    void EncodeSolUaiAccounts();
    void EncodePolUaiAccounts();
    void EncodeBtcUaiAccounts();
    void EncodeBothHDKeyAndOutputDescriptorFromUai();
    void AddDifferentUaiCoinIdentity();
    void AddDifferentUaiMasterFingerprint();

private:
    CoinIdentity createCoinIdentity();
    DetailedAccount createHDKeyAccount();
    DetailedAccount createOutputDescriptorAccount();
};