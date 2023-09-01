/*
 * SynchronizedQueue.h
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef SYNCHRONIZED_QUEUE_H
#define SYNCHRONIZED_QUEUE_H

#include "threads.h"
#include "DynamicArrays.h"

template<class T>
class SynchronizedQueue : private THArray<T>
{
	MUTEX_TYPE ListMutex;

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

	//T	 WaitForElement();
    //template<class T>
    T WaitForElement()
    {
#ifdef WIN32
        WaitForSingleObject(ListSemaphore, INFINITE);
        ENTER_CRITICAL_SECTION(ListMutex);
//#endif
#else
//#ifdef HAVE_PTHREAD_H
        ENTER_CRITICAL_SECTION(ListMutex);
        while (ListSemaphoreValue == 0)
            pthread_cond_wait(&ListSemaphore, &ListMutex);

        ListSemaphoreValue--;
#endif

        T out = this->GetValue((const int)0);//front();
        this->DeleteValue(0);//pop_front();
        LEAVE_CRITICAL_SECTION(ListMutex);

        return out;
    }
	void PushElement(T in_element);
};



template<class T>
SynchronizedQueue<T>::SynchronizedQueue()
{
    INIT_CRITICAL_SECTION(ListMutex);
#ifdef WIN32
    ListSemaphore = CreateSemaphore(NULL, 0, MAXLONG, NULL);
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

    DELETE_CRITICAL_SECTION(ListMutex);
}


template<class T>
void SynchronizedQueue<T>::PushElement(T in_element)
{
    ENTER_CRITICAL_SECTION(ListMutex);
    this->AddValue(in_element);//push_back(in_element);

#ifdef WIN32
    ReleaseSemaphore(ListSemaphore, 1, NULL);
//#endif 
#else
//#ifdef HAVE_PTHREAD_H
    ListSemaphoreValue++;
    pthread_cond_signal(&ListSemaphore);
#endif
    LEAVE_CRITICAL_SECTION(ListMutex);
}

#endif /* SYNCHRONIZED_QUEUE_H */

