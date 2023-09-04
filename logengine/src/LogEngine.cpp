/*
 * LogEngine.cpp
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include <LogEngine.h>
#include <functions.h>
#include <LogEvent.h>
#include <fstream>
#include <string>
#include <chrono>

#ifndef WIN32 //HAVE_STDARG_H assume we have Linux here
#include <stdarg.h>
#include <pthread.h>
#endif

//#ifdef HAVE_PTHREAD_H
//#include <pthread.h>
//#endif
#include <iostream>
 

#define LogExt            ".log"
#define BackupExt         ".bak"

#define SecondMacro       "%SEC%"
#define MinuteMacro       "%MIN%"
#define HourMacro         "%HOUR%"

#define DayMacro          "%DAY%"
#define MonthMacro        "%MONTH%"
#define YearMacro         "%YEAR%"

#define AppNameMacro      "%APPNAME%"
 //char* AppTitleMacro       = "%APPTITLE%";
#define AppVersionMacro   "%APPVERSION%"

#define DateTimeMacro     "%DATETIME%"
#define TimeMacro         "%TIME%"
#define DateMacro         "%DATE%"
#define ThreadMacro		  "%THREAD%"
#define MessageMacro      "%MSG%"

#define ExceptionMacro    "%E%"
#define ClassNameMacro	  "%NAME%"
#define ClassClassMacro	  "%CLASS%"

const char LogTypeChars[5] = " #! ";

#define DefaultLogFileName    "NONAME"
#define DefaultAppName        "noname"
#define DefaultLinePattern    (TimeMacro "#" ThreadMacro " : " MessageMacro)
#define DefaultStartAppLine   "\n%APPNAME% %APPVERSION% startup\nLog is started at %DATETIME%."
#define DefaultStopAppLine    "%APPNAME% %APPVERSION% normal shutdown \nLog stopped at %DATETIME%.\n"
#define DefaultSeparatorLine  "----------------------------------------------------------------"


// 10K should be enough for one format line
#define VARIABLE_LIST_BUFFER_SIZE (10*1024)

//////////////////////////////////////////////////////////////////////
// LogException
//////////////////////////////////////////////////////////////////////

//LogException::LogException(const char * Message)
//{
//	Text = Message; 
//}
//
LogException::LogException(const char* formatstr, ...)
{
	char res[VARIABLE_LIST_BUFFER_SIZE];
	va_list list;
	va_start(list, formatstr);
#if __STDC_SECURE_LIB__ //_MSC_VER < 1400
	vsprintf_s(res, VARIABLE_LIST_BUFFER_SIZE, formatstr, list);
#else
    vsprintf(res, formatstr, list);    
#endif
	va_end(list);

    Text = res;
}

LogException& LogException::operator=(const LogException& ex)
{
	Text = ex.Text;
	return *this;
}

const char* LogException::what() const throw()
{
	static std::string s ="[LogException] " + Text;
	return s.c_str();
}


//////////////////////////////////////////////////////////////////////
// TLogEngineParams
//////////////////////////////////////////////////////////////////////

TLogEngine::LogEngineProperties::LogEngineProperties()
{
	Properties props;
	Fill(props);
}

TLogEngine::LogEngineProperties::LogEngineProperties(const Properties& Props)
{
	Fill(Props);
}

TLogBackupType TLogEngine::LogEngineProperties::GetBackupType(const Properties& Props, const TLogBackupType defaultValue)
{
	std::string value = Properties::trim(Props.getString("BackupType"));

	if(EqualNCase(value, "Single"))
		return lbSingle;

	if(EqualNCase(value, "None"))
		return lbNone;

	if(EqualNCase(value, "Timestamp"))
		return lbTimeStamp;

	return defaultValue;
}

void TLogEngine::LogEngineProperties::Fill(const Properties& Props)
{
	BackupType = GetBackupType(Props, DefaultBackupType);

	// if FileName is not specified then construct from ApplicationName
	FileName = Props.getString("LogFileName");
	if (FileName.empty())
		FileName = Props.getString("ApplicationName", DefaultLogFileName) + LogExt;

	ApplicationName = Props.getString("ApplicationName", DefaultAppName);
	Version         = Props.getString("Version", "0.0.0.0");
	MaxLogSize      = Props.getUInt("MaxLogSize", DefaultMaxLogSize); // if MaxLogSize=0 it means "no limit on log file size".
	//MaxLogSize == 0 ? MaxLogSize = DefaultMaxLogSize : MaxLogSize = MaxLogSize;

	//DetailLevel = Props.getUInt("DebugLevel", DefaultDetailLevel);
	DetailLevel = Props.getUInt("DetailLevel", DefaultDetailLevel /*DetailLevel*/);
	Threaded    = Props.getBool("Threaded", false);

	SeparatorLine.ProcessPattern(Props.getString("SeparatorLine", DefaultSeparatorLine));
	ErrorLine.ProcessPattern    (Props.getString("ErrorLine"    , DefaultLinePattern));
	WarningLine.ProcessPattern  (Props.getString("WarningLine"  , DefaultLinePattern));
	InfoLine.ProcessPattern     (Props.getString("InfoLine"     , DefaultLinePattern));
	StartAppLine.ProcessPattern (Props.getString("StartAppLine" , DefaultStartAppLine));
	StopAppLine.ProcessPattern  (Props.getString("StopAppLine"  , DefaultStopAppLine));
	//LongExceptionLine  = "Exception '%E%' in %CLASSNAME% [ %CLASS% ].";
	//ShortExceptionLine = "Exception '%E%'";
}


