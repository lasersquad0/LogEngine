
#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif 

#include <thread>

#include "Shared.h"
#include "LogEngine.h"
#include "testLog.h"

CPPUNIT_TEST_SUITE_REGISTRATION( LogEngineLogTest );

void LogEngineLogTest::setUp ()
{
	mkdir(LOG_FILES_FOLDER);
}

void LogEngineLogTest::tearDown ()
{
	// free memory allocated in setUp, do other things
}

void LogEngineLogTest::testLog1()
{
	// preparing parameters
	Properties prop;
	prop.SetValue("BackupType", "None");
	prop.SetValue("DetailLevel", "DefaultDetailLevel"); 
	prop.SetValue("LogFileName", LOG_FILES_FOLDER "testLog1.log");
	prop.SetValue("MaxLogSize", "100");
	prop.SetValue("ApplicationName", "LogEngine tests");
	prop.SetValue("Version", "1.1.1");

	InitLogEngine(prop);
	TLogEngine *log = TLogEngine::getInstance();

	std::string s;
	
	s = log->FormatError("testLog1error");
	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string("!testLog1error"), cutLog(s));

	s = log->FormatInfo("testLog1info");
	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string(" testLog1info"), cutLog(s));

	s = log->FormatWarning("testLog1warning");
	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string("#testLog1warning"), cutLog(s));

	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string("testLog1string"), log->FormatStr("testLog1string"));

	CloseLogEngine();
}
	
void LogEngineLogTest::testLog2()
{
	Properties prop;
	
	InitLogEngine(prop);
	TLogEngine *log = TLogEngine::getInstance();
	
	std::string s;
	
	s = log->FormatError("testLog2error");
	CPPUNIT_ASSERT_EQUAL(std::string("!testLog2error"), cutLog(s));

	s = log->FormatInfo("testLog2info");
	CPPUNIT_ASSERT_EQUAL(std::string(" testLog2info"), cutLog(s));

	s = log->FormatWarning("testLog2warning");
	CPPUNIT_ASSERT_EQUAL(std::string("#testLog2warning"), cutLog(s));

	CPPUNIT_ASSERT_EQUAL(std::string("testLog2string"), log->FormatStr("testLog2string"));
	
	CloseLogEngine();
}

void LogEngineLogTest::testLog3()
{
	Properties prop;

	InitLogEngine(prop);
	TLogEngine *log = TLogEngine::getInstance();
	
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
}


void LogEngineLogTest::testLog4()
{
	InitLogEngine();
		
	CPPUNIT_ASSERT_EQUAL(std::string("!testLog4 errorrrrrrrrrrrrrrrrrr"), cutLog(TLogEngine::getInstance()->FormatError("testLog4 errorrrrrrrrrrrrrrrrrr")));
	CPPUNIT_ASSERT_EQUAL(std::string(" testLog4 infoooooooooooooo"), cutLog(TLogEngine::getInstance()->FormatInfo("testLog4 infoooooooooooooo")));
	CPPUNIT_ASSERT_EQUAL(std::string("#testLog4 warningggggggggggggggggg"), cutLog(TLogEngine::getInstance()->FormatWarning("testLog4 warningggggggggggggggggg")));
	CPPUNIT_ASSERT_EQUAL(std::string("testLog4 stringsssssssssssssssss"), TLogEngine::getInstance()->FormatStr("testLog4 stringsssssssssssssssss"));
	
	TLogEngine::getInstance()->Stop();

	CloseLogEngine();
}

void LogEngineLogTest::testLog5()
{
	InitLogEngine();
	TLogEngine* log = TLogEngine::getInstance();

	log->WriteErrorFmt  (0, "testLog5 errorrrrrrrrrrrrrrrrrr %7d", 1);
	log->WriteInfoFmt   (0, "testLog5 infoooooooooooooo %7d", 2);
	log->WriteWarningFmt(0, "testLog5 warningggggggggggggggggg %7d", 3);
	log->WriteStrFmt    (0, "testLog5 stringsssssssssssssssss %7d", 4);

	CloseLogEngine();
}

