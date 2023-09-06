#ifndef _THREAD_LOG_TESTS_HEADER_
#define _THREAD_LOG_TESTS_HEADER_

#include <cppunit/extensions/HelperMacros.h>

/* 
 * A test case to test LogEngine ThreadLog class
 *
 */

class LogEngineThreadLogTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( LogEngineThreadLogTest );
  CPPUNIT_TEST( testCallLogFromManyThreads );
  CPPUNIT_TEST( testThreadLog1 ); 
  CPPUNIT_TEST(testThreadLogMeasureTime1);
  CPPUNIT_TEST(testNONThreadLogMeasureTime1);
  CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
    void tearDown();

    void testCallLogFromManyThreads();
    void testThreadLog1();
    void testThreadLogMeasureTime1();
    void testNONThreadLogMeasureTime1();
};

#endif