//////////////////////////////////////////////////////////////////////
// TLogEngine
//////////////////////////////////////////////////////////////////////

TLogEngine::TLogEngine(void): LogQueue()
{ 
	FLogStream    = nullptr;
	FStarted	  = false;
	FBytesWritten = 0;
	FMessageCount[lmError]   = 0;
	FMessageCount[lmWarning] = 0;
	FMessageCount[lmInfo]    = 0;

	hThread = THREAD_TYPE_INITIALIZER;

	INIT_CRITICAL_SECTION(CriticalSection);
}

TLogEngine::TLogEngine(const Properties& Props): TLogEngine()//, LogQueue()
{
	FProperties.Fill(Props);
	/*FLogStream = NULL;
	FStarted = false;
	FBytesWritten = 0;
	FMessageCount[lmError] = 0;
	FMessageCount[lmWarning] = 0;
	FMessageCount[lmInfo] = 0;

	hThread = THREAD_TYPE_INITIALIZER;

	INIT_CRITICAL_SECTION(CriticalSection);*/
}

TLogEngine::TLogEngine(const std::string& ConfigFileName): TLogEngine() // : LogQueue()
{
	Properties Props;
	std::ifstream fin(ConfigFileName.c_str());

    if(fin.fail())
    {           
        throw LogException("Cannot open config file: %s", ConfigFileName.c_str());
    }

	Props.load(fin);
	FProperties.Fill(Props);
	
/*	if(argv != NULL)
		params.ProgramName = ExtractFileName(argv[0]);
	else
		params.ProgramName = ExtractFileName(FileName);

	if(MaxLogSize > 0)
		params.MaxLogSize = MaxLogSize;
	else 
		params.MaxLogSize = DefaultMaxLogSize;
*/
	/*FLogStream = NULL;
	FStarted = false;
	FBytesWritten = 0;
	FMessageCount[lmError] = 0;
	FMessageCount[lmWarning] = 0;
	FMessageCount[lmInfo] = 0;
	
	hThread = THREAD_TYPE_INITIALIZER;

	INIT_CRITICAL_SECTION(CriticalSection);*/
}

TLogEngine::~TLogEngine()
{
	if(FStarted)
		Stop();

	if(FProperties.Threaded)
	{
#ifdef WIN32
		DWORD exitCode;
		if(GetExitCodeThread(hThread, &exitCode) == STILL_ACTIVE) // wait only when thread is running
			WaitForSingleObject(hThread, INFINITE);
#else
//#ifdef HAVE_PTHREAD_H
		pthread_join(hThread, NULL);
#endif
	}
	
	//	LEAVE_CRITICAL_SECTION(CriticalSection);
	DELETE_CRITICAL_SECTION(CriticalSection);
}

void TLogEngine::InitThread(void)
{
	if(!FProperties.Threaded)
		return;

	LogEngineThreadInfo *info = new LogEngineThreadInfo;
	info->LogQueue = &LogQueue;
	info->LogEngine = this; // <<<< TODO: is this correct?

#ifdef WIN32
	unsigned long threadID;
	hThread = CreateThread(nullptr, 0, ThreadProc, info, 0, &threadID); // <<<< TODO: right param?
#else 
//#ifdef HAVE_PTHREAD_H
	if (pthread_create(&hThread, nullptr, ThreadProc, info) != 0)
	{
		// TODO: exception on failure?
	}
#endif
}

THREAD_OUT_TYPE THREAD_CALL_CONVENTION TLogEngine::ThreadProc(void *parameter)
{
	LogEngineThreadInfo *info = (LogEngineThreadInfo *) parameter;

	LogEvent* current_msg;
	do
	{
		current_msg = info->LogQueue->WaitForElement();
		if(current_msg)
		{
			//OutputDebugString("new event");

			LogEvent* event = current_msg;
			info->LogEngine->WriteEvent(event);
			delete event;
		}
		
	} while (current_msg);
	
	delete info;
	
	return 0;
}

