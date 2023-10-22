/*
 * Compare.h
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef _COMPARE_H_
#define _COMPARE_H_

//#include <exception> //unix
#include <string>
#include <algorithm>
#include "functions.h"

/// Class to compare classes by operators =, <, >
template<class C>
class Compare
{
public:
    // returns True when object a is equal b
	virtual bool eq(const C& a, const C& b) const { return a == b; };
	virtual bool lt(const C& a, const C& b) const { return b > a;  }; 
	virtual bool mt(const C& a, const C& b) const { return a > b;  }; 
	virtual ~Compare() {};
};

template<class C>
class CompareReverse : public Compare<C>
{
public:
	bool eq(const C& a, const C& b) const override 
	{
		return a == b;
	};

	bool lt(const C& a, const C& b) const override
	{
		return a > b;
	};

	bool mt(const C& a, const C& b) const override
	{
		return b > a;
	};
};

// Class to compare std::string WITHOUT CASE sensitivity
class CompareStringNCase: public Compare<std::string>
{
public:
	bool eq(const std::string& a, const std::string& b) const override // returns True when string a is equal b
	{ 
		return EqualNCase(a, b); 
	};

	bool lt(const std::string& a, const std::string& b) const override
	{
		return CompareNCase(a, b) < 0;

		/*std::string aa = a, bb = b;
		std::transform(aa.begin(), aa.end(), aa.begin(), ::toupper);
		std::transform(bb.begin(), bb.end(), bb.begin(), ::toupper);
		return aa.compare(bb) < 0;*/ 
	};

	bool mt(const std::string& a, const std::string& b) const override
	{ 
		return CompareNCase(a, b) > 0;

		/*std::string aa = a, bb = b;
		std::transform(aa.begin(), aa.end(), aa.begin(), ::toupper);
		std::transform(bb.begin(), bb.end(), bb.begin(), ::toupper);
		return aa.compare(bb) > 0;*/ 
	};
};


#endif //_COMPARE_H_
