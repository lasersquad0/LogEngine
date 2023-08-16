//#include "debug_support.h"
#include "testLine.h"
#include "Line.h"
#include "LogEvent.h"

CPPUNIT_TEST_SUITE_REGISTRATION( LogEngineLineTest );

timeb getTime()
{
	timeb tm;
	tm.millitm = 111;
	tm.time = 444;
	return tm;
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
	printf("testLine1 ...");
	
	Line line("");
	timeb tm = getTime();
	LogEvent event("MSG", lmError, 1, tm, 5, NULL);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT(res.empty());
	
	printf("PASSED.\n");
}

void LogEngineLineTest::testLine2()
{
	printf("testLine2 ...");
	
	Line line("%DATE%Line%TIME%");
	timeb tm = getTime();
	LogEvent event("MSG", lmError, 1, tm, 5, NULL);
	
	std::string res = line.format(event);
    CPPUNIT_ASSERT_EQUAL(std::string("01-Jan-1970Line02:07:24"), res);
	
	printf("PASSED.\n");
}

void LogEngineLineTest::testLine3()
{
	printf("testLine3 ...");
	
	Line line("Line");
	timeb tm = getTime();
	LogEvent event("MSG", lmError, 1, tm, 5, NULL);

	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("Line"), res);
	
	printf("PASSED.\n");
}

void LogEngineLineTest::testLine4()
{
	printf("testLine4 ...");
	
	Line line("%DATE%");
	timeb tm = getTime();
	LogEvent event("MSG", lmError, 1, tm, 5, NULL);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("01-Jan-1970"), res);
	
	printf("PASSED.\n");
}

void LogEngineLineTest::testLine5()
{
	printf("testLine5 ...");
	
	Line line("%DATE%%DATE%%THREAD%");
	timeb tm = getTime();
	LogEvent event("MSG", lmError, 1, tm, 5, NULL);

	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("01-Jan-197001-Jan-19701   "), res);
	
	printf("PASSED.\n");
}

void LogEngineLineTest::testLine6()
{
	printf("testLine6 ...");
	
	Line line("%DsssATE%%DATE%");
	timeb tm = getTime();
	LogEvent event("MSG", lmError, 1, tm, 5, NULL);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("%DsssATE%01-Jan-1970"), res);
	
	printf("PASSED.\n");
}

void LogEngineLineTest::testLine7()
{
	printf("testLine7 ...");
	
	Line line("%DsssATE%");
	timeb tm = getTime();
	LogEvent event("MSG", lmError, 1, tm, 5, NULL);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("%DsssATE%"), res);
	
	printf("PASSED.\n");
}

void LogEngineLineTest::testLine8()
{
	printf("testLine8 ...");
	
	Line line("DsssATE%");
	timeb tm = getTime();
	LogEvent event("MSG", lmError, 1, tm, 5, NULL);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("DsssATE%"), res);
	
	printf("PASSED.\n");
}

void LogEngineLineTest::testLine9()
{
	printf("testLine9 ...");
	
	Line line("dff%%dsf");
	timeb tm = getTime();
	LogEvent event("MSG", lmError, 1, tm, 5, NULL);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("dff%%dsf"), res);
	
	printf("PASSED.\n");
}

void LogEngineLineTest::testLine10()
{
	printf("testLine10 ...");
	
	Line line("d%%d%d");
	timeb tm = getTime();
	LogEvent event("MSG", lmError, 1, tm, 5, NULL);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("d%%d%d"), res);
	
	printf("PASSED.\n");
}

void LogEngineLineTest::testLine11()
{
	printf("testLine11 ...");
	
	Line line("Line%DATE%mama%MSG%%TIME%");
	timeb tm = getTime();
	LogEvent event("MSG", lmError, 1, tm, 5, NULL);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("Line01-Jan-1970mamaMSG02:07:24"), res);
	LogEvent event2("789", lmError, 1, tm, 5, NULL);
	
	res = line.format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("Line01-Jan-1970mama78902:07:24"), res);
	
	printf("PASSED.\n");
}

void LogEngineLineTest::testLine12()
{
	printf("testLine12 ...");
	
	Line line("testLine12 %DATE% %MSG% %TIME% #%THREAD");
	timeb tm = getTime();
	LogEvent event("message", lmError, 1, tm, 5, NULL);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine12 01-Jan-1970 message 02:07:24 #%THREAD"), res);
	
	LogEvent event2("789", lmError, 2, tm, 6, NULL);
	res = line.format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine12 01-Jan-1970 789 02:07:24 #%THREAD"), res);
	
	printf("PASSED.\n");
}


void LogEngineLineTest::testLine13()
{
	printf("testLine13 ...");
	
	Line line("testLine13 %DATE% %MSG% %TIME% %THRD% #%THREAD%");
	timeb tm = getTime();
	LogEvent event("message", lmError, 1, tm, 5, NULL);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine13 01-Jan-1970 message 02:07:24 %THRD% #1   "), res);
	
	LogEvent event2("789", lmError, 2, tm, 6, NULL);
	res = line.format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine13 01-Jan-1970 789 02:07:24 %THRD% #2   "), res);
	
	printf("PASSED.\n");
}

void LogEngineLineTest::testLine14()
{
	printf("testLine14 ...");
	
	Line line("testLine14 %DATETIME%..%DATE%..%TIME%");
	timeb tm = getTime();
	LogEvent event("message", lmError, 1, tm, 5, NULL);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine14 01-Jan-1970 02:07:24..01-Jan-1970..02:07:24"), res);
	
	printf("PASSED.\n");
}

void LogEngineLineTest::testLine15()
{
	printf("testLine15 ...");
	
	Line line("testLine15 %OSVERSION%");
	timeb tm = getTime();
	LogEvent event("message", lmError, 1, tm, 5, NULL);
	
	std::string res = line.format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine15 5.1 build 2600"), res);
	
	printf("PASSED.\n");
}