void TLogEngine::Start(void)
{
	if(FStarted)
		return;
	
//	ENTER_CRITICAL_SECTION(CriticalSection);
	
	FLogStream = new TFileStream(FProperties.FileName, fmWrite);
	FLogStream->Seek(0, smFromEnd);
	FStarted = true;

	InitThread();

	WriteStart();
	
//	LEAVE_CRITICAL_SECTION(CriticalSection);
}

void TLogEngine::Stop(void)
{
	if(!FStarted)
		return;
	
//	ENTER_CRITICAL_SECTION(CriticalSection);

	if(FProperties.Threaded) 
	{
		LogQueue.PushElement(nullptr);
#ifdef WIN32
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
#else
//#ifdef HAVE_PTHREAD_H
		if (pthread_join(hThread, NULL))
		{
			// TODO: exception on error?
		}
#endif
	}

	WriteStop();

	delete FLogStream;
	FLogStream = nullptr;
	FStarted = false;

//	LEAVE_CRITICAL_SECTION(CriticalSection);
}

void TLogEngine::InternalWrite(const std::string& msg)
{
	ENTER_CRITICAL_SECTION(CriticalSection);

	if(!FStarted)
		throw LogException("The LogEngine is not started!");

	// TODO �� ������ Write �� �������� API �-��� fstat() ��� ��������� ������� �����. �������� ��� ��� ������� ����� �������� ��� ������� � �������.
	if(FProperties.MaxLogSize > 0 && FLogStream->Length() > FProperties.MaxLogSize*1024)
		TruncLogFile();

	//FBytesWritten += msg.size();

	FBytesWritten += FLogStream->WriteString(msg);
	FBytesWritten += FLogStream->WriteCRLF();
	//FLogStream->Flush();

	LEAVE_CRITICAL_SECTION(CriticalSection);
}

void TLogEngine::WriteEvent(LogEvent* event)
{
	if(event->m_detailLevel > FProperties.DetailLevel)
		return;
	
	switch(event->m_msgType)
	{
		case lmNone:
			InternalWrite(event->m_message);
			FMessageCount[lmNone]++;
			break;
		case lmInfo:
			InternalWrite(LogTypeChars[lmInfo] + FProperties.InfoLine.format(*event));
			FMessageCount[lmInfo]++;
			break;
		case lmWarning:
			InternalWrite(LogTypeChars[lmWarning] + FProperties.InfoLine.format(*event));
			FMessageCount[lmWarning]++;
			break;
		case lmError:
			InternalWrite(LogTypeChars[lmError] + FProperties.InfoLine.format(*event));
			FMessageCount[lmError]++;
			break;
		default:
			InternalWrite("UNRECOGNIZED MESSAGE TYPE " + event->m_message);
			FMessageCount[lmNone]++;
			break;
	}	
}

std::string TLogEngine::FormatStr(const std::string& str, uint /*DetailLevel = 0*/)
{
	return str;
}

std::string TLogEngine::FormatInfo(const std::string& str, uint DetailLevel /*= 0*/)
{
	LogEvent event(str, lmInfo, GET_THREAD_ID(), GetCurrTime(), DetailLevel, this);
	
	return LogTypeChars[lmInfo] + FProperties.InfoLine.format(event);
}

std::string TLogEngine::FormatWarning(const std::string& str, uint DetailLevel /*= 0*/)
{
	LogEvent event(str, lmWarning, GET_THREAD_ID(), GetCurrTime(), DetailLevel, this);
	
	return LogTypeChars[lmWarning] + FProperties.WarningLine.format(event);
}

std::string TLogEngine::FormatError(const std::string& str, uint DetailLevel /*= 0*/)
{
	LogEvent event(str, lmError, GET_THREAD_ID(), GetCurrTime(), DetailLevel, this);
	
	return LogTypeChars[lmError] + FProperties.ErrorLine.format(event);
}

void TLogEngine::WriteStr(const std::string& str, uint DetailLevel /*=0*/)
{
	if(DetailLevel > FProperties.DetailLevel)
		return;

	if(FProperties.Threaded) 
	{	
		//timeb tm;
		//ftime(&tm);
		LogEvent* event = new LogEvent(str, lmNone, GET_THREAD_ID(), GetCurrTime(), DetailLevel, this);
		
		LogQueue.PushElement(event);
	}
	else
	{		
		InternalWrite(LogTypeChars[lmNone] + str);
		FMessageCount[lmNone]++;
	}
}

