#include "stdafx.h"
#include "qlock.h"

void CQLock::setFlag (LONG *ptheFlag) 
{
	HANDLE theHandle = (HANDLE)InterlockedCompareExchange (ptheFlag, -1, 0);
	if (theHandle) 
	{
		// Another thread has already stored an event handle in the flag.
		SetEvent(theHandle);
	}
} // setFlag

void CQLock::waitOnFlag (LONG *ptheFlag) 
{
	if (InterlockedExchangeAdd (ptheFlag, 0) == 0) 
	{
		// The flag is not set; create event.
		HANDLE theHandle = CreateEvent (NULL, FALSE, FALSE, NULL);
		if (InterlockedCompareExchange (ptheFlag, (LONG)theHandle, 0) == 0) 
		{
			// The event handle has been stored in the flag; wait on it now.
			WaitForSingleObject(theHandle, INFINITE);
		}
		CloseHandle (theHandle);
	}
} // waitOnFlag

CQLock::CQLock (CQLock::Mutex *ptheMutex) : m_ptheMutex (ptheMutex),
																						m_theNextFlag (0),
																						m_theReadyFlag (0),
																						m_ptheNext (0)
{
	// Queue for the lock.
	CQLock *pPred = NULL;
	pPred = (CQLock*)InterlockedExchange ((LPLONG)ptheMutex, (LONG)this);
	if (pPred) 
	{
		// The lock was not free; link behind predecessor.
		pPred->m_ptheNext = this;
		setFlag (&pPred->m_theNextFlag);
		waitOnFlag (&m_theReadyFlag);
	} // if 
} // CQLock

CQLock::~CQLock()
{
	if ((CQLock*)InterlockedCompareExchange ((LPLONG)m_ptheMutex, 0, (LONG)this) == this) 
	{
		// There is no successor; the lock is free now.
		return;
	}
	// Wait for successor.
	waitOnFlag (&m_theNextFlag);
	// Pass the lock.
	setFlag (&m_ptheNext->m_theReadyFlag);
} // ~CQLock

//Singleton& Singleton::instance () 
//{
//	static volatile LONG flag = 0;
//	if (InterlockedExchangeAdd(&flag, 0) == 0) 
//	{
//		static CQLock::Mutex mutex;
//		QLock lock (&mutex);
//		if (0 == InterlockedExchangeAdd (&flag, 0)) 
//		{
//			if (!s_singleton) 
//			{
//				init();
//				InterlockedExchange (&flag, 1);
//			} // if 
//		} // if 
//	} // if 
//	return *s_singleton;
//} // instance
