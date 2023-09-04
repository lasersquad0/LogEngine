
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "Shared.h"
#include "functions.h"
#include "LogEngine.h"
#include "testThreadLog.h"
//#include "debug_support.h"
#include "threads.h"

CPPUNIT_TEST_SUITE_REGISTRATION( LogEngineThreadLogTest );

void LogEngineThreadLogTest::setUp ()
{

}

void LogEngineThreadLogTest::tearDown ()
{
    // free memory allocated in setUp, do other things
}

struct ThreadInfoStruct
{
    TLogEngine *log;
    bool       begin;
};

THREAD_OUT_TYPE THREAD_CALL_CONVENTION testThreadProc(void* param)
{
	ThreadInfoStruct *info = (ThreadInfoStruct*) param;
	TLogEngine *log = info->log;

	if(!log)
	    return 0; // <<< unused, so 0. (void*) 1 looks silly =)

	while (!info->begin)
#ifdef WIN32
	    Sleep(10);
#endif

#ifdef __unix__
  	    usleep(10 * 1000);
#endif

	std::string s = IntToStr((int) GET_THREAD_ID());
	log->WriteError  (s + " is executing 1!");
	log->WriteInfo   (s + " is executing 2!");
	log->WriteStr    (s + " is executing 3!");
	log->WriteWarning(s + " is executing 4!");

	log->WriteErrorFmt  (0, "%d is executing %s!", GET_THREAD_ID(), "11");
	log->WriteInfoFmt   (0, "%d is executing %s!", GET_THREAD_ID(), "22");
	log->WriteStrFmt    (0, "%d is executing %s!", GET_THREAD_ID(), "33");
	log->WriteWarningFmt(0, "%d is executing %s!", GET_THREAD_ID(), "44");

	return 0;
}

#define nthreads 10

void LogEngineThreadLogTest::testCallLogFromManyThreads()
{
	//printf("testLogManyThreads ....");

	Properties prop;
	prop.SetValue("LogFileName", "ThreadsLog.log");
	InitLogEngine(prop);

	TLogEngine* log = getLogEngine();
	
	log->Start();
	log->WriteInfo("\nbegin creating threads");
	
	std::vector<THREAD_TYPE> handles;
	std::vector<unsigned long> ids;

	unsigned long thrID = 0;
	THREAD_TYPE hThread;

	ThreadInfoStruct info = { log, false };
	int i;

	for(i = 0; i < nthreads; i++)
	{
		std::string s = "Creating thread ";
		log->WriteInfo(s);

#ifdef WIN32
		hThread = CreateThread(nullptr, 0, testThreadProc, &info, 0, &thrID);
#else
//#ifdef HAVE_PTHREAD_H
		pthread_create(&hThread, nullptr, testThreadProc, &info);
#endif
		
		handles.insert(handles.end(), hThread);
		ids.insert(ids.end(), thrID);

		log->WriteInfoFmt(0, "Created thread #%d", thrID);
	}

	log->WriteInfo("all threads created");

	log->WriteInfo("begin resuming");
	
	info.begin = true;
	/*

	for( i = 0; i < nthreads; i++)
	{
		log->WriteInfoFmt(0, "resuming thread with id=%d", ids[i]);
		ResumeThread(handles[i]);
		}*/

	log->WriteInfo("all threads resumed");

#ifdef WIN32
	if(WaitForMultipleObjects(nthreads, &(*handles.begin()), TRUE, INFINITE) == WAIT_FAILED)
	{
		log->WriteError("WaitForMultipleObjects FAILED!");
	}
#else
//#ifdef HAVE_PTHREAD_H
	std::vector<THREAD_TYPE>::iterator j;
	for (j = handles.begin(); j != handles.end(); j++)
	    pthread_join(*j, nullptr);
#endif
	
	CloseLogEngine();

	//printf("PASSED\n");
}


void LogEngineThreadLogTest::testThreadLog1()
{
	//printf("testThreadLog1 ...");

	Properties prop;
	prop.SetValue("LogFileName", "ThreadedLog.log");
	prop.SetValue("Threaded", "trUe");
	InitLogEngine(prop);

	TLogEngine* log = getLogEngine();

	log->WriteError("threaded error");
	log->WriteInfo("threaded info");
	log->WriteStr("threaded str");
	log->WriteWarning("threaded warning");
	LogEvent event;

	CloseLogEngine();

	//printf("PASSED\n");
}