void TLogEngine::WriteInfo(const std::string& str, uint DetailLevel /*=0*/)
{
	if(DetailLevel > FProperties.DetailLevel)
		return;

	if(FProperties.Threaded) 
	{	
		//timeb tm;
		//ftime(&tm);
		LogEvent* event = new LogEvent(str, lmInfo, GET_THREAD_ID(), GetCurrTime(), DetailLevel, this);
		
		LogQueue.PushElement(event);
	}
	else
	{
/*		timeb tm;
		ftime(&tm);
		LogEvent event(str, lmInfo, GET_THREAD_ID(), tm, DetailLevel);
	*/
		InternalWrite(FormatInfo(str, DetailLevel));
		FMessageCount[lmInfo]++;
	}
}

void TLogEngine::WriteWarning(const std::string& str, uint DetailLevel /*=0*/)
{
	if(DetailLevel > FProperties.DetailLevel)
		return;

	if(FProperties.Threaded) 
	{	
		LogEvent* event = new LogEvent(str, lmWarning, GET_THREAD_ID(), GetCurrTime(), DetailLevel, this);
		
		LogQueue.PushElement(event);
	}
	else
	{		
/*		timeb tm;
		ftime(&tm);
		LogEvent event(str, lmWarning, GET_THREAD_ID(), tm, DetailLevel);
*/		
		InternalWrite(FormatWarning(str, DetailLevel));
		FMessageCount[lmWarning]++;
	}
}

void TLogEngine::WriteError(const std::string& str, uint DetailLevel /*=0*/)
{
	if(DetailLevel > FProperties.DetailLevel)
		return;

	if(FProperties.Threaded) 
	{	
		//timeb tm;
		//ftime(&tm);
		LogEvent* event = new LogEvent(str, lmError, GET_THREAD_ID(), GetCurrTime(), DetailLevel, this);
		
		LogQueue.PushElement(event);
	}
	else
	{
/*		timeb tm;
		ftime(&tm);
		LogEvent event(str, lmError, GET_THREAD_ID(), tm, DetailLevel);
*/		
		InternalWrite(FormatError(str, DetailLevel));
		FMessageCount[lmError]++;
	}
}

void TLogEngine::WriteStrFmt(uint DetailLevel, const char* formatstr, ...)
{
	if(DetailLevel > FProperties.DetailLevel)
		return;

	char res[VARIABLE_LIST_BUFFER_SIZE];
	va_list list;
	va_start(list, formatstr);
#if __STDC_SECURE_LIB__ //_MSC_VER < 1400
	vsprintf_s(res, VARIABLE_LIST_BUFFER_SIZE, formatstr, list);
#else
    vsprintf(res, formatstr, list);
    
#endif
	va_end(list);
	
	WriteStr(res, DetailLevel);
}

void TLogEngine::WriteInfoFmt(uint DetailLevel, const char* formatstr, ...)
{
	if(DetailLevel > FProperties.DetailLevel)
		return;

	char res[VARIABLE_LIST_BUFFER_SIZE];
	va_list list;
	va_start(list, formatstr);

#if __STDC_SECURE_LIB__ //_MSC_VER < 1400
	vsprintf_s(res, VARIABLE_LIST_BUFFER_SIZE, formatstr, list);
#else
    vsprintf(res, formatstr, list);    
#endif

	va_end(list);
	
	WriteInfo(res, DetailLevel);
}

void TLogEngine::WriteWarningFmt(uint DetailLevel, const char* formatstr, ...)
{
	if(DetailLevel > FProperties.DetailLevel)
		return;

	char res[VARIABLE_LIST_BUFFER_SIZE];
	va_list list;
	va_start(list, formatstr);

#if __STDC_SECURE_LIB__ //_MSC_VER < 1400
	vsprintf_s(res, VARIABLE_LIST_BUFFER_SIZE, formatstr, list);
#else
    vsprintf(res, formatstr, list);    
#endif

	va_end(list);
	
	WriteWarning(res, DetailLevel);
}

void TLogEngine::WriteErrorFmt(uint DetailLevel, const char* formatstr, ...)
{
	if(DetailLevel > FProperties.DetailLevel)
		return;

	char res[VARIABLE_LIST_BUFFER_SIZE];
	va_list list;
	va_start(list, formatstr);

#if __STDC_SECURE_LIB__ //_MSC_VER < 1400
    vsprintf_s(res, VARIABLE_LIST_BUFFER_SIZE, formatstr, list);
#else
    vsprintf(res, formatstr, list);    
#endif

	va_end(list);
	
	WriteError(res, DetailLevel);
}

