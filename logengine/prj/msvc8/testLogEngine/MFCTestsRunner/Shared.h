#pragma once

#ifdef WIN32
#include <string>
#include <iostream>
//#include <fstream>
#else
#include <string.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <cppunit/extensions/HelperMacros.h>

#ifdef WIN32
#define TEST_FILES_FOLDER "..\\..\\..\\..\\test\\testFiles\\"
#else
#define TEST_FILES_FOLDER "testFiles/"
#endif

CPPUNIT_NS_BEGIN
template<>
struct assertion_traits<std::string>   // specialization for the std::string type
{
	static bool equal(const std::string& x, const std::string& y)
	{
		return x.compare(y) == 0;
	}

	static std::string toString(const std::string& x)
	{
		return x;
	}
};

template<>
struct assertion_traits<const char*>   // specialization for the char* type
{
	static bool equal(const char* x, const char* y)
	{
		return strcmp(x, y) == 0;
	}

	static std::string toString(const char* x)
	{
		std::string text = '"' + x + '"';    // adds quote around the string to see whitespace
		CppUnit::OStringStream ost;
		ost << text;
		return ost.str();
	}
};

template<>
struct assertion_traits<char*>   // specialization for the char* type
{
	static bool equal(char* x, char* y)
	{
		return strcmp(x, y) == 0;
	}

	static std::string toString(char* x)
	{
		std::string text = '"' + x + '"';    // adds quote around the string to see whitespace
		CppUnit::OStringStream ost;
		ost << text;
		return ost.str();
	}
};

CPPUNIT_NS_END

std::string millisecToStr(long long ms);

