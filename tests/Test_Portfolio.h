#pragma once

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../src/portfolio.h"
#include "../src/portfolio_coin.h"
#include "../src/portfolio_metadata.h"
#include "../src/coin_identity.h"
#include "../src/detailed_account.h"
#include "../src/hdkey.h"
#include "../src/output_descriptor.h"

class TestPortfolio : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestPortfolio);
    CPPUNIT_TEST(EncodeWithCoinsOnly);
    CPPUNIT_TEST(EncodeWithCoinsAndMetadata);
    CPPUNIT_TEST(EncodeWithMultipleCoins);
    CPPUNIT_TEST(EncodeEmpty);
    CPPUNIT_TEST(EncodeUaiToPortfolio);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void EncodeWithCoinsOnly();
    void EncodeWithCoinsAndMetadata();
    void EncodeWithMultipleCoins();
    void EncodeEmpty();
    void EncodeUaiToPortfolio();

private:
    PortfolioCoin createBitcoinCoin();
    PortfolioCoin createEthereumCoin();
    PortfolioMetadata createMetadata();
    std::vector<uint8_t> createSyncId();
};