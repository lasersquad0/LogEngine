/*
 * SynchronizedQueue.h
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef _SYNCHRONIZED_QUEUE_H_
#define _SYNCHRONIZED_QUEUE_H_

#include <mutex>
#include <condition_variable>
#include "DynamicArrays.h"


template<class T>
class SafeQueue : private THArray<T>
{
private:
    std::mutex mtx;
    std::condition_variable cv;

public:
    T WaitForElement();
    void PushElement(T in_element);
};

template<class T>
T SafeQueue<T>::WaitForElement()
{
    std::unique_lock<std::mutex> lock(mtx);
    while (this->Count() == 0)
        cv.wait(lock);

//    T out = this->GetValue(0);
//    this->DeleteValue(0);

    return this->PopFront(); //out;
}

template<class T>
void SafeQueue<T>::PushElement(T in_element)
{
    std::lock_guard<std::mutex> lock(mtx);

    this->AddValue(in_element);
    cv.notify_one();
}

//-------------------------------------------------------------------
// Old implementation based on WinAPI and pthread calls
//-------------------------------------------------------------------

/*
template<class T>
class SynchronizedQueue : private THArray<T>
{
	//MUTEX_TYPE ListMutex;
    std::recursive_mutex mtx;
    
#ifdef WIN32
	HANDLE ListSemaphore;
//#endif
#else
//#ifdef HAVE_PTHREAD_H
	int            ListSemaphoreValue;
	pthread_cond_t ListSemaphore;
#endif

public:
	SynchronizedQueue();
	virtual ~SynchronizedQueue();

	T WaitForElement();
	void PushElement(T in_element);
};

template<class T>
T SynchronizedQueue<T>::WaitForElement()
{
#ifdef WIN32
    WaitForSingleObject(ListSemaphore, INFINITE);
    //ENTER_CRITICAL_SECTION(ListMutex);

    //#endif
#else
    //#ifdef HAVE_PTHREAD_H
    //ENTER_CRITICAL_SECTION(ListMutex);
    while (ListSemaphoreValue == 0)
        pthread_cond_wait(&ListSemaphore, &ListMutex);

    ListSemaphoreValue--;
#endif

    mutexguard lock(mtx);
    T out = this->GetValue((const int)0);//front();
    this->DeleteValue(0);//pop_front();
    //LEAVE_CRITICAL_SECTION(ListMutex);

    return out;
}

template<class T>
SynchronizedQueue<T>::SynchronizedQueue()
{
    //INIT_CRITICAL_SECTION(ListMutex);
#ifdef WIN32
    ListSemaphore = CreateSemaphore(nullptr, 0, MAXLONG, nullptr);
//#endif
#else
//#ifdef HAVE_PTHREAD_H
    pthread_cond_init(&ListSemaphore, NULL);
    ListSemaphoreValue = 0;
    this->SetCapacity(100);
#endif
}

template<class T>
SynchronizedQueue<T>::~SynchronizedQueue()
{
#ifdef WIN32
    CloseHandle(ListSemaphore);
//#endif
#else
//#ifdef HAVE_PTHREAD_H
    pthread_cond_destroy(&ListSemaphore);
#endif

   // DELETE_CRITICAL_SECTION(ListMutex);
}


template<class T>
void SynchronizedQueue<T>::PushElement(T in_element)
{
    //ENTER_CRITICAL_SECTION(ListMutex);
    mutexguard lock(mtx);
    this->AddValue(in_element);//push_back(in_element);

#ifdef WIN32
    ReleaseSemaphore(ListSemaphore, 1, nullptr);
//#endif 
#else
//#ifdef HAVE_PTHREAD_H
    ListSemaphoreValue++;
    pthread_cond_signal(&ListSemaphore);
#endif
    //LEAVE_CRITICAL_SECTION(ListMutex);
}
*/



#endif //_SYNCHRONIZED_QUEUE_H_

