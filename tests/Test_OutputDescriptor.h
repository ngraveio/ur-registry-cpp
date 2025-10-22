#pragma once

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class TestOutputDescriptor : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestOutputDescriptor);
    CPPUNIT_TEST(testValidOutDescWithPlaceholders);
    CPPUNIT_TEST(testValidTextualOutputDesc);
    CPPUNIT_TEST(testInvalidOutputDescriptors);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);

protected:
    void testValidOutDescWithPlaceholders(void);
    void testValidTextualOutputDesc(void);
    void testInvalidOutputDescriptors(void);

private:
};
