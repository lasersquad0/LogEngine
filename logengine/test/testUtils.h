#ifndef _UTIL_TESTS_HEADER_
#define _UTIL_TESTS_HEADER_

#include <cppunit/extensions/HelperMacros.h>

/* 
 * A test case to test LogEngine utils 
 *
 */

class LogEngineUtilsTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( LogEngineUtilsTest );
  CPPUNIT_TEST( testStringReplace1 );
  CPPUNIT_TEST( testStringReplace2 );  
  CPPUNIT_TEST( testStringReplace3 );  
  CPPUNIT_TEST( testStringReplace4 );  
  CPPUNIT_TEST( testStringReplace5 );  
  CPPUNIT_TEST( testStringReplace6 );
  CPPUNIT_TEST( testStringReplace7 );
  CPPUNIT_TEST( testExtractFileName1 );  
  CPPUNIT_TEST( testExtractFileName2 );  
  CPPUNIT_TEST( testExtractFileName3 );  
  CPPUNIT_TEST( testExtractFileName4 );  
  CPPUNIT_TEST( testIntToStr1 );  
  CPPUNIT_TEST( testIntToStr2 );  
  //CPPUNIT_TEST( testIntToStr3 );  

  CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
    void tearDown();

    void testStringReplace1();
    void testStringReplace2();
    void testStringReplace3();
    void testStringReplace4();
    void testStringReplace5();
    void testStringReplace6();
    void testStringReplace7();
    void testExtractFileName1();
    void testExtractFileName2();
    void testExtractFileName3();
    void testExtractFileName4();
 
    void testIntToStr1();
    void testIntToStr2();
//    void testIntToStr3();

};

#endif // _UTIL_TESTS_HEADER_
