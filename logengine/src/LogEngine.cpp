/*
 * LogEngine.cpp
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 * 
 * 
 */

#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <LogEngine.h>
#include <functions.h>
#include <LogEvent.h>

#ifndef WIN32 //HAVE_STDARG_H assume we have Linux here
#include <stdarg.h>
#include <pthread.h>
#endif

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

//const char LogTypeChars[5] = " #! ";

#define DefaultLogFileName    "NONAME"
#define DefaultAppName        "nonameapp"
#define DefaultAppVersion     "0.0.0.0"
#define DefaultLinePattern    TimeMacro " #" ThreadMacro ": " MessageMacro
#define DefaultErrorPattern   "!" DefaultLinePattern
#define DefaultWarningPattern "#" DefaultLinePattern
#define DefaultInfoPattern    " " DefaultLinePattern
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
#ifdef WIN32 // __STDC_SECURE_LIB__ //_MSC_VER < 1400
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
// LogEngineProperties
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

	// if LogFileName is not specified then construct from ApplicationName
	LogFileName = Props.getString("LogFileName");
	if (LogFileName.empty())
		LogFileName = Props.getString("ApplicationName", DefaultLogFileName) + LogExt;
	else
		if (ExtractFileName(LogFileName).find('.') == std::string::npos) // add an extension if absent
			LogFileName += LogExt; 

	ApplicationName = Props.getString("ApplicationName", DefaultAppName);
	Version         = Props.getString("Version", DefaultAppVersion);
	MaxLogSize      = Props.getUInt("MaxLogSize", DefaultMaxLogSize); // MaxLogSize is in kilobytes. if MaxLogSize=0 it means "no limit on log file size".
	//MaxLogSize == 0 ? MaxLogSize = DefaultMaxLogSize : MaxLogSize = MaxLogSize;

	//DetailLevel = Props.getUInt("DebugLevel", DefaultDetailLevel);
	DetailLevel = Props.getUInt("DetailLevel", DefaultDetailLevel /*DetailLevel*/);
	Threaded    = Props.getBool("Threaded", false);

	SeparatorLine.ProcessPattern(Props.getString("SeparatorLine", DefaultSeparatorLine));
	ErrorLine.ProcessPattern    (Props.getString("ErrorLine"    , DefaultErrorPattern));
	WarningLine.ProcessPattern  (Props.getString("WarningLine"  , DefaultWarningPattern));
	InfoLine.ProcessPattern     (Props.getString("InfoLine"     , DefaultInfoPattern));
	StartAppLine.ProcessPattern (Props.getString("StartAppLine" , DefaultStartAppLine));
	StopAppLine.ProcessPattern  (Props.getString("StopAppLine"  , DefaultStopAppLine));
	//LongExceptionLine  = "Exception '%E%' in %CLASSNAME% [ %CLASS% ].";
	//ShortExceptionLine = "Exception '%E%'";
}


//////////////////////////////////////////////////////////////////////
// TLogEngine
//////////////////////////////////////////////////////////////////////

void TLogEngine::initialInit()
{
	FLogStream = nullptr;
	FStarted = false;
	FFileBytesWritten = 0;
	FTotalBytesWritten = 0;
	FInitialFileSize = 0;
	FMessageCount[lmNone] = 0;
	FMessageCount[lmError] = 0;
	FMessageCount[lmWarning] = 0;
	FMessageCount[lmInfo] = 0;
}

TLogEngine::TLogEngine(void)//: LogQueue()
{ 
	initialInit();

	//hThread = THREAD_TYPE_INITIALIZER;
	//INIT_CRITICAL_SECTION(CriticalSection);
}

TLogEngine::TLogEngine(const Properties& Props): FProperties(Props) //, LogQueue()
{
	initialInit();
//	FProperties.Fill(Props);
}

TLogEngine::TLogEngine(const std::string& ConfigFileName)//: TLogEngine() // : LogQueue()
{
	initialInit();

	Properties Props;
	std::ifstream fin(ConfigFileName.c_str());

    if(fin.fail())
    {           
        throw LogException("Cannot open config file: %s", ConfigFileName.c_str());
    }

	Props.load(fin);
	FProperties.Fill(Props);
}

TLogEngine::~TLogEngine()
{
	if(FStarted)
		Stop();

//	if(FProperties.Threaded)
//	{
//#ifdef WIN32
//		DWORD exitCode;
//		if(GetExitCodeThread(hThread, &exitCode) == STILL_ACTIVE) // wait only when thread is running
//			WaitForSingleObject(hThread, INFINITE);
//#else
////#ifdef HAVE_PTHREAD_H
//		pthread_join(hThread, NULL);
//#endif
//	}
	
	//	LEAVE_CRITICAL_SECTION(CriticalSection);
	//DELETE_CRITICAL_SECTION(CriticalSection);

}

