#ifndef _LOG_TESTS_HEADER_
#define _LOG_TESTS_HEADER_

#include <string>
#include <cppunit/extensions/HelperMacros.h>

/* 
 * A test case to test LogEngine Log class
 *
 */

class LogEngineLogTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( LogEngineLogTest );
  CPPUNIT_TEST( testLog1 );
  CPPUNIT_TEST( testLog2 );
  CPPUNIT_TEST( testLog3 );
  CPPUNIT_TEST( testLog4 );
  CPPUNIT_TEST( testLog5 );
  CPPUNIT_TEST( testLog6 );
  CPPUNIT_TEST( testLogStartStop );  
  CPPUNIT_TEST( testLogInitClose );
  CPPUNIT_TEST( testLogPlaceholders );
  CPPUNIT_TEST( testLogDebugLevel );
  CPPUNIT_TEST( testLogBackupTypeNone );
  CPPUNIT_TEST( testLogBackupTypeSingle );
  CPPUNIT_TEST( testLogFullPath );
  CPPUNIT_TEST( testLogMacro );
  CPPUNIT_TEST( testLogEmptyFileName );
  CPPUNIT_TEST( testLogAppName );  
  CPPUNIT_TEST( testWrong_LFG_File );
  CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
    void tearDown();

    void testLog1();
    void testLog2();
    void testLog3();
    void testLog4();
    void testLog5();
    void testLog6();
    void testLogStartStop();
    void testLogInitClose();
    void testLogPlaceholders();
    void testLogDebugLevel();
    void testLogBackupTypeNone();
    void testLogBackupTypeSingle();
    void testLogFullPath();
    void testLogMacro();
    void testLogEmptyFileName();
    void testLogAppName();
    void testWrong_LFG_File();

};

inline std::string cutLog(std::string str)
{
    //return str.replace(1, 17, "").c_str(); // cut off time and thread information (which is not same time from time)
    size_t pos = str.find_first_of('#', 1);
    pos = str.find_first_of(':', pos +  1);
    return str.erase((size_t)1, pos+1).c_str();
}

#endif // _LOG_TESTS_HEADER_
