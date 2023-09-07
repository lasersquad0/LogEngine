/*
 * Compare.h
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef _COMPARE_H_
#define _COMPARE_H_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

#include <exception> //unix
#include <string>
#include "functions.h"

/// Class to compare classes by operators =, <, >
template<class C>
class Compare
{
public:
    // returns True when object a is equal b
	virtual bool eq(const C& a, const C& b) const { return a == b; };
	virtual bool lt(const C& a, const C& b) const { return a < b;  }; 
	virtual bool mt(const C& a, const C& b) const { return a > b;  }; 
	virtual ~Compare() {};
};

/// Class to compare std::string WITHOUT CASE sensitivity
template<>
class Compare<std::string>
{
public:
    // returns True when string a is equal b
	virtual bool eq(const std::string& a, const std::string& b) const { return EqualNCase(a, b); };

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable : 4100)
#endif

	virtual bool lt(const std::string& a, const std::string& b) const { throw std::exception();/*"string comparing don't process operator '<'");*/ };
	virtual bool mt(const std::string& a, const std::string& b) const { throw std::exception();/*"string comparing don't process operator '>'");*/ };

#ifdef WIN32
#pragma warning(pop)
#endif

	virtual ~Compare() {};
};


#endif //_COMPARE_H_