void TLogEngine::initThread(void)
{
	if(!FProperties.Threaded)
		return;

	LogEngineThreadInfo *info = new LogEngineThreadInfo;
	info->LogQueue = &LogQueue;
	info->LogEngine = this; 

	std::thread thr(ThreadProc, info);
	m_thread.swap(thr);
	std::stringstream s;
	s<< thr.get_id();
	unsigned long threadID;
	s >> threadID;

//#ifdef WIN32
//	unsigned long threadID;
//	hThread = CreateThread(nullptr, 0, ThreadProc, info, 0, &threadID); // <<<< TODO: right param?
//#else 
//#ifdef HAVE_PTHREAD_H
//	if (pthread_create(&hThread, nullptr, ThreadProc, info) != 0)
//	{
		// TODO: exception on failure?
//	}
//#endif
}

int TLogEngine::ThreadProc(void *parameter)
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
			info->LogEngine->writeEvent(event);
			delete event;
		}
		
	} while (current_msg);
	
	delete info;
	
	return 0;
}

void TLogEngine::resetStatistics()
{
	FFileBytesWritten        = 0;
	FTotalBytesWritten       = 0;
	FInitialFileSize         = 0;
	FMessageCount[lmNone]    = 0;
	FMessageCount[lmError]   = 0;
	FMessageCount[lmWarning] = 0;
	FMessageCount[lmInfo]    = 0;
}

void TLogEngine::Start(void)
{
	if(FStarted)
		return;
	
//	ENTER_CRITICAL_SECTION(CriticalSection);
	
	resetStatistics();

	FLogStream = new TFileStream(FProperties.LogFileName, fmWrite);
	FLogStream->Seek(0, smFromEnd);
	FInitialFileSize = FLogStream->Length();
	FStarted = true;

	initThread();

	writeStart();
	
//	LEAVE_CRITICAL_SECTION(CriticalSection);
}

void TLogEngine::Stop(void)
{
	if(!FStarted)
		return;

	if(FProperties.Threaded) 
	{
		LogQueue.PushElement(nullptr);
		m_thread.join(); // waiting till thread finishes

//#ifdef WIN32
//		WaitForSingleObject(hThread, INFINITE);
//		CloseHandle(hThread);
//#else
//#ifdef HAVE_PTHREAD_H
//		if (pthread_join(hThread, NULL))
//		{
			// TODO: exception on error?
//		}
//#endif
	}

	writeStop(); // writeStop and writeStart do not use threads at all
	
	delete FLogStream;
	FLogStream = nullptr;
	FStarted = false;
}

ulong TLogEngine::getFileLength()
{
	return FInitialFileSize + FFileBytesWritten;
}

void TLogEngine::internalWrite(const std::string& msg)
{
	//ENTER_CRITICAL_SECTION(CriticalSection);
	mutexguard lock(mtx);

	if(!FStarted)
		throw LogException("The LogEngine is not started!");

	if(FProperties.MaxLogSize > 0 && getFileLength() > FProperties.MaxLogSize * 1024)
		truncLogFile();

	int written = FLogStream->WriteString(msg);
	written += FLogStream->WriteCRLF();

	FFileBytesWritten += written;
	FTotalBytesWritten += written;
	//FLogStream->Flush();

	//LEAVE_CRITICAL_SECTION(CriticalSection);
}

