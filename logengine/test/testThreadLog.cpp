
#include <chrono>
#include <thread>
#include "Shared.h"
#include "functions.h"
#include "LogEngine.h"
#include "testThreadLog.h"

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
    bool begin;
};

int testThreadProc(void* param)
{
	ThreadInfoStruct *info = (ThreadInfoStruct*) param;
	TLogEngine *log = info->log;

	if(!log)
	    return 0; // <<< unused, so 0. (void*) 1 looks silly =)

	while (!info->begin)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	std::string s = IntToStr(GET_THREAD_ID());
	log->WriteError  (s + " is executing 1.");
	log->WriteInfo   (s + " is executing 2.");
	log->WriteStr    (s + " is executing 3.");
	log->WriteWarning(s + " is executing 4.");

	log->WriteErrorFmt  (0, "%u is executing %s.", GET_THREAD_ID(), "11");
	log->WriteInfoFmt   (0, "%u is executing %s.", GET_THREAD_ID(), "22");
	log->WriteStrFmt    (0, "%u is executing %s.", GET_THREAD_ID(), "33");
	log->WriteWarningFmt(0, "%u is executing %s.", GET_THREAD_ID(), "44");

	return 0;
}

#define nthreads 10

void LogEngineThreadLogTest::testCallLogFromManyThreads()
{
	Properties prop;
	prop.SetValue("LogFileName", LOG_FILES_FOLDER "ThreadsLog.log");
	prop.SetValue("ApplicationName", "testCallLogFromManyThreads");
	InitLogEngine(prop);

	TLogEngine* log = TLogEngine::getInstance();
	
	log->Start();
	log->WriteInfo("begin creating threads");
	
	THArray<std::thread*> threads;
	//THArray<unsigned long> ids;

	//std::vector<THREAD_TYPE> handles;
	//std::vector<unsigned long> ids;

	//unsigned long thrID = 0;
	//THREAD_TYPE hThread;

	ThreadInfoStruct info = { log, false };

	for(uint i = 0; i < nthreads; i++)
	{
		std::string s = "Creating thread ";
		log->WriteInfo(s);

		std::thread* thr = new std::thread(testThreadProc, &info);
//#ifdef WIN32
//		hThread = CreateThread(nullptr, 0, testThreadProc, &info, 0, &thrID);
//#else
//#ifdef HAVE_PTHREAD_H
//		pthread_create(&hThread, nullptr, testThreadProc, &info);
//#endif
		
		//handles.insert(handles.end(), hThread);
		//ids.insert(ids.end(), thrID);
		threads.AddValue(thr);
		//ids.AddValue(thrID);

		log->WriteInfoFmt(0, "Created thread #%u", thr->get_id());
	}

	log->WriteInfo("all threads created");

	log->WriteInfo("begin resuming");
	
	info.begin = true; //TODO small data race here, consider use atomics
	
	log->WriteInfo("all threads resumed");

	// waiting till all threads finished
	for (uint i = 0; i < threads.Count(); i++)
	{
		threads[i]->join();
	}

	//freeing memory
	for (uint i = 0; i < threads.Count(); i++)
	{
		delete threads[i];
	}

//#ifdef WIN32
	//if(WaitForMultipleObjects(nthreads, &(*handles.begin()), TRUE, INFINITE) == WAIT_FAILED)
//	if (WaitForMultipleObjects(nthreads, handles.Memory(), TRUE, INFINITE) == WAIT_FAILED)
//	{
//		log->WriteError("WaitForMultipleObjects FAILED!");
//	}
//#else
//#ifdef HAVE_PTHREAD_H
//	std::vector<THREAD_TYPE>::iterator j;
	//for (j = handles.begin(); j != handles.end(); j++)
	//pthread_join(*j, nullptr);
//	for (size_t i = 0; i < handles.Count(); i++)
//	{
//		pthread_join(handles[i], nullptr);
//	}
	    
//#endif
	
	CloseLogEngine();
}


void LogEngineThreadLogTest::testThreadLog1()
{
	Properties prop;
	prop.SetValue("LogFileName", LOG_FILES_FOLDER "ThreadedLog.log");
	prop.SetValue("Threaded", "trUe");
	InitLogEngine(prop);

	TLogEngine* log = TLogEngine::getInstance();

	std::string str("main thread ID: "); 
	str += IntToStr((uint)GET_THREAD_ID());
	log->WriteInfo(str);
	log->WriteError("threaded error #1");
	log->WriteInfo("threaded info #2");
	log->WriteStr("threaded str #3");
	log->WriteWarning("threaded warning #4");

	CloseLogEngine();
}

void LogEngineThreadLogTest::testThreadLogMeasureTime1()
{
	std::string fileName = LOG_FILES_FOLDER "ThreadLogMeasureTime1.log";

	Properties prop;
	prop.SetValue("ApplicationName", "testThreadLogMeasureTime1");
	prop.SetValue("LogFileName", fileName);
	prop.SetValue("MaxLogSize", "0"); // to prevent creating log file backup copies, etc
	prop.SetValue("Threaded", "TRUE");

	remove(fileName.c_str());

	InitLogEngine(prop);
	TLogEngine* log = TLogEngine::getInstance();

	auto start1 = std::chrono::high_resolution_clock::now();
	
	const int NUM_LOGS = 10000;

	for (size_t i = 0; i < NUM_LOGS; i++)
	{
		log->WriteWarning("threaded warning ###" + std::to_string(i));
	}

	auto stop = std::chrono::high_resolution_clock::now();

	std::cout << " Excec time: " << millisecToStr(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start1).count());
	
	CloseLogEngine();

	std::fstream ff(fileName);
	if (!ff)
		throw IOException("Cannot open file for reading:" + fileName);

	std::string ln;
	std::getline(ff, ln);
	CPPUNIT_ASSERT_EQUAL((size_t)0, ln.size());
	
	std::getline(ff, ln);
	size_t n = ln.find("###");
	CPPUNIT_ASSERT_EQUAL(std::string::npos, n);
	
	std::getline(ff, ln);
	n = ln.find("###");
	CPPUNIT_ASSERT_EQUAL(std::string::npos, n);

	for (size_t i = 0; i < NUM_LOGS; i++)
	{
		std::getline(ff, ln);
		n = ln.find("###");
		std::string seq = ln.substr(n + 3);
		std::string si = std::to_string(i);
		CPPUNIT_ASSERT_EQUAL(seq, si);
	}
	
}

void LogEngineThreadLogTest::testNONThreadLogMeasureTime1()
{
	Properties prop;
	prop.SetValue("ApplicationName", "testNONThreadLogMeasureTime1");
	prop.SetValue("LogFileName", LOG_FILES_FOLDER "NONThreadLogMeasureTime1.log");
	prop.SetValue("MaxLogSize", "0"); // to prevent creating log file backup copies, etc
	prop.SetValue("Threaded", "false");

	InitLogEngine(prop);
	TLogEngine* log = TLogEngine::getInstance();

	auto start1 = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < 10000; i++)
	{
		log->WriteWarning("NON-threaded warning #" + std::to_string(i));
	}

	auto stop = std::chrono::high_resolution_clock::now();

	std::cout << " Excec time: " << millisecToStr(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start1).count());

	CloseLogEngine();
}

