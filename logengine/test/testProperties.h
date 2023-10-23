#if !defined(_TEST_PROPERTIES_)
#define _TEST_PROPERTIES_

#include <cppunit/extensions/HelperMacros.h>

/* 
 * A test case to test LogEngine Properties class
 *
 */

class LogEnginePropertiesTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( LogEnginePropertiesTest );

  CPPUNIT_TEST( testProperties1 );
  CPPUNIT_TEST( testProperties2 );
  CPPUNIT_TEST( testProperties3 );
  CPPUNIT_TEST( testProperties4 );
  CPPUNIT_TEST( testProperties5 );
  CPPUNIT_TEST( testProperties6 );
  CPPUNIT_TEST( testProperties7 );  
  CPPUNIT_TEST( testProperties8 );
  CPPUNIT_TEST( testProperties9 );
  CPPUNIT_TEST( testProperties10 );
  CPPUNIT_TEST( testProperties11 );
  CPPUNIT_TEST( testProperties12 );

  CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
    void tearDown();

    void testProperties1();
    void testProperties2();
    void testProperties3();
    void testProperties4();
    void testProperties5();
    void testProperties6();
    void testProperties7();
    void testProperties8();
    void testProperties9();
    void testProperties10();
    void testProperties11();
    void testProperties12();
};

#endif //_TEST_PROPERTIES_