void TLogEngine::writeEvent(LogEvent* event)
{
	if(event->m_detailLevel > FProperties.DetailLevel)
		return;
	
	switch(event->m_msgType)
	{
		case lmNone:
			internalWrite(event->m_message);
			FMessageCount[lmNone]++;
			break;
		case lmInfo:
			internalWrite(/*LogTypeChars[lmInfo] + */FProperties.InfoLine.format(*event));
			FMessageCount[lmInfo]++;
			break;
		case lmWarning:
			internalWrite(/*LogTypeChars[lmWarning] + */ FProperties.WarningLine.format(*event));
			FMessageCount[lmWarning]++;
			break;
		case lmError:
			internalWrite(/*LogTypeChars[lmError] + */ FProperties.ErrorLine.format(*event));
			FMessageCount[lmError]++;
			break;
		default:
			internalWrite("UNRECOGNIZED MESSAGE TYPE " + event->m_message);
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
	LogEvent event(str, lmInfo, GET_THREAD_ID(), GetCurrDateTime(), DetailLevel, this);
	
	return /*LogTypeChars[lmInfo] + */FProperties.InfoLine.format(event);
}

std::string TLogEngine::FormatWarning(const std::string& str, uint DetailLevel /*= 0*/)
{
	LogEvent event(str, lmWarning, GET_THREAD_ID(), GetCurrDateTime(), DetailLevel, this);
	
	return /*LogTypeChars[lmWarning] + */ FProperties.WarningLine.format(event);
}

std::string TLogEngine::FormatError(const std::string& str, uint DetailLevel /*= 0*/)
{
	LogEvent event(str, lmError, GET_THREAD_ID(), GetCurrDateTime(), DetailLevel, this);
	
	return /*LogTypeChars[lmError] + */FProperties.ErrorLine.format(event);
}

void TLogEngine::WriteStr(const std::string& str, uint DetailLevel /*=0*/)
{
	if(DetailLevel > FProperties.DetailLevel)
		return;

	if(FProperties.Threaded) 
	{	
		//timeb tm;
		//ftime(&tm);
		LogEvent* event = new LogEvent(str, lmNone, GET_THREAD_ID(), GetCurrDateTime(), DetailLevel, this);
		
		LogQueue.PushElement(event);
	}
	else
	{		
		internalWrite(/*LogTypeChars[lmNone] + */str);
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
		LogEvent* event = new LogEvent(str, lmInfo, GET_THREAD_ID(), GetCurrDateTime(), DetailLevel, this);
		
		LogQueue.PushElement(event);
	}
	else
	{
/*		timeb tm;
		ftime(&tm);
		LogEvent event(str, lmInfo, GET_THREAD_ID(), tm, DetailLevel);
	*/
		internalWrite(FormatInfo(str, DetailLevel));
		FMessageCount[lmInfo]++;
	}
}

void TLogEngine::WriteWarning(const std::string& str, uint DetailLevel /*=0*/)
{
	if(DetailLevel > FProperties.DetailLevel)
		return;

	if(FProperties.Threaded) 
	{	
		LogEvent* event = new LogEvent(str, lmWarning, GET_THREAD_ID(), GetCurrDateTime(), DetailLevel, this);
		
		LogQueue.PushElement(event);
	}
	else
	{		
/*		timeb tm;
		ftime(&tm);
		LogEvent event(str, lmWarning, GET_THREAD_ID(), tm, DetailLevel);
*/		
		internalWrite(FormatWarning(str, DetailLevel));
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
		LogEvent* event = new LogEvent(str, lmError, GET_THREAD_ID(), GetCurrDateTime(), DetailLevel, this);
		
		LogQueue.PushElement(event);
	}
	else
	{
		/*		timeb tm;
				ftime(&tm);
				LogEvent event(str, lmError, GET_THREAD_ID(), tm, DetailLevel);
		*/
		internalWrite(FormatError(str, DetailLevel));
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
#ifdef WIN32 // __STDC_SECURE_LIB__ //_MSC_VER < 1400
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

#ifdef WIN32 // __STDC_SECURE_LIB__ //_MSC_VER < 1400
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

#ifdef WIN32 // __STDC_SECURE_LIB__ //_MSC_VER < 1400
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

#ifdef WIN32 // __STDC_SECURE_LIB__ //_MSC_VER < 1400
    vsprintf_s(res, VARIABLE_LIST_BUFFER_SIZE, formatstr, list);
#else
    vsprintf(res, formatstr, list);    
#endif

	va_end(list);
	
	WriteError(res, DetailLevel);
}

void TLogEngine::writeStart()
{
	LogEvent event("", lmNone, GET_THREAD_ID(), GetCurrDateTime(), 0, this);
	
	internalWrite(FProperties.StartAppLine.format(event));
}

void TLogEngine::writeStop()
{
	LogEvent event("", lmNone, GET_THREAD_ID(), GetCurrDateTime(), 0, this);
	
	internalWrite(FProperties.StopAppLine.format(event));
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
	FLogStream->Flush();
	if (MaxLogSize > 0 && (uint)FLogStream->Length() > MaxLogSize * 1024)
		truncLogFile();

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

void TLogEngine::SetLogFileName(const std::string& newFileName)
{
	if (FStarted)
		throw LogException("Cannot change log filename when LogEngine is started. Stop LogEngne before changing log filename.");

	FProperties.LogFileName = newFileName;
}

std::string TLogEngine::generateBackupName(void)
{
	std::string s;
	switch(FProperties.BackupType)
	{
		case lbTimeStamp:
			s = StripFileExt(FProperties.LogFileName) + "(" + FormatCurrDateTime("%d-%m-%Y %H.%M.%S") + ")" + LogExt;
			break;
		case lbSingle:
			s = StripFileExt(FProperties.LogFileName) + LogExt + BackupExt;
			break;
		default:
			throw LogException("Wrong Backup Type.");
	}

	return s;
}

void TLogEngine::truncLogFile(void)
{
	//ENTER_CRITICAL_SECTION(CriticalSection);
	mutexguard lock(mtx);

	delete FLogStream;
	FLogStream = nullptr;

	if (FProperties.BackupType == lbNone)
	{
		remove(FProperties.LogFileName.c_str()); // for lbNone we remove existing log file and start it from beginning
	}
	else
	{
		std::string newName = generateBackupName();
		remove(newName.c_str());
		rename(FProperties.LogFileName.c_str(), newName.c_str());
	}

	FLogStream = new TFileStream(FProperties.LogFileName, fmWrite);
	FInitialFileSize = FLogStream->Length();
	FFileBytesWritten = 0;

	//LEAVE_CRITICAL_SECTION(CriticalSection);
}

void TLogEngine::Flush()
{
	//TODO Add here wait while other thread purges all data to the disk and then call FLogStream->Flush
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

//TLogEngine* getLogEngine(void)
//{
//	return TLogEngine::getInstance();
//}
//
//TLogEngine* getLogEngine(const Properties& Props)
//{
//	return TLogEngine::getInstance(Props);
//}

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
