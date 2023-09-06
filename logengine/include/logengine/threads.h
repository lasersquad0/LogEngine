/*
 * threads.h
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef _LOGENGINE_THREADS_
#define _LOGENGINE_THREADS_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef WIN32
#include <windows.h>

#define INIT_CRITICAL_SECTION(CritSec)   InitializeCriticalSection(&CritSec)
#define ENTER_CRITICAL_SECTION(CritSec)	 EnterCriticalSection (&CritSec)
#define LEAVE_CRITICAL_SECTION(CritSec)  LeaveCriticalSection (&CritSec)
#define DELETE_CRITICAL_SECTION(CritSec) DeleteCriticalSection(&CritSec)

#define GET_THREAD_ID() GetCurrentThreadId() 
//std::hash<std::thread::id> hasher, size_t val=hasher(std::this_thread::get_id()))


 
#define THREAD_OUT_TYPE unsigned long

#define MUTEX_TYPE CRITICAL_SECTION
#define THREAD_TYPE HANDLE
#define THREAD_CALL_CONVENTION __stdcall

#define THREAD_TYPE_INITIALIZER NULL

#else  // if WIN32 is not defined then it is Linux.

//#ifdef HAVE_PTHREAD_H
#include <pthread.h>

#define INIT_CRITICAL_SECTION(CritSec)   CritSec = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP //pthread_mutex_init(&CritSec, nullptr)
#define ENTER_CRITICAL_SECTION(CritSec)	 pthread_mutex_lock(&CritSec)
#define LEAVE_CRITICAL_SECTION(CritSec)  pthread_mutex_unlock(&CritSec)
#define DELETE_CRITICAL_SECTION(CritSec) pthread_mutex_destroy(&CritSec)

#define GET_THREAD_ID() pthread_self()
#define THREAD_OUT_TYPE void*

#define MUTEX_TYPE pthread_mutex_t
#define THREAD_TYPE pthread_t
#define THREAD_CALL_CONVENTION

#define THREAD_TYPE_INITIALIZER pthread_self()

#endif

#ifndef GET_THREAD_ID
#error Neither Win32 nor POSIX threading model is available
#endif

#endif /* LOGENGINE_THREADS_H_INCLUDED */
