
#include "Shared.h"
#include "functions.h"
#include "testUtils.h"

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
	std::string s = StringReplace("", "", "");
	CPPUNIT_ASSERT_EQUAL((size_t)0, s.size());
}

void LogEngineUtilsTest::testStringReplace2()
{
	std::string s = StringReplace("", "a", "b");
	CPPUNIT_ASSERT_EQUAL((size_t)0, s.size());
}

void LogEngineUtilsTest::testStringReplace3()
{
	std::string s = StringReplace("", "zzz", "bbb");
	CPPUNIT_ASSERT_EQUAL((size_t)0, s.size());
}

void LogEngineUtilsTest::testStringReplace4()
{
	std::string s = StringReplace("", "zzz", "");
	CPPUNIT_ASSERT_EQUAL((size_t)0, s.size());
}

void LogEngineUtilsTest::testStringReplace5()
{
	std::string s = StringReplace("", "", "bbbb");
	CPPUNIT_ASSERT_EQUAL((size_t)0, s.size());
}

void LogEngineUtilsTest::testStringReplace6()
{
	std::string s = StringReplace("gaaah vvvvb aa aaa hghg4aaa54", "aaa", "ZZZZ");
	CPPUNIT_ASSERT(s.compare("gZZZZh vvvvb aa ZZZZ hghg4ZZZZ54") == 0);
}

void LogEngineUtilsTest::testStringReplace7()
{
	std::string s = StringReplace("gaaah vvvvb aa aaa hghg4aaa", "aaa", "");
	CPPUNIT_ASSERT_EQUAL(std::string("gh vvvvb aa  hghg4"), s);
}

void LogEngineUtilsTest::testExtractFileName1()
{
	std::string s = ExtractFileName("c:\\aaa\\bbb\\pp.log");
	CPPUNIT_ASSERT_EQUAL(std::string("pp.log"), s);
}

void LogEngineUtilsTest::testExtractFileName2()
{
	std::string s = ExtractFileName("\\sssss.ss\\dsd\\ffffff");
	CPPUNIT_ASSERT_EQUAL(std::string{ "ffffff" }, s);
}

void LogEngineUtilsTest::testExtractFileName3()
{
	std::string s = ExtractFileName("/dfdfd/dfdf/gfg/dffff/");
	CPPUNIT_ASSERT_EQUAL(std::string(""), s);
}

void LogEngineUtilsTest::testExtractFileName4()
{
	std::string s = ExtractFileName("app.log.fgh");
    CPPUNIT_ASSERT_EQUAL(std::string("app.log.fgh"), s);
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