void LogEngineLogTest::testLog6()
{
	InitLogEngine();
	TLogEngine* log = TLogEngine::getInstance();

	log->WriteErrorFmt  (0, "testLog6 11111111111111111 %s", "222222");
	log->WriteInfoFmt   (0, "testLog6 22222222222222222 %s", "333333");
	log->WriteWarningFmt(0, "testLog6 33333333333333333 %s", "444444");
	log->WriteStrFmt    (0, "testLog6 44444444444444444 %s", "555555");

	CloseLogEngine();
}

void LogEngineLogTest::testLogStartStop()
{
	InitLogEngine();
	TLogEngine* log = TLogEngine::getInstance();

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

	CPPUNIT_ASSERT_THROW(TLogEngine::getInstance()->WriteError("write after LogEngine.Stop()!"), LogException);

	//try
	//{
	//	getLogEngine()->WriteError("write after Stop()!");
	//	CPPUNIT_ASSERT_MESSAGE("write after Stop()!", false);
	//}
	//catch(LogException& /*ex*/)
	//{
	//}

	CloseLogEngine();
}

void LogEngineLogTest::testLogInitClose()
{
	Properties prop;
	prop.SetValue("LogFileName", LOG_FILES_FOLDER "testLogInitClose.log");
	
	InitLogEngine();
	InitLogEngine(prop);
	InitLogEngine();
	
	TLogEngine* log1 = TLogEngine::getInstance();
	TLogEngine* log2 = TLogEngine::getInstance();
	
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
}


void LogEngineLogTest::testLogPlaceholders()
{
	// preparing parameters
	Properties prop;
	prop.SetValue("LogFileName", LOG_FILES_FOLDER "LogEngine_tests");
	prop.SetValue("Version", "2.2.2");
	prop.SetValue("ErrorLine",  "%MSG% %APPNAME% %APPVERSION% %OS% %OSVERSION% %DETAILLEVEL%");
	prop.SetValue("WarningLine","%DETAILLEVEL% %OS% %OSVERSION% %APPNAME% %APPVERSION% %MSG%");
	prop.SetValue("InfoLine",   "%OS% %MSG% %APPNAME% %OSVERSION% %DETAILLEVEL% %APPVERSION%");
	
	InitLogEngine(prop);
	TLogEngine *log = TLogEngine::getInstance();
	
	std::string s = log->FormatError("aaa", 6);
	CPPUNIT_ASSERT_EQUAL(std::string("aaa nonameapp 2.2.2 6.3 SP 0.0 4 10.0.22621.2506 6"), log->FormatError("aaa", 6));
	CPPUNIT_ASSERT_EQUAL(std::string("6 6.3 SP 0.0 4 10.0.22621.2506 nonameapp 2.2.2 aaa"), log->FormatWarning("aaa", 6));
	CPPUNIT_ASSERT_EQUAL(std::string("6.3 SP 0.0 4 aaa nonameapp 10.0.22621.2506 6 2.2.2"), log->FormatInfo("aaa", 6));
	CPPUNIT_ASSERT_EQUAL(std::string("aaa"), log->FormatStr("aaa", 6));
}

// TODO
void LogEngineLogTest::testLogDetailLevel()
{
	InitLogEngine(TEST_FILES_FOLDER "test9.lfg");
	TLogEngine* log = TLogEngine::getInstance();
	CPPUNIT_ASSERT_EQUAL(0u, log->GetLogDetailLevel());
	CPPUNIT_ASSERT_EQUAL(1u, log->GetMaxLogSize());
	CPPUNIT_ASSERT_EQUAL(lbNone, log->GetBackupType());
	CPPUNIT_ASSERT_EQUAL(std::string("NONAME.log"), log->GetLogFileName());
	CPPUNIT_ASSERT_EQUAL(std::string("0.0.0.0"), log->GetVersionInfo());

	InitLogEngine(TEST_FILES_FOLDER "test10.lfg");
	log = TLogEngine::getInstance();
	CPPUNIT_ASSERT_EQUAL(4u, log->GetLogDetailLevel());
	CPPUNIT_ASSERT_EQUAL(2u, log->GetMaxLogSize());
	CPPUNIT_ASSERT_EQUAL(lbNone, log->GetBackupType());
	CPPUNIT_ASSERT_EQUAL(std::string("NONAME.log"), log->GetLogFileName());
	CPPUNIT_ASSERT_EQUAL(std::string("0.0.0.0"), log->GetVersionInfo());

	InitLogEngine(TEST_FILES_FOLDER "test11.lfg");
	log = TLogEngine::getInstance();
	CPPUNIT_ASSERT_EQUAL(7u, log->GetLogDetailLevel());
	CPPUNIT_ASSERT_EQUAL(1000u, log->GetMaxLogSize());
	CPPUNIT_ASSERT_EQUAL(lbNone, log->GetBackupType());
	CPPUNIT_ASSERT_EQUAL(std::string("NONAME.log"), log->GetLogFileName());
	CPPUNIT_ASSERT_EQUAL(std::string("0"), log->GetVersionInfo());
}

