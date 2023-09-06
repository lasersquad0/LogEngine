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
#include "functions.h"

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
	TLogEngine* m_logEngine;
	TLogMessageType m_msgType;
	unsigned int m_threadID; // this is a thread that generated a log message. it may differ from thread that makes actual writing to the file (in case LogEngone.Threaded property is set to true)
	struct tm m_time;
	unsigned int m_detailLevel;
	std::string m_message;  

	LogEvent() :  m_logEngine(nullptr) , m_msgType(lmNone), m_threadID(0), m_time{0}, m_detailLevel(0), m_message() {}
	LogEvent(
		std::string message, 
		TLogMessageType msgType, 
		unsigned int threadID, 
		struct tm time, 
		unsigned int detailLevel,
		TLogEngine* log
		)
		:m_message(message), m_msgType(msgType), m_threadID(threadID), m_time(time), 
		m_detailLevel(detailLevel), m_logEngine(log) {}

	virtual ~LogEvent() {};
	
};

#endif // !defined(_LOGEVENT_H_)
