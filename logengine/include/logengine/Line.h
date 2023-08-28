/*
 * Line.h
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#if !defined(_LINE_H_)
#define _LINE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "functions.h"
#include "DynamicArrays.h"
#include "LogEvent.h"

#ifdef WIN32
#include <windows.h>
#endif


class Holder
{
public:
	virtual std::string format(LogEvent& event) = 0;
};

class LiteralHolder: public Holder
{
private:
	std::string value;
public:
	LiteralHolder(std::string value){ this->value = value; }
#pragma warning(push)
#pragma warning(disable: 4100)
	std::string format(LogEvent& event){ return value; }
#pragma warning(pop)
};

class DateHolder: public Holder
{
public:
	std::string format(LogEvent& event)
	{ 
#if __STDC_SECURE_LIB__ //_MSC_VER < 1400
        // TODO: check for return value
        tm tp;
        localtime_s(&tp, &(event.time.time));
        char ss[100];
        strftime(ss, 100, "%d-%b-%Y", &tp);
        return ss;
#else
        struct tm *tp = localtime(&(event.time.time));
        char ss[100];
        strftime(ss, 100, "%d-%b-%Y", tp);
        return ss;
#endif
	};
};

class TimeHolder: public Holder
{
public:
	std::string format(LogEvent& event)
	{ 
#if __STDC_SECURE_LIB__ // _MSC_VER < 1400
        //TODO: check return value here
        tm tp;
        localtime_s(&tp, &(event.time.time));
        char ss[100];
        strftime(ss, 100, "%X", &tp);
        return ss;
#else        
        struct tm *tp = localtime(&(event.time.time));
        char ss[100];
        strftime(ss, 100, "%X", tp);
        return ss;
#endif

	};
};

class DateTimeHolder: public Holder
{
public:
	std::string format(LogEvent& event)
	{ 
#if __STDC_SECURE_LIB__ //_MSC_VER < 1400
        // TODO: check return value here
        tm tp;
        localtime_s(&tp, &(event.time.time));
        char ss[100];
        strftime(ss, 100, "%d-%b-%Y %X", &tp);
        return ss;
#else        
        struct tm *tp = localtime(&(event.time.time));
        char ss[100];
        strftime(ss, 100, "%d-%b-%Y %X", tp);
        return ss;
#endif
	};
};

class MessageHolder: public Holder
{
public:
	std::string format(LogEvent& event){ return event.message; }
};

class ThreadHolder: public Holder
{
public:
	std::string format(LogEvent& event){ return IntToStr(event.threadID, 4); }
};

class AppNameHolder: public Holder
{
public:
	std::string format(LogEvent& event);
};

class AppVersionHolder: public Holder
{
public:
	std::string format(LogEvent& event);
};

class OSHolder: public Holder
{
public:
	std::string format(LogEvent& event)
	{
		return "<OS>"; 
	}
};

class OSVersionHolder: public Holder
{
public:
	std::string format(LogEvent& event)
	{
#ifdef WIN32
		OSVERSIONINFO ver;
		ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		
		DWORD OSMajorVer = 0, OSMinorVer = 0, SPMajorVer = 0, SPMinorVer = 0;
		DWORD result;
		GetProductInfo(OSMajorVer, OSMinorVer, SPMajorVer, SPMinorVer, &result);

		//GetVersionEx(&ver);
		
		return "<need to be implemented>";//IntToStr(ver.dwMajorVersion) + "." + IntToStr(ver.dwMinorVersion) + " build " + IntToStr(ver.dwBuildNumber);
#else
		return "<OSVERSION>";
#endif
	}
};

class DetailLevelHolder: public Holder
{
public:
	std::string format(LogEvent& event){ return IntToStr(event.detailLevel); }
};

class Line
{
protected:
	//TLogEngine* logParent;
    // container of pointers is required here to support proper virtual "->format()" calls 
	THArray<Holder*> holders;
	void parsePattern(const std::string pattern);
public:
	Line(){}
    Line(const Line& line); 
    Line& operator=(const Line& line);
	Line(std::string pattern);
	//Line& operator=(const std::string pattern);
	virtual ~Line();
	//void setLogEngine(TLogEngine* log){ logParent = log; }
	std::string format(LogEvent& event);
    void ProcessPattern(const std::string pattern);
	
};


#endif // !defined(_LINE_H_)