void LogEngineLogTest::testLogBackupTypeNone()
{
	std::string fileName = LOG_FILES_FOLDER "a3.log";

	// preparing parameters
	Properties prop;
	prop.SetValue("ApplicationName", "testLogBackupTypeNone()");
	prop.SetValue("version", "3.3.3");
	prop.SetValue("backuptype", "None");
	prop.SetValue("maxlogsize", "1");
	prop.SetValue("logfilename", fileName);
	prop.SetValue("InfoLine",  "%TIME% : %MSG%"); // fixed length pattern needed for this test to run properly on Win and Linux
	prop.SetValue("ErrorLine", "%TIME% : %MSG%"); // fixed length pattern needed for this test to run properly on Win and Linux

	std::ifstream iff(fileName);
	if (iff.fail() == false) // if file does not exist then std::remove() fails. below workaround for this
	{
		iff.close();
		int res = std::remove(fileName.c_str());
		if(res == -1)
			throw IOException("Cannot remove file: " + fileName);
	}

	InitLogEngine(prop);
	TLogEngine* log = TLogEngine::getInstance();

	CPPUNIT_ASSERT_EQUAL(fileName, log->GetLogFileName());
	CPPUNIT_ASSERT_EQUAL(1u, log->GetMaxLogSize());
	CPPUNIT_ASSERT_EQUAL(std::string("3.3.3"), log->GetVersionInfo());

	CPPUNIT_ASSERT_EQUAL(lbNone, log->GetBackupType());
	CPPUNIT_ASSERT_EQUAL(79ul, log->GetBytesWritten());
	CPPUNIT_ASSERT_EQUAL(79ul, log->GetTotalBytesWritten());

	while(true)
	{
		log->WriteError("1234567890");
		if(log->GetBytesWritten() > 1024)
			break;
	}
	
	log->Flush();
	struct stat st;
	stat(fileName.c_str(), &st);

	CPPUNIT_ASSERT_EQUAL((ulong)st.st_size, log->GetBytesWritten());
	
	log->WriteInfo("L"); // file will be truncated and re-written from beginning (because BackupType=lbNone)
	CPPUNIT_ASSERT_EQUAL(13ul, log->GetBytesWritten());
	CPPUNIT_ASSERT_EQUAL((ulong)st.st_size + 13ul, log->GetTotalBytesWritten());

	CPPUNIT_ASSERT_EQUAL(43u, log->GetMessageCount(lmError));
	CPPUNIT_ASSERT_EQUAL(0u, log->GetMessageCount(lmNone));
	CPPUNIT_ASSERT_EQUAL(0u, log->GetMessageCount(lmWarning));
	CPPUNIT_ASSERT_EQUAL(1u, log->GetMessageCount(lmInfo));

	CloseLogEngine();
}

