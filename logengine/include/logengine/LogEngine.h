/*
 * LogEngine.h
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef LOG_ENGINE_H
#define LOG_ENGINE_H

#include <exception>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include "FileStream.h"
#include "Properties.h"
#include "LogEvent.h"
#include "Line.h"
#include "SynchronizedQueue.h"


enum TLogBackupType { lbNone, lbTimeStamp, lbSingle };
//enum TLogMessageType {lmError, lmWarning, lmInfo};

#define DefaultDetailLevel 0  // see  example.cfg for more details
#define DefaultMaxLogSize  1000 // size in kilobytes here 
#define DefaultBackupType  lbNone

//typedef SynchronizedQueue<LogEvent*> LogEventQueue;
typedef SafeQueue<LogEvent*> LogEventQueue;

class LogException : public std::exception
{
public:
	//LogException(const char * Message);
    LogException(const char* formatstr, ...);
	LogException(const std::string& Message) { Text = Message; }
	LogException(const LogException& ex) { Text = ex.Text; }
	virtual ~LogException() noexcept{ /*nothing to do */ }
	LogException& operator=(const LogException& rhs);
	const char *what() const noexcept override; // throw();
	//std::string GetError(void);
private:
	std::string Text;
};

	
class TLogEngine  
{

//friend class Line;

private:
	class LogEngineProperties
	{
	public:
		LogEngineProperties();
		LogEngineProperties(const Properties& Props);
		//TLogEngineProperties& operator=(const TLogEngineProperties& rhs);
		void Fill(const Properties& Props);
		TLogBackupType GetBackupType(const Properties& Props, const TLogBackupType defaultValue);
		
		TLogBackupType BackupType;
		std::string LogFileName;
		std::string ApplicationName;
		std::string Version;
		uint MaxLogSize;
		uint DetailLevel;
		bool Threaded;
		
		//templates
		Line SeparatorLine;
		Line ErrorLine;
		Line WarningLine;
		Line InfoLine;
		//Line LongExceptionLine;   //for future use
		//Line ShortExceptionLine;  //for future use
		Line StartAppLine;
		Line StopAppLine;
	};

	LogEngineProperties FProperties;
	
	TFileStream* FLogStream;
	ulong FFileBytesWritten;
	ulong FTotalBytesWritten;
	ulong FInitialFileSize;
	uint FMessageCount[4];
	bool FStarted;
	//FMessageTime[3];
	//FStartupTime;

	std::thread m_thread;
	//THREAD_TYPE hThread;
	//MUTEX_TYPE CriticalSection;
	std::recursive_mutex mtx;

protected:
	LogEventQueue LogQueue;
	static TLogEngine* loginstance;

	TLogEngine();
	TLogEngine(const Properties& Props);
	TLogEngine(const std::string& ConfigFileName);
	virtual ~TLogEngine();
	
	void initialInit();
	void initThread();
	void truncLogFile(void);
	void internalWrite(const std::string& msg);
	void writeStart(void);
	void writeStop(void);
	void writeEvent(LogEvent* event);
	std::string generateBackupName(void);
	void resetStatistics();
	ulong getFileLength();

	static int ThreadProc(void* parameter);
public:
	//int FLogDetailLevel;
	void Start(void);
	void Stop(void);
	bool IsStarted() { return FStarted; }
	void Flush(); // forces save data to the disk

	//log format functions
	std::string FormatStr	  (const std::string& str, uint DetailLevel = DefaultDetailLevel);
	std::string FormatInfo	  (const std::string& str, uint DetailLevel = DefaultDetailLevel);
	std::string FormatWarning (const std::string& str, uint DetailLevel = DefaultDetailLevel);
	std::string FormatError	  (const std::string& str, uint DetailLevel = DefaultDetailLevel);

	//write functions
	void WriteStr	  (const std::string& str, uint DetailLevel = DefaultDetailLevel);
	void WriteInfo	  (const std::string& str, uint DetailLevel = DefaultDetailLevel);
	void WriteWarning (const std::string& str, uint DetailLevel = DefaultDetailLevel);
	void WriteError	  (const std::string& str, uint DetailLevel = DefaultDetailLevel);

	// formatted write functions
	void WriteStrFmt	(uint DetailLevel, const char* formatstr,...);
	void WriteInfoFmt	(uint DetailLevel, const char* formatstr,...);
	void WriteWarningFmt(uint DetailLevel, const char* formatstr,...);
	void WriteErrorFmt	(uint DetailLevel, const char* formatstr,...);

	void SetVersionInfo(const std::string& VerInfo);
	void SetAppName    (const std::string& AppName);
	void SetMaxLogSize (const uint MaxLogSize);
	void SetBackupType (const TLogBackupType BackupType);
	void SetLogDetailLevel(const uint DetailLevel); 
	void SetLogFileName(const std::string& newFileName);

	TLogBackupType GetBackupType(void)	   const { return FProperties.BackupType; }
	uint		   GetMaxLogSize(void)	   const { return FProperties.MaxLogSize; }
	std::string	   GetLogFileName(void)	   const { return FProperties.LogFileName; }
	std::string	   GetAppName(void)		   const { return FProperties.ApplicationName; }
	std::string	   GetVersionInfo(void)	   const { return FProperties.Version; }
	uint		   GetLogDetailLevel(void) const { return FProperties.DetailLevel; } 
	ulong		   GetBytesWritten(void)   const { return FFileBytesWritten; }
	ulong		   GetTotalBytesWritten(void) const { return FTotalBytesWritten; }
	uint		   GetMessageCount(TLogMessageType msgType) const { return FMessageCount[msgType]; }

	static TLogEngine* getInstance();
	static TLogEngine* getInstance(const Properties& Props);
	static void InitLogEngine();
	static void InitLogEngine(const std::string& ConfigFileName);
	static void InitLogEngine(const Properties& Props);
	static void CloseLogEngine();	

};

struct LogEngineThreadInfo
{
    LogEventQueue	*LogQueue;
    TLogEngine		*LogEngine;
};


//TLogEngine* getLogEngine();
//TLogEngine* getLogEngine(const Properties& Props);
void InitLogEngine(const Properties& Props);
void InitLogEngine(const std::string& ConfigFileName);
void InitLogEngine();
void CloseLogEngine();

inline void LOG_ERROR(const std::string& mess, uint DetailLevel = DefaultDetailLevel)
{
	TLogEngine* log = TLogEngine::getInstance();
    if(log)
		log->WriteError(mess, DetailLevel);
}

	
inline void LOG_INFO(const std::string& mess, uint DetailLevel = DefaultDetailLevel)
{
	TLogEngine* log = TLogEngine::getInstance();
	if(log)
		log->WriteInfo(mess, DetailLevel);
}


inline void LOG_WARN(const std::string& mess, uint DetailLevel = DefaultDetailLevel)
{
	TLogEngine* log = TLogEngine::getInstance();
	if(log)
		log->WriteWarning(mess, DetailLevel);
}

inline void LOG_STR(const std::string& mess, uint DetailLevel = DefaultDetailLevel)
{
	TLogEngine* log = TLogEngine::getInstance();
	if(log)
		log->WriteStr(mess, DetailLevel);
}

	
#endif //LOG_ENGINE_H
