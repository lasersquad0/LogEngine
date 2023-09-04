
#ifndef WIN32
#include <unistd.h>
#endif

#include "Shared.h"
#include "LogEngine.h"
#include "testLog.h"
#include <cppunit/portability/Stream.h>
//#include "debug_support.h"

CPPUNIT_TEST_SUITE_REGISTRATION( LogEngineLogTest );


void LogEngineLogTest::setUp ()
{

}

void LogEngineLogTest::tearDown ()
{
    // free memory allocated in setUp, do other things
}


void LogEngineLogTest::testLog1()
{
	//printf("mytestLog1 ... ");

	// preparing parameters
	Properties prop;
	prop.SetValue("BackupType", "None");
	prop.SetValue("DetailLevel", "DefaultDetailLevel"); 
	prop.SetValue("FileName", "testLog1.log");
	prop.SetValue("MaxLogSize", "100");
	prop.SetValue("ApplicationName", "LogEngine tests");
	prop.SetValue("Version", "1.1.1");

	InitLogEngine(prop);
	TLogEngine *log = getLogEngine();

	std::string s;
	
	s = log->FormatError("testLog1error");
    CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string("#testLog1error"), cutLog(s));

	s = log->FormatInfo("testLog1info");
	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string(" testLog1info"), cutLog(s));

	s = log->FormatWarning("testLog1warning");
	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string("!testLog1warning"), cutLog(s));

	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string("testLog1string"), log->FormatStr("testLog1string"));

	CloseLogEngine();

	//printf("myPASSED\n");
}
	
void LogEngineLogTest::testLog2()
{
	//printf("mytestLog2 ... ");

	Properties prop;
	
	InitLogEngine(prop);
	TLogEngine *log = getLogEngine();
	
	std::string s;
	
	s = log->FormatError("testLog2error");
	CPPUNIT_ASSERT_EQUAL(std::string("#testLog2error"), cutLog(s));

	s = log->FormatInfo("testLog2info");
	CPPUNIT_ASSERT_EQUAL(std::string(" testLog2info"), cutLog(s));

	s = log->FormatWarning("testLog2warning");
	CPPUNIT_ASSERT_EQUAL(std::string("!testLog2warning"), cutLog(s));

	CPPUNIT_ASSERT_EQUAL(std::string("testLog2string"), log->FormatStr("testLog2string"));
	
	CloseLogEngine();

	//printf("myPASSED\n");
}


void LogEngineLogTest::testLog3()
{
	//printf("testLog3 ... ");

	Properties prop;

	InitLogEngine(prop);
	TLogEngine *log = getLogEngine();
	
	log->Stop();

	try
	{
		log->WriteError("testLog3 ddddddddddddddddddddddddddddddddddddd");
		CPPUNIT_ASSERT_MESSAGE(std::string("write on stopped log"), false);
	}
	catch(LogException&/* ex*/)
	{
	}

	CloseLogEngine();

	//printf("PASSED\n");
}


void LogEngineLogTest::testLog4()
{
	//printf("testLog4 ... ");

	InitLogEngine();
		
	CPPUNIT_ASSERT_EQUAL(std::string("#testLog4 errorrrrrrrrrrrrrrrrrr"), cutLog(getLogEngine()->FormatError("testLog4 errorrrrrrrrrrrrrrrrrr")));
	CPPUNIT_ASSERT_EQUAL(std::string(" testLog4 infoooooooooooooo"), cutLog(getLogEngine()->FormatInfo("testLog4 infoooooooooooooo")));
	CPPUNIT_ASSERT_EQUAL(std::string("!testLog4 warningggggggggggggggggg"), cutLog(getLogEngine()->FormatWarning("testLog4 warningggggggggggggggggg")));
	CPPUNIT_ASSERT_EQUAL(std::string("testLog4 stringsssssssssssssssss"), getLogEngine()->FormatStr("testLog4 stringsssssssssssssssss"));
	
	getLogEngine()->Stop();

	CloseLogEngine();
	//printf("PASSED\n");
}