void LogEngineLogTest::testLogBackupTypeSingle()
{
	std::string logfname = LOG_FILES_FOLDER "aaa.log";
	// preparing parameters
	Properties prop;
	prop.SetValue("ApplicationName", "LogEngine_tests");
	prop.SetValue("Version", "5.5.5");
	prop.SetValue("backuptype", "single");
	prop.SetValue("maxlogsize", "1");
	prop.SetValue("logfilename", logfname);
	prop.SetValue("ErrorLine", "%TIME% : %MSG%"); //  fixed length pattern needed for this test
	
	remove(logfname.c_str());

	InitLogEngine(prop);
	TLogEngine* log = TLogEngine::getInstance();
	
	CPPUNIT_ASSERT_EQUAL(lbSingle, log->GetBackupType());
	
	while(true)
	{
		log->WriteError("1234567890");
		if(log->GetBytesWritten() > 1024)
			break;
	}
	
	struct stat st;
	stat(logfname.c_str(), &st);
	CPPUNIT_ASSERT_EQUAL((ulong)st.st_size, log->GetBytesWritten());
	CPPUNIT_ASSERT_EQUAL((ulong)st.st_size, log->GetTotalBytesWritten());

	CPPUNIT_ASSERT_EQUAL(44u, log->GetMessageCount(lmError));
	CPPUNIT_ASSERT_EQUAL(0u, log->GetMessageCount(lmNone));
	CPPUNIT_ASSERT_EQUAL(0u, log->GetMessageCount(lmWarning));
	CPPUNIT_ASSERT_EQUAL(0u, log->GetMessageCount(lmInfo));
	
	CloseLogEngine();
}

void LogEngineLogTest::testLogFullPath()
{
	InitLogEngine(TEST_FILES_FOLDER "test12.lfg");
	TLogEngine* log = TLogEngine::getInstance();

	CPPUNIT_ASSERT_EQUAL(11u, log->GetLogDetailLevel());
	CPPUNIT_ASSERT_EQUAL(lbTimeStamp, log->GetBackupType());
	CPPUNIT_ASSERT_EQUAL(101u, log->GetMaxLogSize());
	CPPUNIT_ASSERT_EQUAL(std::string("1.1.1.1"), log->GetVersionInfo());
	CPPUNIT_ASSERT_EQUAL(std::string("ExampleApp11"), log->GetAppName());
	CPPUNIT_ASSERT_EQUAL(std::string("c:\\temp\\ExampleApp11.log"), log->GetLogFileName());	

	CloseLogEngine();
}

void LogEngineLogTest::testLogMacro()
{
	InitLogEngine();

	LOG_ERROR("dfdfdf");
	LOG_ERROR("dfdfdf");

	TLogEngine* log = TLogEngine::getInstance();
	CPPUNIT_ASSERT_EQUAL(2u, log->GetMessageCount(lmError));
	CPPUNIT_ASSERT_EQUAL(0u, log->GetMessageCount(lmNone));
	CPPUNIT_ASSERT_EQUAL(0u, log->GetMessageCount(lmWarning));
	CPPUNIT_ASSERT_EQUAL(0u, log->GetMessageCount(lmInfo));
	CloseLogEngine();
}

void LogEngineLogTest::testLogEmptyFileName()
{
	// preparing parameters
	Properties prop;
	prop.SetValue("BackupType", "None");
	prop.SetValue("DetailLevel", "DefaultDetailLevel"); 
	prop.SetValue("MaxLogSize", "100");
	prop.SetValue("ApplicationName", "logs/LogEngine tests");
	prop.SetValue("Version", "4.4.4");

	InitLogEngine(prop);

	TLogEngine *log = TLogEngine::getInstance();

	std::string s = log->GetAppName();
	CPPUNIT_ASSERT_EQUAL(std::string("logs/LogEngine tests"), s);
	s = log->GetLogFileName();
	CPPUNIT_ASSERT_EQUAL(std::string("logs/LogEngine tests.log"), s);

	CloseLogEngine();
}

void LogEngineLogTest::testLogAppName()
{
	// preparing parameters
	Properties prop;
	prop.SetValue("BackupType", "None");
	prop.SetValue("ApplicationName", "logs/AAAAAAAA");
	prop.SetValue("Version", "1.0.555");
	prop.SetValue("DetailLevel", "0");

	InitLogEngine(prop);
	
	TLogEngine *log = TLogEngine::getInstance();

	std::string s = log->GetAppName();
	CPPUNIT_ASSERT_EQUAL(std::string("logs/AAAAAAAA"), s);
	s = log->GetLogFileName();
	CPPUNIT_ASSERT_EQUAL(std::string("logs/AAAAAAAA.log"), s);

	log->Stop();
	log->Start();
	CloseLogEngine();
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
#ifdef WIN32 // '\n' is not alowed in Windows filenames, but allowed in Linux filenames
	CPPUNIT_ASSERT_THROW( InitLogEngine(TEST_FILES_FOLDER "test13.lfg"), IOException );
#else
	InitLogEngine(TEST_FILES_FOLDER "test13.lfg"); // just make sure that no exception is thrown in contrast to Windows
#endif
	CloseLogEngine();
}

