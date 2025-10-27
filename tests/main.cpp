#include <iostream>
#include <string>
#include <list>
#include <csignal>
#include <execinfo.h>
#include <unistd.h>

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>

#include "Test_EthSignRequest.h"
#include "Test_HDKey.h"
#include "Test_Keypath.h"
#include "Test_CoinInfo.h"
#include "Test_OutputDescriptor.h"
#include "Test_DetailedAccount.h"
#include "Test_PortfolioMetadata.h"
#include "Test_PortfolioCoin.h"
#include "Test_Portfolio.h"
#include "Test_UrSignatures.h"
#include "Test_CoinIdentity.h"
#include "Test_SignRequest.h"
#include "Test_BatchSignRequest.h"
#include "Test_Intent.h"

using namespace CppUnit;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(TestDetailedAccount);
CPPUNIT_TEST_SUITE_REGISTRATION(TestOutputDescriptor);
CPPUNIT_TEST_SUITE_REGISTRATION(TestCoinInfo);
CPPUNIT_TEST_SUITE_REGISTRATION(TestKeypath);
CPPUNIT_TEST_SUITE_REGISTRATION(TestHDKey);
CPPUNIT_TEST_SUITE_REGISTRATION(TestPortfolioMetadata);
CPPUNIT_TEST_SUITE_REGISTRATION(TestPortfolioCoin);
CPPUNIT_TEST_SUITE_REGISTRATION(TestPortfolio);
CPPUNIT_TEST_SUITE_REGISTRATION(TestUrSignatures);
CPPUNIT_TEST_SUITE_REGISTRATION(TestCoinIdentity);
CPPUNIT_TEST_SUITE_REGISTRATION(TestSignRequest);
CPPUNIT_TEST_SUITE_REGISTRATION(TestBatchSignRequest);
CPPUNIT_TEST_SUITE_REGISTRATION(TestIntent);

int main(int argc, char *argv[])
{
    // informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener(&collectedresults);

    // register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener(&progress);

    // insert test-suite at test-runner by registry
    CPPUNIT_NS::TestRunner testrunner;

    testrunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    testrunner.run(testresult);

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1;
}