void LogEngineLogTest::testLog5()
{
	//printf("testLog5 ... ");

	InitLogEngine();
	TLogEngine* log = getLogEngine();

	log->WriteErrorFmt  (0, "testLog5 errorrrrrrrrrrrrrrrrrr %7d", 1);
	log->WriteInfoFmt   (0, "testLog5 infoooooooooooooo %7d", 2);
	log->WriteWarningFmt(0, "testLog5 warningggggggggggggggggg %7d", 3);
	log->WriteStrFmt    (0, "testLog5 stringsssssssssssssssss %7d", 4);

	CloseLogEngine();

	//printf("PASSED\n");
}

void LogEngineLogTest::testLog6()
{
	//printf("testLog6 ... ");

	InitLogEngine();
	TLogEngine* log = getLogEngine();

	log->WriteErrorFmt  (0, "testLog6 11111111111111111 %s", "222222");
	log->WriteInfoFmt   (0, "testLog6 22222222222222222 %s", "333333");
	log->WriteWarningFmt(0, "testLog6 33333333333333333 %s", "444444");
	log->WriteStrFmt    (0, "testLog6 44444444444444444 %s", "555555");

	CloseLogEngine();

	//printf("PASSED\n");
}

void LogEngineLogTest::testLogStartStop()
{
	//printf("testLogStartStop ... ");

	InitLogEngine();
	TLogEngine* log = getLogEngine();

	CPPUNIT_ASSERT(log->IsStarted());
	log->Start();
	CPPUNIT_ASSERT(log->IsStarted());
	log->Start();
	CPPUNIT_ASSERT(log->IsStarted());
	log->Stop();
	CPPUNIT_ASSERT(!log->IsStarted());
	log->Stop();
	CPPUNIT_ASSERT(!log->IsStarted());

	log->Start();
	log->Start();
	log->Stop();
	log->Stop();

	try
	{
		getLogEngine()->WriteError("write after Stop()!");
		CPPUNIT_ASSERT_MESSAGE("write after Stop()!", false);
	}
	catch(LogException& /*ex*/)
	{
	}

	CloseLogEngine();

	//printf("PASSED\n");
}

void LogEngineLogTest::testLogInitClose()
{
	//printf("testLogInitClose ... ");

	Properties prop;
	prop.SetValue("FileName", "testLogInitClose.log");
	
	InitLogEngine();
	InitLogEngine(prop);
	InitLogEngine();
	
	TLogEngine* log1 = getLogEngine();
	TLogEngine* log2 = getLogEngine();
	
	CPPUNIT_ASSERT(log1 == log2);
	
	CPPUNIT_ASSERT(log1->IsStarted());
	CPPUNIT_ASSERT(log2->IsStarted());
	
	log1->Stop();
	CPPUNIT_ASSERT(!log1->IsStarted());
	CPPUNIT_ASSERT(!log2->IsStarted());
	
	log2->Start();
	CPPUNIT_ASSERT(log1->IsStarted());
	CPPUNIT_ASSERT(log2->IsStarted());
	
	CloseLogEngine();
	CloseLogEngine();
	CloseLogEngine();
	
	
	//printf("PASSED\n");
}


void LogEngineLogTest::testLogPlaceholders()
{
//	printf("testLogPlaceholders ... ");
	
	// preparing parameters
	Properties prop;
	prop.SetValue("ApplicationName", "LogEngine_tests");
	prop.SetValue("Version", "1.1.1");
	prop.SetValue("ErrorLine", "%MSG% %APPNAME% %APPVERSION% %OS% %OSVERSION% %DETAILLEVEL%");
	prop.SetValue("WarningLine", "%DETAILLEVEL% %OS% %OSVERSION% %APPNAME% %APPVERSION% %MSG%");
	prop.SetValue("InfoLine", "%OS% %MSG% %APPNAME% %OSVERSION% %DETAILLEVEL% %APPVERSION%");
	
	InitLogEngine(prop);
	TLogEngine *log = getLogEngine();
	
	std::string s = log->FormatError("aaa", 6);
	CPPUNIT_ASSERT_EQUAL(std::string("#aaa LogEngine_tests 1.1.1 <OS> 5.1 build 2600 6"), log->FormatError("aaa", 6));
	CPPUNIT_ASSERT_EQUAL(std::string("!6 <OS> 5.1 build 2600 LogEngine_tests 1.1.1 aaa"), log->FormatWarning("aaa", 6));
	CPPUNIT_ASSERT_EQUAL(std::string(" <OS> aaa LogEngine_tests 5.1 build 2600 6 1.1.1"), log->FormatInfo("aaa", 6));
	CPPUNIT_ASSERT_EQUAL(std::string("aaa"), log->FormatStr("aaa", 6));
	
	//printf("PASSED\n");
}