void LogEngineLogTest::testBadLFGFile2()
{
	InitLogEngine(TEST_FILES_FOLDER "test14.lfg");
	TLogEngine* log = TLogEngine::getInstance();

	CPPUNIT_ASSERT_EQUAL(0u, log->GetLogDetailLevel());
	CPPUNIT_ASSERT_EQUAL(lbNone, log->GetBackupType());
	CPPUNIT_ASSERT_EQUAL(1000u, log->GetMaxLogSize());
	CPPUNIT_ASSERT_EQUAL(std::string("d.d.d.d"), log->GetVersionInfo());
	CPPUNIT_ASSERT_EQUAL(std::string("BadLFGFileApp"), log->GetAppName());
	CPPUNIT_ASSERT_EQUAL(std::string("logs/BadLFGLog.log"), log->GetLogFileName());

	CloseLogEngine();
}

void LogEngineLogTest::testLogRotation1()
{
	std::string logfname = LOG_FILES_FOLDER "TestLogRotation.log";
	remove(logfname.c_str()); // name sure that previously created log file does not exist

	Properties props;
	props.SetValue("LogFileName", logfname);
	props.SetValue("BackupType", "TimeStamp");
	props.SetValue("MaxLogSize", "1"); // 1 Kilobyte
	
	InitLogEngine(props);
	TLogEngine* log = TLogEngine::getInstance();

	CPPUNIT_ASSERT_EQUAL(0u, log->GetMessageCount(lmNone));

	ulong StartMsgLen = 67;
	ulong MaxLogSize = 1024;
	CPPUNIT_ASSERT_EQUAL(StartMsgLen, log->GetBytesWritten());
	CPPUNIT_ASSERT_EQUAL(StartMsgLen, log->GetTotalBytesWritten());

	std::string str;
	str.resize(MaxLogSize - StartMsgLen - 1, 'M'); // we deduc 2 because WriteStr writes str AND 'CRLF' - 3 bytes in total
	log->WriteStr(str); 
	
	CPPUNIT_ASSERT_EQUAL(MaxLogSize, log->GetBytesWritten());
	CPPUNIT_ASSERT_EQUAL(MaxLogSize, log->GetTotalBytesWritten());
	log->WriteStr("F"); // 3 bytes written
	CPPUNIT_ASSERT_EQUAL(MaxLogSize + 2, log->GetBytesWritten());
	CPPUNIT_ASSERT_EQUAL(MaxLogSize + 2, log->GetTotalBytesWritten());
	
	log->WriteStr("A"); // 3 bytes written
	CPPUNIT_ASSERT_EQUAL(2ul, log->GetBytesWritten());
	CPPUNIT_ASSERT_EQUAL(MaxLogSize + 4, log->GetTotalBytesWritten());

// wait to make sure that next backup file will have different name.
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	str.resize(MaxLogSize + 1, 'M');
	log->WriteStr(str);
	CPPUNIT_ASSERT_EQUAL(MaxLogSize + 4, log->GetBytesWritten());
	CPPUNIT_ASSERT_EQUAL(MaxLogSize + 4 + MaxLogSize + 2, log->GetTotalBytesWritten());

	log->WriteStr("G");
	CPPUNIT_ASSERT_EQUAL(2ul, log->GetBytesWritten());
	CPPUNIT_ASSERT_EQUAL(MaxLogSize + 6 + MaxLogSize + 2, log->GetTotalBytesWritten());

	CPPUNIT_ASSERT_EQUAL(5u, log->GetMessageCount(lmNone));
	CPPUNIT_ASSERT_EQUAL(0u, log->GetMessageCount(lmError));
	CPPUNIT_ASSERT_EQUAL(0u, log->GetMessageCount(lmWarning));
	CPPUNIT_ASSERT_EQUAL(0u, log->GetMessageCount(lmInfo));

	CloseLogEngine();
}


#undef TEST_FILES_FOLDER
