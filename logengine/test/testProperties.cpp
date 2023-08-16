#include <fstream>
#include "testProperties.h"
#include "Properties.h"
//#include "debug_support.h"

CPPUNIT_TEST_SUITE_REGISTRATION( LogEnginePropertiesTest );

#define TEST_FILES_FOLDER "..\\..\\..\\..\\test\\testFiles\\"

void LogEnginePropertiesTest::setUp ()
{

}

void LogEnginePropertiesTest::tearDown ()
{
    // free memory allocated in setUp, do other things
}

void LogEnginePropertiesTest::testProperties1()
{
	printf("testProperties1 ... ");

	std::ifstream fin(TEST_FILES_FOLDER "test1.lfg"/*, std::ios::in*/);
	//fin.unsetf(std::ios::skipws);
	Properties props;
	props.load(fin);
	
	CPPUNIT_ASSERT_EQUAL(3, props.Count());
	CPPUNIT_ASSERT_EQUAL(4, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(1, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT(props.getString("BackupType") == "None");

	printf("PASSED\n");
}

void LogEnginePropertiesTest::testProperties2()
{
	printf("testProperties2 ... ");

	Properties props;

	CPPUNIT_ASSERT(props.trim("").empty());
	CPPUNIT_ASSERT(props.trim(" ") == "");
	CPPUNIT_ASSERT(props.trim("  ") == "");
	CPPUNIT_ASSERT(props.trim("   \n") == "\n");
	CPPUNIT_ASSERT(props.trim("\n") == "\n");
    CPPUNIT_ASSERT_EQUAL(std::string("a"), props.trim("a"));
	CPPUNIT_ASSERT_EQUAL(std::string("aa"), props.trim("aa"));
	CPPUNIT_ASSERT_EQUAL(std::string("aaa"), props.trim("aaa"));
	CPPUNIT_ASSERT_EQUAL(std::string("a"), props.trim(" a"));
	CPPUNIT_ASSERT_EQUAL(std::string("a"), props.trim("  a"));
	CPPUNIT_ASSERT(props.trim("\n  a") == "\n  a");
	CPPUNIT_ASSERT_EQUAL(std::string("a"), props.trim("a "));
	CPPUNIT_ASSERT_EQUAL(std::string("a"), props.trim("a  "));
	CPPUNIT_ASSERT(props.trim("a  \n") == "a  \n");
	CPPUNIT_ASSERT_EQUAL(std::string("a"), props.trim(" a "));
	CPPUNIT_ASSERT_EQUAL(std::string("aaa"), props.trim("  aaa   "));
	CPPUNIT_ASSERT_EQUAL(std::string("a a a"), props.trim(" a a a  "));

	printf("PASSED\n");
}

void LogEnginePropertiesTest::testProperties3()
{
	printf("testProperties3 ... ");

	std::ifstream fin(TEST_FILES_FOLDER "test2.lfg"/*, std::ios::in*/);
	
	Properties props;
	props.load(fin);
	
	CPPUNIT_ASSERT_EQUAL(3, props.Count());
	CPPUNIT_ASSERT_EQUAL(0, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(0, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT(props.getString("BackupType").empty());

	printf("PASSED\n");
}

void LogEnginePropertiesTest::testProperties4()
{
	printf("testProperties4 ... ");

	std::ifstream fin(TEST_FILES_FOLDER "test3.lfg"/*, std::ios::in*/);

    if(std::ios_base::failbit == (fin.rdstate() & std::ios_base::failbit))
        CPPUNIT_ASSERT("fail to open file " TEST_FILES_FOLDER "test3.lfg", false);
	
	Properties props;
	props.load(fin);
	
	CPPUNIT_ASSERT_EQUAL(1, props.Count());
	CPPUNIT_ASSERT_EQUAL(0, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(0, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT(props.getString("BackupType").empty());

	printf("PASSED\n");
}

void LogEnginePropertiesTest::testProperties5()
{
	printf("testProperties5 ... ");

	std::ifstream fin(TEST_FILES_FOLDER "test4.lfg"/*, std::ios::in*/);
	
	Properties props;
	props.load(fin);
	
	CPPUNIT_ASSERT_EQUAL(3, props.Count());
	CPPUNIT_ASSERT_EQUAL(4, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(1, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT_EQUAL(std::string("Single"), props.getString("BackupType"));

	printf("PASSED\n");
}

void LogEnginePropertiesTest::testProperties6()
{
	printf("testProperties6 ... ");

	std::ifstream fin(TEST_FILES_FOLDER "test5.lfg"/*, std::ios::in*/);
	
	Properties props;
	props.load(fin);
	
	CPPUNIT_ASSERT_EQUAL(3, props.Count());
	CPPUNIT_ASSERT_EQUAL(4, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(1, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT_EQUAL(std::string("Timestamp"), props.getString("BackupType"));

	printf("PASSED\n");
}

void LogEnginePropertiesTest::testProperties7()
{
	printf("testProperties7 ... ");

	std::ifstream fin(TEST_FILES_FOLDER "test6.lfg"/*, std::ios::in*/);
	
	Properties props;
	props.load(fin);
	
	CPPUNIT_ASSERT_EQUAL(3, props.Count());
	CPPUNIT_ASSERT_EQUAL(4, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(1, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT_EQUAL(std::string("None"), props.getString("BackupType"));

	printf("PASSED\n");
}

void LogEnginePropertiesTest::testProperties8()
{
	printf("testProperties8 ... ");

	std::ifstream fin(TEST_FILES_FOLDER "test7.lfg"/*, std::ios::in*/);
	
	Properties props;
	props.load(fin);
	
	CPPUNIT_ASSERT_EQUAL(3, props.Count());
	CPPUNIT_ASSERT_EQUAL(4, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(1, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT(props.getString("BackupType").empty());

	printf("PASSED\n");
}

void LogEnginePropertiesTest::testProperties9()
{
	printf("testProperties9 ... ");

	std::ifstream fin(TEST_FILES_FOLDER "test8.lfg"/*, std::ios::in*/);
	
	Properties props;
	props.load(fin);
				
	CPPUNIT_ASSERT_EQUAL(12, props.Count());
	CPPUNIT_ASSERT_EQUAL(0, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(100, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT_EQUAL(std::string("None"), props.getString("BackupType"));
	CPPUNIT_ASSERT_EQUAL(std::string("ExampleApp"), props.getString("ApplicationName"));
	CPPUNIT_ASSERT_EQUAL(std::string("1.1.1.1"), props.getString("Version"));
	CPPUNIT_ASSERT_EQUAL(std::string("ExampleApp.log"), props.getString("LogFileName"));
	std::string s = props.getString("StartAppLine");
	CPPUNIT_ASSERT_EQUAL(std::string("\n%APPNAME% %APPVERSION% startup.\nLog is started at %DATETIME%."), s);
	CPPUNIT_ASSERT_EQUAL(std::string("%APPNAME% %APPVERSION% normal shutdown.\nLog is stopped at %DATETIME%."), props.getString("StopAppLine"));
	CPPUNIT_ASSERT_EQUAL(std::string("---------------------------------------------------------------------"), props.getString("SeparatorLine"));
	s = props.getString("ErrorLine");
	CPPUNIT_ASSERT_EQUAL(std::string("%TIME% #%THREAD% : %MSG%"), s);
	CPPUNIT_ASSERT_EQUAL(std::string("%TIME% #%THREAD% : %MSG%"), props.getString("WarningLine"));
	CPPUNIT_ASSERT_EQUAL(std::string("%TIME% #%THREAD% : %MSG%"), props.getString("InfoLine"));

	printf("PASSED\n");
}

void LogEnginePropertiesTest::testProperties10()
{
	printf("testProperties10 ... ");
	
	std::ifstream fin(TEST_FILES_FOLDER "test12.lfg"/*, std::ios::in*/);
	
	Properties props;
	props.load(fin);
	
	CPPUNIT_ASSERT_EQUAL(6, props.Count());
	CPPUNIT_ASSERT_EQUAL(11, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(std::string("TimeStamp"), props.getString("BackupType"));
	CPPUNIT_ASSERT_EQUAL(101, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT_EQUAL(std::string("1.1.1.1"), props.getString("Version"));
	CPPUNIT_ASSERT_EQUAL(std::string("ExampleApp11"), props.getString("applicationname"));
	CPPUNIT_ASSERT_EQUAL(std::string("c:\\temp\\ExampleApp11.log"), props.getString("LogFileName"));
	
	printf("PASSED\n");
}


void LogEnginePropertiesTest::testProperties11()
{

    Properties props;
    props.SetValue("LogFileName", "DreamChatLog.log");

    try
    {
    std::string s = props.GetValue("BackupType");
    }
    catch(THArrayException & ex)
    {
        char* str = "THArrayException : " "THash<I,V>::GetValue(Key) : Key not found !";
        CPPUNIT_ASSERT_EQUAL(str, (char*)ex.what());        
    }
    catch(...)
    {
        CPPUNIT_ASSERT(false, "Incorrect exception is thrown");
    }
}

#undef TEST_FILES_FOLDER