void LogEngineLogTest::testLogDetailLevel()
{
	//printf("testLogDetailLevel ... ");
	
	InitLogEngine(TEST_FILES_FOLDER "test9.lfg");
	TLogEngine* log = getLogEngine();
	CPPUNIT_ASSERT_EQUAL(0u, log->GetLogDetailLevel());
	
	InitLogEngine(TEST_FILES_FOLDER "test10.lfg");
	log = getLogEngine();
	CPPUNIT_ASSERT_EQUAL(4u, log->GetLogDetailLevel());
	
	InitLogEngine(TEST_FILES_FOLDER "test11.lfg");
	log = getLogEngine();
	CPPUNIT_ASSERT_EQUAL(7u, log->GetLogDetailLevel());
	
	//printf("PASSED\n");
}

void LogEngineLogTest::testLogBackupTypeNone()
{
	//printf("testLogBackupTypeNone1 ...  ");
	
	const char* fileName = "aaa.log";

	// preparing parameters
	Properties prop;
	prop.SetValue("ApplicationName", "LogEngine_tests");
	prop.SetValue("Version", "1.1.1");
	prop.SetValue("backuptype", "None");
	prop.SetValue("maxlogsize", "1");
	prop.SetValue("logfilename", fileName);

	//unlink
	remove(fileName);

	InitLogEngine(prop);
	TLogEngine* log = getLogEngine();

	CPPUNIT_ASSERT_EQUAL(lbNone, log->GetBackupType());
	
	while(true)
	{
		log->WriteError("1234567890");
		if(log->GetBytesWritten() > 1024)
			break;
	}
	
	log->Flush();
	
	struct stat st;
	stat(fileName, &st);
	CPPUNIT_ASSERT_EQUAL((unsigned int)st.st_size, log->GetBytesWritten());
	
	CloseLogEngine();
	
	//printf("PASSED\n");
}

void LogEngineLogTest::testLogBackupTypeSingle()
{
	// preparing parameters
	Properties prop;
	prop.SetValue("ApplicationName", "LogEngine_tests");
	prop.SetValue("Version", "1.1.1");
	prop.SetValue("backuptype", "single");
	prop.SetValue("maxlogsize", "1");
	prop.SetValue("logfilename", "aaa.log");
	
	InitLogEngine(prop);
	TLogEngine* log = getLogEngine();
	
	CPPUNIT_ASSERT_EQUAL(lbSingle, log->GetBackupType());
	
	while(true)
	{
		log->WriteError("1234567890");
		if(log->GetBytesWritten() > 1024)
			break;
	}
	
	/*struct _stat st;
	_stat("aaa.log", &st);
	CPPUNIT_ASSERT_EQUAL(st.st_size, log->GetBytesWritten());
	*/
	
	CloseLogEngine();
	
	//printf("PASSED\n");
}

void LogEngineLogTest::testLogFullPath()
{
	//printf("testLogFullPath ... ");

	InitLogEngine(TEST_FILES_FOLDER "test12.lfg");
	TLogEngine* log = getLogEngine();

	CPPUNIT_ASSERT_EQUAL(11u, log->GetLogDetailLevel());
	CPPUNIT_ASSERT_EQUAL(lbTimeStamp, log->GetBackupType());
	CPPUNIT_ASSERT_EQUAL(101u, log->GetMaxLogSize());
	CPPUNIT_ASSERT_EQUAL(std::string("1.1.1.1"), log->GetVersionInfo());
	CPPUNIT_ASSERT_EQUAL(std::string("ExampleApp11"), log->GetAppName());
	CPPUNIT_ASSERT_EQUAL(std::string("c:\\temp\\ExampleApp11.log"), log->GetLogFileName());	

	CloseLogEngine();
	
	//printf("PASSED\n");
}

