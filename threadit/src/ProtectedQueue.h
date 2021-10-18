/**
 * Title: CProtectedQueue
 * Description: class CProtectedQueue is a template for a queue that uses 
 * critical sections for access to elements in the queue. 
 *
 * Copyright: Copyright (c) 2008 Ashkel Software 
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2008/12/19 02:38:06 $
 */

#ifndef _PROTECTED_QUEUE_H
#define _PROTECTED_QUEUE_H

// Include files
#include <windows.h>
#include <list>

/**
 * Class CProtectedQueue is a template class that implements a queue
 * for the specified type. Access to elements in the queue is bounded by
 * a critical section to support use in a multi-threaded environment.
 */
template <class T> class CProtectedQueue 
{
	// Attributes
private:
	/** List of class T */
	std::list<T*>	 m_theQueue;
	/** Concurrency control via a critical section. */
	CRITICAL_SECTION	m_csQAccess;	
	/** Handle that is signalled when an item is received. */
	HANDLE m_theSignal;

	// Constructors and destructors
public:
	/**
	 * Default constructor: It initializes the critical section and creates the 
	 * HANDLE for the Semaphore.
	 */
	CProtectedQueue (void);

	/**
	 * ~CProtectedQueue is the destructor for the instance and frees all resources
	 * in use.
	 */
	~CProtectedQueue (void);
	
	// Methods
public: 
	/**
	 * Method insertItem enters a critical section before adding an item to the tail of 
	 * the queue. After it leaves the critical section, it releases a semaphore to
	 * indicate that an item has been added to the queue.
	 * removes all messages from the queue.
	 * theItem is the item to be added to the queue.
	 */
	void insertItem (T* theItem);

	/**
	 * Method waitItem waits for a single object and then enters a critical section
	 * where it gets and then removes the first item in the queue if the 
	 * queue is not empty.
	 * rec is the item retreived from the queue.
	 * theWaitTime is the time(in millisecs) to wait for a semaphore.
	 * RET returns the success of the operation.
	 */
	T* waitItem (DWORD theWaitTime = 500); 

	/**
	 * Method GetItem returns a pointer to the first item in the queue.
	 * The method does not block but will return NULL if there is nothing
	 * in the queue. The internal queue semaphore count that is signalled when
	 * an item is available is decremented.
	 */
	T* getItem (void); 

	/**
	 * Method getItemNoDec returns a pointer to the first item in the queue.
	 * The method does not block but will return NULL if there is nothing
	 * in the queue. The method also does not decrement the signal count
	 * received for each item in the queue. This can be done by obtaining
	 * the queue semaphore and manipulating it directly.
	 */
	T* getItemNoDec (void); 
		
	/**
	 * Method getQSemaphore returns the semaphore associated with the queue.
	 */
	HANDLE getQSemaphore (void) const
;
		
	/** 
	 * Methd clear this method enters a critical section before it removes all messages 
	 * from the queue. The memory referenced by the elemets in the queue is not freed
	 * currently.
	 */
	void clear (void);
	 
	/**
	 * Method size enters a critical section before it the number of items in the queue.
	 */
	int size (void);

	/**
	 * Method isEmpty enters a critical section before it tests to see if the queue is empty.
	 */
	bool isEmpty (void);

}; // template <class T> class CProtectedQueue 


/**
 * Implementation of template <class T> class CProtectedQueue.
 */

/**
 * Constructor CProtectedQueue is the default constructor: It initializes the 
 * critical section and creates the HANDLE for the Semaphore.
 */
template <class T> CProtectedQueue<T>::CProtectedQueue ()
{
	::InitializeCriticalSection (&m_csQAccess);
	m_theSignal = CreateSemaphore (NULL, 0, LONG_MAX, NULL);
} // constructor CProtectedQueue

/**
 * Destructor ~CProtectedQueue releases all semaphores, closes the semaphore handle and 
 * removes all messages from the queue.
 */
template <class T> CProtectedQueue<T>::~CProtectedQueue ()
{
	clear ();
	CloseHandle (m_theSignal);
	::DeleteCriticalSection (&m_csQAccess);
} // destructor ~CProtectedQueue

/**
 * Method InsertItem enters a critical section before adding an item to the tail of 
 * the queue. After it leaves the critical section, it releases a semaphore to
 * indicate that an item has been added to the queue.
 * rec is theitem to be added to the queue.
 */
