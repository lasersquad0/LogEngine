
#include "Shared.h"
#include "testProperties.h"
#include "Properties.h"
#include "LogEngine.h"

CPPUNIT_TEST_SUITE_REGISTRATION( LogEnginePropertiesTest );

void LogEnginePropertiesTest::setUp ()
{

}

void LogEnginePropertiesTest::tearDown ()
{
    // free memory allocated in setUp, do other things
}

void LogEnginePropertiesTest::testProperties1()
{
	std::ifstream fin(TEST_FILES_FOLDER "test1.lfg"/*, std::ios::in*/);
	CPPUNIT_ASSERT_MESSAGE("Cannot open file" TEST_FILES_FOLDER "test1.lfg", !fin.fail());
	//fin.unsetf(std::ios::skipws);
	Properties props;
	props.load(fin);
	
	CPPUNIT_ASSERT_EQUAL(3u, props.Count());
	CPPUNIT_ASSERT_EQUAL(4, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(1, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT(props.getString("BackupType") == "None");
}

void LogEnginePropertiesTest::testProperties2()
{
	Properties props;

	std::string s = "  123 ";
	std::string ss = s;
	std::string sss = props.trim(s);

	CPPUNIT_ASSERT_EQUAL(s, ss); // check that trim does not change string s
	CPPUNIT_ASSERT_EQUAL(sss, std::string("123"));

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
}

void LogEnginePropertiesTest::testProperties3()
{
	std::ifstream fin(TEST_FILES_FOLDER "test2.lfg"/*, std::ios::in*/);
	CPPUNIT_ASSERT_MESSAGE("Cannot open file" TEST_FILES_FOLDER "test2.lfg", !fin.fail());

	Properties props;
	props.load(fin);
	
	CPPUNIT_ASSERT_EQUAL(3u, props.Count());
	CPPUNIT_ASSERT_EQUAL(0, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(0, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT(props.getString("BackupType").empty());
}

void LogEnginePropertiesTest::testProperties4()
{
	std::ifstream fin(TEST_FILES_FOLDER "test3.lfg"/*, std::ios::in*/);

    //if(std::ios_base::failbit == (fin.rdstate() & std::ios_base::failbit))
    CPPUNIT_ASSERT_MESSAGE("Cannot open file " TEST_FILES_FOLDER "test3.lfg", !fin.fail());
	
	Properties props;
	props.load(fin);
	
	CPPUNIT_ASSERT_EQUAL(1u, props.Count());
	CPPUNIT_ASSERT_EQUAL(0, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(0, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT(props.getString("BackupType").empty());
}

void LogEnginePropertiesTest::testProperties5()
{
	std::ifstream fin(TEST_FILES_FOLDER "test4.lfg"/*, std::ios::in*/);
	CPPUNIT_ASSERT_MESSAGE("Cannot open file" TEST_FILES_FOLDER "test4.lfg", !fin.fail());

	Properties props;
	props.load(fin);
	
	CPPUNIT_ASSERT_EQUAL(3u, props.Count());
	CPPUNIT_ASSERT_EQUAL(4, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(1, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT_EQUAL(std::string("Single"), props.getString("BackupType"));
}

void LogEnginePropertiesTest::testProperties6()
{
	std::ifstream fin(TEST_FILES_FOLDER "test5.lfg"/*, std::ios::in*/);
	CPPUNIT_ASSERT_MESSAGE("Cannot open file" TEST_FILES_FOLDER "test5.lfg", !fin.fail());

	Properties props;
	props.load(fin);
	
	CPPUNIT_ASSERT_EQUAL(3u, props.Count());
	CPPUNIT_ASSERT_EQUAL(4, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(1, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT_EQUAL(std::string("Timestamp"), props.getString("BackupType"));
}

void LogEnginePropertiesTest::testProperties7()
{
	std::ifstream fin(TEST_FILES_FOLDER "test6.lfg"/*, std::ios::in*/);
	CPPUNIT_ASSERT_MESSAGE("Cannot open file" TEST_FILES_FOLDER "test6.lfg", !fin.fail());

	Properties props;
	props.load(fin);
	
	CPPUNIT_ASSERT_EQUAL(3u, props.Count());
	CPPUNIT_ASSERT_EQUAL(4, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(1, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT_EQUAL(std::string("None"), props.getString("BackupType"));
}

void LogEnginePropertiesTest::testProperties8()
{
	std::ifstream fin(TEST_FILES_FOLDER "test7.lfg"/*, std::ios::in*/);
	CPPUNIT_ASSERT_MESSAGE("Cannot open file" TEST_FILES_FOLDER "test7.lfg", !fin.fail());

	Properties props;
	props.load(fin);
	
	CPPUNIT_ASSERT_EQUAL(3u, props.Count());
	CPPUNIT_ASSERT_EQUAL(4, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(1, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT(props.getString("BackupType").empty());
}

void LogEnginePropertiesTest::testProperties9()
{
	std::ifstream fin(TEST_FILES_FOLDER "test8.lfg"/*, std::ios::in*/);
	CPPUNIT_ASSERT_MESSAGE("Cannot open file" TEST_FILES_FOLDER "test8.lfg", !fin.fail());

	Properties props;
	props.load(fin);
				
	CPPUNIT_ASSERT_EQUAL(12u, props.Count());
	CPPUNIT_ASSERT_EQUAL(0, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(100, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT_EQUAL(std::string("None"), props.getString("BackupType"));
	CPPUNIT_ASSERT_EQUAL(std::string("ExampleApp"), props.getString("ApplicationName"));
	CPPUNIT_ASSERT_EQUAL(std::string("E.E.E.E"), props.getString("Version"));
	CPPUNIT_ASSERT_EQUAL(std::string("logs/ExampleApp.log"), props.getString("LogFileName"));
	std::string s = props.getString("StartAppLine");
	CPPUNIT_ASSERT_EQUAL(std::string("\n%APPNAME% %APPVERSION% startup.\nLog is started at %DATETIME%."), s);
	CPPUNIT_ASSERT_EQUAL(std::string("%APPNAME% %APPVERSION% normal shutdown.\nLog is stopped at %DATETIME%."), props.getString("StopAppLine"));
	CPPUNIT_ASSERT_EQUAL(std::string("---------------------------------------------------------------------"), props.getString("SeparatorLine"));
	s = props.getString("ErrorLine");
	CPPUNIT_ASSERT_EQUAL(std::string("%TIME% #%THREAD% : %MSG%"), s);
	CPPUNIT_ASSERT_EQUAL(std::string("%TIME% #%THREAD% : %MSG%"), props.getString("WarningLine"));
	CPPUNIT_ASSERT_EQUAL(std::string("%TIME% #%THREAD% : %MSG%"), props.getString("InfoLine"));
}

void LogEnginePropertiesTest::testProperties10()
{
	std::ifstream fin(TEST_FILES_FOLDER "test12.lfg"/*, std::ios::in*/);
	CPPUNIT_ASSERT_MESSAGE("Cannot open file" TEST_FILES_FOLDER "test12.lfg", !fin.fail());

	Properties props;
	props.load(fin);
	
	CPPUNIT_ASSERT_EQUAL(6u, props.Count());
	CPPUNIT_ASSERT_EQUAL(11, props.getInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(std::string("TimeStamp"), props.getString("BackupType"));
	CPPUNIT_ASSERT_EQUAL(101, props.getInt("MaxLogSize"));
	CPPUNIT_ASSERT_EQUAL(std::string("1.1.1.1"), props.getString("Version"));
	CPPUNIT_ASSERT_EQUAL(std::string("ExampleApp11"), props.getString("applicationname"));
	CPPUNIT_ASSERT_EQUAL(std::string("c:\\temp\\ExampleApp11.log"), props.getString("LogFileName"));
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
        const char* str = "THArrayException : THash<I,V>::GetValue(Key) : Key not found !";
        CPPUNIT_ASSERT_EQUAL(str, ex.what());       
    }
    catch(...)
    {
        CPPUNIT_ASSERT_MESSAGE("Incorrect exception is thrown", false);
    }
}

void LogEnginePropertiesTest::testProperties12()
{
	std::ifstream fin(TEST_FILES_FOLDER "test13.lfg");
	CPPUNIT_ASSERT_MESSAGE("Cannot open file" TEST_FILES_FOLDER "test13.lfg", !fin.fail());

	Properties props;
	props.load(fin);

	CPPUNIT_ASSERT_EQUAL(12u, props.Count());
	CPPUNIT_ASSERT_EQUAL(0ul, props.getUInt("DetailLevel", DefaultDetailLevel)); // in file it is '-1'
	CPPUNIT_ASSERT_EQUAL(1000ul, props.getUInt("MaxLogSize", DefaultMaxLogSize)); // should be default value 100 while in file it is non digit value
	CPPUNIT_ASSERT_EQUAL(std::string("Nones"), props.getString("BackupType")); // in file it is errorenos value
	CPPUNIT_ASSERT_EQUAL(std::string("Exam\npleApp"), props.getString("ApplicationName"));
	CPPUNIT_ASSERT_EQUAL(std::string("t.t.t.t"), props.getString("Version"));
	CPPUNIT_ASSERT_EQUAL(std::string("logs/Error\nLog.log"), props.getString("LogFileName"));

	std::string s = props.getString("StartAppLine");
#ifdef WIN32
	CPPUNIT_ASSERT_EQUAL(std::string("%APPNAME% C:\\Users\\Andrey\\AppData\\Local\\Temp %APPVERSION% CMDB-182726 startup.\nLog is started at %DATETIME%."), s);
#else
	CPPUNIT_ASSERT_EQUAL(std::string("%APPNAME% /bin/bash %APPVERSION% CMDB-182726 startup.\nLog is started at %DATETIME%."), s);
#endif
	
	CPPUNIT_ASSERT_EQUAL(std::string("%APPNAME% %APPVERSION% normal shutdown.\nLog is stopped at %DATETIME%."), props.getString("StopAppLine"));
	CPPUNIT_ASSERT_EQUAL(std::string("---------------------------------------------------------------------"), props.getString("SeparatorLine"));
	s = props.getString("ErrorLine");
	CPPUNIT_ASSERT_EQUAL(std::string("%TIME% #%THREAD% : %MSG%"), s);
	CPPUNIT_ASSERT_EQUAL(std::string("%TIME% #%THREAD% : %MSG%"), props.getString("WarningLine"));
	CPPUNIT_ASSERT_EQUAL(std::string("%TIME% #%THREAD% : %MESSAGE%"), props.getString("InfoLine"));
}

#undef TEST_FILES_FOLDER