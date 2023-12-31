/*
 * Line.h
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#if !defined(_LINE_H_)
#define _LINE_H_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

#include <vector>
#include <chrono>
#include <iomanip>
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
	virtual ~Holder() {};
};

#define STRFTIME_SIZE 100

class DateHolder: public Holder
{
public:
	std::string format(LogEvent& event)
	{ 
        char ss[STRFTIME_SIZE];
        std::strftime(ss, STRFTIME_SIZE, "%d-%b-%Y", &event.m_time);
        return ss;
	};
};

class TimeHolder: public Holder
{
public:
	std::string format(LogEvent& event)
	{ 
		char ss[STRFTIME_SIZE];
        std::strftime(ss, STRFTIME_SIZE, "%X", &event.m_time);
        return ss;
	};
};

class DateTimeHolder: public Holder
{
public:
	std::string format(LogEvent& event)
	{ 
        char ss[STRFTIME_SIZE];
        std::strftime(ss, STRFTIME_SIZE, "%d-%b-%Y %X", &event.m_time);
        return ss;
	};
};

class MessageHolder: public Holder
{
public:
	std::string format(LogEvent& event){ return event.m_message; }
};

class ThreadHolder: public Holder
{
public:
	std::string format(LogEvent& event){ return IntToStr(event.m_threadID, 4); }
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

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable : 4100)
#endif

class LiteralHolder : public Holder
{
private:
	std::string value;
public:
	LiteralHolder(std::string value) { this->value = value; }
	std::string format(LogEvent& event) { return value; }
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

#ifdef WIN32
#pragma warning(pop)
#endif

class DetailLevelHolder: public Holder
{
public:
	std::string format(LogEvent& event){ return IntToStr(event.m_detailLevel); }
};

class Line
{
protected:
	//TLogEngine* logParent;
    // container of pointers is required here to support proper virtual "->format()" calls 
	THArray<Holder*> holders;
	void parsePattern(const std::string pattern);
	void clearHolders();
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


#endif // _LINE_H_