void TLogEngine::WriteStart()
{
	LogEvent event("", lmNone, GET_THREAD_ID(), GetCurrTime(), 0, this);
	
	InternalWrite(FProperties.StartAppLine.format(event));
}

void TLogEngine::WriteStop()
{
	LogEvent event("", lmNone, GET_THREAD_ID(), GetCurrTime(), 0, this);
	
	InternalWrite(FProperties.StopAppLine.format(event));
}

void TLogEngine::SetVersionInfo(const std::string& VerInfo)
{
	FProperties.Version = VerInfo;
}

void TLogEngine::SetAppName(const std::string& AppName)
{
	FProperties.ApplicationName = AppName;
}

void TLogEngine::SetMaxLogSize(const uint MaxLogSize)
{
	// if new MaxLogSize is less than current filesize - do truncate.
	if (MaxLogSize > 0 && FLogStream->Length() > MaxLogSize * 1024)
		TruncLogFile();

	FProperties.MaxLogSize = MaxLogSize;
}

void TLogEngine::SetLogDetailLevel(const uint DetailLevel)
{
	FProperties.DetailLevel = DetailLevel;
}

void TLogEngine::SetBackupType(const TLogBackupType BackupType)
{
	FProperties.BackupType = BackupType;
}

std::string TLogEngine::GenerateBackupName(void)
{
	std::string s;
	switch(FProperties.BackupType)
	{
		case lbTimeStamp:
			s = StripFileExt(FProperties.FileName) + "(" + FormatCurrDateTime("%d-%m-%Y %H.%M.%S") + ")" + LogExt;
			break;
		case lbSingle:
			s = StripFileExt(FProperties.FileName) + LogExt + BackupExt;
			break;
		default:
			throw LogException("Wrong Backup Type.");
	}

	return s;
}

void TLogEngine::TruncLogFile(void)
{
	ENTER_CRITICAL_SECTION(CriticalSection);

	delete FLogStream;
	FLogStream = nullptr;

	if (FProperties.BackupType == lbNone)
	{
		remove(FProperties.FileName.c_str()); // for lbNone we remove existing log file and start it from beginning
	}
	else
	{
		std::string newName = GenerateBackupName();
		remove(newName.c_str());
		rename(FProperties.FileName.c_str(), newName.c_str());
	}

	FLogStream = new TFileStream(FProperties.FileName, fmWrite);

	LEAVE_CRITICAL_SECTION(CriticalSection);
}

void TLogEngine::Flush()
{
	//Add here wait while other thread purges all data tot he disk and the call FLogStream->Flush
	FLogStream->Flush();
}

// Default value of instance of LogEngine
TLogEngine* TLogEngine::loginstance = nullptr;

void TLogEngine::InitLogEngine(const std::string& ConfigFileName)
{
	if(loginstance)
	{
		delete loginstance;	
	}

	loginstance = new TLogEngine(ConfigFileName);
	loginstance->Start();
}

void TLogEngine::InitLogEngine(const Properties& Props)
{
	if(loginstance)
	{
		delete loginstance;	
	}

	loginstance = new TLogEngine(Props);
	loginstance->Start();
}

void TLogEngine::InitLogEngine()
{
	if(loginstance)
	{
		delete loginstance;	
	}
	
	loginstance = new TLogEngine();
	loginstance->Start();
}

void TLogEngine::CloseLogEngine()
{
	if(loginstance)
	{
		delete loginstance;	
		loginstance = nullptr;
	}
}

TLogEngine* TLogEngine::getInstance(const Properties& Props)
{
	if(loginstance == nullptr)
	{
		loginstance = new TLogEngine(Props);
		loginstance->Start();
	}

	return loginstance;
}


TLogEngine* TLogEngine::getInstance()
{
	if(nullptr == loginstance)
	{
		loginstance = new TLogEngine();
		loginstance->Start();
	}

	return loginstance;
}

/************************************************************************/
/* Global Functions                                                     */
/************************************************************************/

TLogEngine* getLogEngine(void)
{
	return TLogEngine::getInstance();
}

TLogEngine* getLogEngine(const Properties& Props)
{
	return TLogEngine::getInstance(Props);
}

void InitLogEngine(const Properties& Props)
{
	TLogEngine::InitLogEngine(Props);
}

void InitLogEngine(const std::string& ConfigFileName)
{
	TLogEngine::InitLogEngine(ConfigFileName);
}

void InitLogEngine(void)
{
	TLogEngine::InitLogEngine();
}

void CloseLogEngine(void)
{
	TLogEngine::CloseLogEngine();
}