void LogEngineLogTest::testLogMacro()
{
	//printf("testLogMacro ... ");

	InitLogEngine();

	LOG_ERROR("dfdfdf");
	LOG_ERROR("dfdfdf");

	CloseLogEngine();

	//printf("PASSED\n");
}

void LogEngineLogTest::testLogEmptyFileName()
{
	//printf("testLogEmptyFileName ... ");

	// preparing parameters
	Properties prop;
	prop.SetValue("BackupType", "None");
	prop.SetValue("DetailLevel", "DefaultDetailLevel"); 
	prop.SetValue("MaxLogSize", "100");
	prop.SetValue("ApplicationName", "LogEngine tests");
	prop.SetValue("Version", "1.1.1");

	InitLogEngine(prop);

	TLogEngine *log = getLogEngine();

	std::string s = log->GetLogFileName();
	CPPUNIT_ASSERT_EQUAL(std::string("LogEngine tests.log"), s);

	CloseLogEngine();

	//printf("PASSED\n");
}

void LogEngineLogTest::testLogAppName()
{
	//printf("testLogAppName ... ");

	// preparing parameters
	Properties prop;
	prop.SetValue("BackupType", "None");
	prop.SetValue("ApplicationName", "AAAAAAAA");
	prop.SetValue("Version", "1.0.555");
	prop.SetValue("DetailLevel", "0");

	InitLogEngine(prop);

	TLogEngine *log = getLogEngine();

	std::string s = log->GetAppName();
	CPPUNIT_ASSERT_EQUAL(std::string("AAAAAAAA"), s);

	log->Stop();
	log->Start();
	CloseLogEngine();

	//printf("PASSED\n");
}

void LogEngineLogTest::testWrong_LFG_File()
{
    try
    {
        InitLogEngine(TEST_FILES_FOLDER "test000.lfg");
    }
    catch (LogException& ex) 
    {
        CPPUNIT_ASSERT_EQUAL((char*)("[LogException] Cannot open config file: " TEST_FILES_FOLDER "test000.lfg"), (char*)ex.what());
    }
/* 


    InitLogEngine(TEST_FILES_FOLDER "test10.lfg");
    log = getLogEngine();
    CPPUNIT_ASSERT_EQUAL(4, log->GetLogDetailLevel());

    InitLogEngine(TEST_FILES_FOLDER "test11.lfg");
    log = getLogEngine();
    CPPUNIT_ASSERT_EQUAL(7, log->GetLogDetailLevel());

    printf("PASSED\n");*/
}

void LogEngineLogTest::testBadLFGFile()
{
	CPPUNIT_ASSERT_THROW( InitLogEngine(TEST_FILES_FOLDER "test13.lfg"), IOException );
	TLogEngine* log = getLogEngine();

	CloseLogEngine();
}

void LogEngineLogTest::testBadLFGFile2()
{
	InitLogEngine(TEST_FILES_FOLDER "test14.lfg");
	TLogEngine* log = getLogEngine();

	CPPUNIT_ASSERT_EQUAL(0u, log->GetLogDetailLevel());
	CPPUNIT_ASSERT_EQUAL(lbNone, log->GetBackupType());
	CPPUNIT_ASSERT_EQUAL(1000u, log->GetMaxLogSize());
	CPPUNIT_ASSERT_EQUAL(std::string("t.t.t.t"), log->GetVersionInfo());
	CPPUNIT_ASSERT_EQUAL(std::string("BadLFGFileApp"), log->GetAppName());
	CPPUNIT_ASSERT_EQUAL(std::string("BadLFGLog.log"), log->GetLogFileName());

	CloseLogEngine();
}


#undef TEST_FILES_FOLDER