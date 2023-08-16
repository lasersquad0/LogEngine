/*
 * LogEvent.h
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#if !defined(_LOGEVENT_H_)
#define _LOGEVENT_H_

#include <string>
#include <sys/timeb.h>

//#include <sys/types.h>

enum TLogMessageType {lmNone, lmError, lmWarning, lmInfo};
//enum TLogMessageType;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TLogEngine;

class LogEvent  
{
protected:
public:
	TLogEngine* logEngine;
	std::string message;
	TLogMessageType msgType;
	int threadID;
	timeb time;
	int detailLevel;
	              
	LogEvent();
	LogEvent(
		std::string message, 
		TLogMessageType msgType, 
		int threadID, 
		timeb time, 
		int detailLevel,
		TLogEngine* log
		)
		:message(message), msgType(msgType), threadID(threadID), time(time), 
		detailLevel(detailLevel), logEngine(log){}
	virtual ~LogEvent();
	
};

#endif // !defined(_LOGEVENT_H_)
