
#include "Shared.h"
#include "testLine.h"
#include "Line.h"
#include "LogEvent.h"

CPPUNIT_TEST_SUITE_REGISTRATION( LogEngineLineTest );

struct tm getTime()
{
	timeb tm;
	tm.millitm = 111;
	tm.time = 444;
	return *std::localtime(&tm.time);
}


void LogEngineLineTest::setUp ()
{

}

void LogEngineLineTest::tearDown ()
{
    // free memory allocated in setUp, do other things
}

void LogEngineLineTest::testLine1()
{
	Line line("");
	struct tm tp = getTime();
	LogEvent event("MSG", lmError, 1, tp, 5, nullptr);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT(res.empty());
}

void LogEngineLineTest::testLine2()
{
	Line line("%DATE%Line%TIME%");
	struct tm tp = getTime();
	LogEvent event("MSG", lmError, 1, tp, 5, nullptr);
	
	std::string res = line.format(event);
    CPPUNIT_ASSERT_EQUAL(std::string("01-Jan-1970Line03:07:24"), res);
}

void LogEngineLineTest::testLine3()
{
	Line line("Line");
	struct tm tp = getTime();
	LogEvent event("MSG", lmError, 1, tp, 5, nullptr);

	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("Line"), res);
}

void LogEngineLineTest::testLine4()
{
	Line line("%DATE%");
	struct tm tp = getTime();
	LogEvent event("MSG", lmError, 1, tp, 5, nullptr);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("01-Jan-1970"), res);
}

void LogEngineLineTest::testLine5()
{
	Line line("%DATE%%DATE%%THREAD%");
	struct tm tp = getTime();
	LogEvent event("MSG", lmError, 1, tp, 5, nullptr);

	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("01-Jan-197001-Jan-19701   "), res);
}

void LogEngineLineTest::testLine6()
{
	Line line("%DsssATE%%DATE%");
	struct tm tp = getTime();
	LogEvent event("MSG", lmError, 1, tp, 5, nullptr);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("%DsssATE%01-Jan-1970"), res);
}

void LogEngineLineTest::testLine7()
{
	Line line("%DsssATE%");
	struct tm tp = getTime();
	LogEvent event("MSG", lmError, 1, tp, 5, nullptr);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("%DsssATE%"), res);
}

void LogEngineLineTest::testLine8()
{
	Line line("DsssATE%");
	struct tm tp = getTime();
	LogEvent event("MSG", lmError, 1, tp, 5, nullptr);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("DsssATE%"), res);
}

void LogEngineLineTest::testLine9()
{
	Line line("dff%%dsf");
	struct tm tp = getTime();
	LogEvent event("MSG", lmError, 1, tp, 5, nullptr);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("dff%%dsf"), res);
}

void LogEngineLineTest::testLine10()
{
	Line line("d%%d%d");
	struct tm tp = getTime();
	LogEvent event("MSG", lmError, 1, tp, 5, nullptr);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("d%%d%d"), res);
}

void LogEngineLineTest::testLine11()
{
	Line line("Line%DATE%mama%MSG%%TIME%");
	struct tm tp = getTime();
	LogEvent event("MSG", lmError, 1, tp, 5, nullptr);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("Line01-Jan-1970mamaMSG03:07:24"), res);
	LogEvent event2("789", lmError, 1, tp, 5, nullptr);
	
	res = line.format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("Line01-Jan-1970mama78903:07:24"), res);
}

void LogEngineLineTest::testLine12()
{
	Line line("testLine12 %DATE% %MSG% %TIME% #%THREAD");
	struct tm tp = getTime();
	LogEvent event("message", lmError, 1, tp, 5, nullptr);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine12 01-Jan-1970 message 03:07:24 #%THREAD"), res);
	
	LogEvent event2("789", lmError, 2, tp, 6, nullptr);
	res = line.format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine12 01-Jan-1970 789 03:07:24 #%THREAD"), res);
}

void LogEngineLineTest::testLine13()
{
	Line line("testLine13 %DATE% %MSG% %TIME% %THRD% #%THREAD%");
	struct tm tp = getTime();
	LogEvent event("message", lmError, 1, tp, 5, nullptr);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine13 01-Jan-1970 message 03:07:24 %THRD% #1   "), res);
	
	LogEvent event2("789", lmError, 2, tp, 6, nullptr);
	res = line.format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine13 01-Jan-1970 789 03:07:24 %THRD% #2   "), res);
}

void LogEngineLineTest::testLine14()
{
	Line line("testLine14 %DATETIME%..%DATE%..%TIME%");
	struct tm tp = getTime();
	LogEvent event("message", lmError, 1, tp, 5, nullptr);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine14 01-Jan-1970 03:07:24..01-Jan-1970..03:07:24"), res);
}

void LogEngineLineTest::testLine15()
{
	Line line("testLine15 %OSVERSION%");
	struct tm tp = getTime();
	LogEvent event("message", lmError, 1, tp, 5, nullptr);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine15 5.1 build 2600"), res);
}
