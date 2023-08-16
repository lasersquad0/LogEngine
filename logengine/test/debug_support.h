#ifndef _ASSERT_H_
#define _ASSERT_H_


#include "stdio.h"
#include <exception>
#include <string>
#include <strstream>


#ifdef _DEBUG

// Conditional defines for Borland C++ Builder 6 which implementation of STL
// does not have constructor of std::exception(char *).
// The Microsoft and most of Linux implementations are have such constructor.
#ifdef __BORLANDC__
class d_exception: public std::exception
{
	std::string mess;
public:
	d_exception(char* str){ mess = str; }
	const char* what() const { return mess.c_str();}
};

#define ASSERT_EXCEPTION d_exception

#else

#define ASSERT_EXCEPTION std::exception

#endif //__BORLANDC__

template<class T>
inline std::string toString(const T& value)
{
	std::strstream o;
	o << value;
	return std::string(o.str(), o.pcount());
}

inline void _LOG_ASSERT(std::string cond, std::string filename, int line)
//inline void _ASSERT(char* cond, char* filename, int line)
{
	char str[1024];
#if _MSC_VER < 1400
	sprintf(str, "Assert triggered ('%s') in the file %s(%d)", cond.c_str(), filename.c_str(), line);
#else
    sprintf_s(str, 1024, "Assert triggered ('%s') in the file %s(%d)", cond.c_str(), filename.c_str(), line);
#endif

	throw ASSERT_EXCEPTION(str); //std::exception((const char*)str);
};

inline void _LOG_ASSERT_EQUAL(std::string expected, std::string actual, char* filename, int line)
{
	char str[1024];
#if _MSC_VER < 1400
	sprintf(str, "Expected value '%s' but was '%s' in the file %s(%d)", expected.c_str(), actual.c_str(), filename, line);
#else
    sprintf_s(str, 1024, "Expected value '%s' but was '%s' in the file %s(%d)", expected.c_str(), actual.c_str(), filename, line);
#endif

	throw ASSERT_EXCEPTION(str); //std::exception(str);
};

#define LOG_ASSERT(condition) if(condition){}else{ _LOG_ASSERT(#condition, __FILE__, __LINE__); }

#define LOG_ASSERT_EQUAL(expected, actual) if((expected) != (actual)){_LOG_ASSERT_EQUAL(toString(expected), toString(actual), __FILE__, __LINE__); }

#define LOG_ASSERT_MESSAGE(message, condition)if(condition){}else{ _LOG_ASSERT(toString(message), __FILE__, __LINE__); }

#else  //_DEBUG

#define LOG_ASSERT
#define LOG_ASSERT_EQUAL
#define LOG_ASSERT_MESSAGE

#endif //_DEBUG

#endif //_ASSERT_H_