template <class T> void CProtectedQueue<T>::insertItem (T* theItem)
{
	::EnterCriticalSection (&m_csQAccess);
	m_theQueue.push_back (theItem);
	::LeaveCriticalSection (&m_csQAccess);
	ReleaseSemaphore (m_theSignal, 1, NULL);
} // insertItem

/**
 * Method WaitItem waits for a single object and then enters a critical section
 * where it gets and then removes the first item in the queue if the 
 * queue is not empty.
 * rec is the item retrieved from the queue.
 * WaitTime is the time(in milli secs) to wait for a semaphore.
 * RET returns the success of the operation.
 */
template <class T> T* CProtectedQueue<T>::waitItem (DWORD theWaitTime)
{
	T* theItem = NULL;
	DWORD theResult = 0;

	// wait for an item to arrive
	theResult = WaitForSingleObject (m_theSignal, theWaitTime);
	// First check if the state of the specified object is signalled.
	if (theResult == WAIT_OBJECT_0)
	{
		::EnterCriticalSection (&m_csQAccess);
		// check the queue - if there is an item, get it
		bool isEmpty = m_theQueue.empty ();
		if (!isEmpty)
		{
			theItem = m_theQueue.front ();
			m_theQueue.pop_front ();
		} // if 
		::LeaveCriticalSection (&m_csQAccess);
	} // if 
	// return the result
	return theItem;
} // waitItem

/**
 * Method getItem enters a critical section
 * where it gets and then removes the first item in the queue if the 
 * queue is not empty. If the queue is empty the method returns NULL.
 * rec is the item retrieved from the queue.
 */
template <class T> T* CProtectedQueue<T>::getItem (void)
{
  return waitItem(0);
} // getItem

/**
 * Method getItemNoDec returns a pointer to the first item in the queue.
 * The method does not block but will return NULL if there is nothing
 * in the queue. The method also does not decrement the signal count
 * received for each item in the queue. This can be done by obtaining
 * the queue semaphore and manipulating it directly.
 */
template <class T> T* CProtectedQueue<T>::getItemNoDec (void)
{
	T* theItem = NULL; 
	// wait for an item to arrive
	::EnterCriticalSection (&m_csQAccess);
	// check the queue - if there is an item, get it
	BOOL isEmpty = m_theQueue.empty ();
	if (!isEmpty)
	{
		theItem = m_theQueue.front ();
		m_theQueue.pop_front ();
	} // if 
	::LeaveCriticalSection (&m_csQAccess);
	// return the result
	return theItem;
} // getItemNoDec

/**
 * Method getQSemaphore returns the semaphore associated with the queue.
 */
template <class T> HANDLE CProtectedQueue<T>::getQSemaphore (void) const
{
	return m_theSignal;
} // getQSemaphore

/** 
 * Methd clear this method enters a critical section before it removes all messages 
 * from the queue. The memory referenced by the elements in the queue is not freed
 * currently.
 */
template <class T> void CProtectedQueue<T>::clear (void)
{
	BOOL isEmpty = FALSE;
	T* theItem = NULL; 

	::EnterCriticalSection (&m_csQAccess);
	// remove all the queue entries
	do
	{
		isEmpty = m_theQueue.empty ();
		if (!isEmpty)
		{
			theItem = m_theQueue.front ();
			m_theQueue.pop_front ();
			delete theItem;
		} // if 
	} while (!isEmpty);
	::LeaveCriticalSection (&m_csQAccess);
	// clear the semaphore
	while (WaitForSingleObject (m_theSignal, 0) == WAIT_OBJECT_0);
} // clear

/**
 * Method size enters a critical section before it the number of items in the queue.
 */
template <class T> int CProtectedQueue<T>::size (void)
{
	int theSize = 0;
	::EnterCriticalSection (&m_csQAccess);
	theSize = (int)m_theQueue.size ();
	::LeaveCriticalSection (&m_csQAccess);
	return theSize;
} // size

/**
 * Method isEmpty enters a critical section before it tests to see if the queue is empty.
 */
template <class T> bool CProtectedQueue<T>::isEmpty (void)
{
	bool isEmpty = false;
	::EnterCriticalSection (&m_csQAccess);
	isEmpty = m_theQueue.empty ();
	::LeaveCriticalSection (&m_csQAccess);
	return isEmpty;
} // isEmpty

#endif	// _PROTECTED_QUEUE_H
