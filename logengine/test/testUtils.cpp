#include <string>
#include "functions.h"
#include "testUtils.h"
//#include "debug_support.h"

CPPUNIT_TEST_SUITE_REGISTRATION( LogEngineUtilsTest );

void LogEngineUtilsTest::setUp ()
{

}

void LogEngineUtilsTest::tearDown ()
{
    // free memory allocated in setUp, do other things
}

void LogEngineUtilsTest::testStringReplace1()
{
	printf("testStringReplace1 ... ");

	std::string s = StringReplace("", "", "");
	CPPUNIT_ASSERT(s.size() == 0);

	printf("PASSED \n");
}

void LogEngineUtilsTest::testStringReplace2()
{
	printf("testStringReplace2 ... ");

	std::string s = StringReplace("", "a", "b");
	CPPUNIT_ASSERT(s.size() == 0);

	printf("PASSED \n");
}

void LogEngineUtilsTest::testStringReplace3()
{
	printf("testStringReplace3 ... ");

	std::string s = StringReplace("", "zzz", "bbb");
	CPPUNIT_ASSERT(s.size() == 0);

	printf("PASSED \n");
}

void LogEngineUtilsTest::testStringReplace4()
{
	printf("testStringReplace4 ... ");

	std::string s = StringReplace("", "zzz", "");
	CPPUNIT_ASSERT(s.size() == 0);

	printf("PASSED \n");
}

void LogEngineUtilsTest::testStringReplace5()
{
	printf("testStringReplace5 ... ");

	std::string s = StringReplace("", "", "bbbb");
	CPPUNIT_ASSERT(s.size() == 0);

	printf("PASSED \n");
}

void LogEngineUtilsTest::testStringReplace6()
{
	printf("testStringReplace6 ... ");

	std::string s = StringReplace("gaaah vvvvb aa aaa hghg4aaa54", "aaa", "ZZZZ");
	CPPUNIT_ASSERT(s.compare("gZZZZh vvvvb aa ZZZZ hghg4ZZZZ54") == 0);

	printf("PASSED \n");
}

void LogEngineUtilsTest::testStringReplace7()
{
	printf("testStringReplace7 ... ");

	std::string s = StringReplace("gaaah vvvvb aa aaa hghg4aaa", "aaa", "");
	CPPUNIT_ASSERT(s.compare("gh vvvvb aa  hghg4") == 0);

	printf("PASSED \n");
}


void LogEngineUtilsTest::testExtractFileName1()
{
	printf("testExtractFileName1 ... ");

	std::string s = ExtractFileName("c:\\aaa\\bbb\\pp.log");
	CPPUNIT_ASSERT(s.compare("pp.log") == 0);

	printf("PASSED \n");
}

void LogEngineUtilsTest::testExtractFileName2()
{
	printf("testExtractFileName2 ... ");

	std::string s = ExtractFileName("\\sssss.ss\\dsd\\ffffff");
	CPPUNIT_ASSERT(s.compare("ffffff") == 0);

	printf("PASSED \n");
}

void LogEngineUtilsTest::testExtractFileName3()
{
	printf("testExtractFileName3 ... ");

	std::string s = ExtractFileName("/dfdfd/dfdf/gfg/dffff/");
	CPPUNIT_ASSERT(s.compare("") == 0);

	printf("PASSED \n");
}

void LogEngineUtilsTest::testExtractFileName4()
{
	printf("testExtractFileName4 ... ");

	std::string s = ExtractFileName("app.log.fgh");
    CPPUNIT_ASSERT_EQUAL(std::string("app.log.fgh"), s);

	printf("PASSED \n");
}

void LogEngineUtilsTest::testIntToStr1()
{
	CPPUNIT_ASSERT_EQUAL(std::string("1"), IntToStr(1, 1));
	CPPUNIT_ASSERT_EQUAL(std::string("34"), IntToStr(34, 1));
	CPPUNIT_ASSERT_EQUAL(std::string("34"), IntToStr(34, 2));
}

void LogEngineUtilsTest::testIntToStr2()
{
	CPPUNIT_ASSERT_EQUAL(std::string("123450"), IntToStr(123450, 6));
	CPPUNIT_ASSERT_EQUAL(std::string("123450"), IntToStr(123450, 5));
	CPPUNIT_ASSERT_EQUAL(std::string("123450 "), IntToStr(123450, 7));
	CPPUNIT_ASSERT_EQUAL(std::string("12     "), IntToStr(12, 7));
}
