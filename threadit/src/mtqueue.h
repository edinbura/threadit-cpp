/**
 * Title: CMtQueue
 * Description: class CMtQueue is a template for a queue that uses 
 * critical sections for access to elements in the queue. 
 *
 * Copyright: Copyright (c) 2009 Ashkel Software 
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2008/05/21 01:00:06 $
 */

#ifndef MT_QUEUE_H
#define MT_QUEUE_H

// Include files
#include <windows.h>
#include <list>

/**
 * Class CMtQueue is a template class that implements a queue
 * for the specified type. Access to elements in the queue is bounded by
 * a critical section to support use in a multi-threaded environment.
 */
template <class T> class CMtQueue 
{
  // non-copiable, because of a contained HANDLE
  const CMtQueue& operator=(const CMtQueue&);
  CMtQueue(const CMtQueue&);
	// Attributes
private:
	/** queue of class T */
	std::list<T>	 m_theQueue;
	/** Concurrency control via a critical section. */
	CRITICAL_SECTION	m_csQAccess;	
	/** Handle that is signalled when an item is received. */
	HANDLE m_theEvent;

	// Constructors and destructors
public:
	/**
	 * Default constructor: It initializes the critical section and creates the 
	 * HANDLE for the Semaphore.
	 */
	CMtQueue (void);

	/**
	 * ~CMtQueue is the destructor for the instance and frees all resources
	 * in use.
	 */
	~CMtQueue (void);
	
	// Methods
public: 
	/**
	 * Method insertItem enters a critical section before adding an item to the tail of 
	 * the queue. After it leaves the critical section, it releases a semaphore to
	 * indicate that an item has been added to the queue.
	 * removes all messages from the queue.
	 * theItem is the item to be added to the queue.
	 */
	void insertItem (const T& theItem);

	/**
	 * Method waitItem waits for a single object and then enters a critical section
	 * where it gets and then removes the first item in the queue if the 
	 * queue is not empty.
	 * rec is the item retreived from the queue.
	 * theWaitTime is the time(in millisecs) to wait for a semaphore.
	 * RET returns the success of the operation.
	 */
	bool waitItem (T& theItem, DWORD theWaitTime = 500); 

	/**
	 * Method GetItem returns a pointer to the first item in the queue.
	 * The method does not block but will return NULL if there is nothing
	 * in the queue. The internal queue semaphore count that is signalled when
	 * an item is available is decremented.
	 */
	bool getItem (T& theItem); 

	/**
	 * Method getItemNoDec returns a pointer to the first item in the queue.
	 * The method does not block but will return NULL if there is nothing
	 * in the queue. The method also does not decrement the signal count
	 * received for each item in the queue. This can be done by obtaining
	 * the queue semaphore and manipulating it directly.
	 */
	bool getItemNoDec (T& theItem); 
		
	/**
	 * Method getQSemaphore returns the semaphore associated with the queue.
	 */
	HANDLE getQSemaphore (void) const;
		
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

}; // template <class T> class CMtQueue 


/**
 * Implementation of template <class T> class CMtQueue.
 */

/**
 * Constructor CMtQueue is the default constructor: It initializes the 
 * critical section and creates the HANDLE for the Semaphore.
 */
template <class T> CMtQueue<T>::CMtQueue ()
{
	::InitializeCriticalSection (&m_csQAccess);
	// Create a manual reset event initially signalled.
	m_theEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
} // constructor CMtQueue

/**
 * Destructor ~CMtQueue releases all semaphores, closes the semaphore handle and 
 * removes all messages from the queue.
 */
template <class T> CMtQueue<T>::~CMtQueue ()
{
	::EnterCriticalSection (&m_csQAccess);
	CloseHandle (m_theEvent);
	m_theQueue.clear ();
	::LeaveCriticalSection (&m_csQAccess);
	::DeleteCriticalSection (&m_csQAccess);
} // destructor ~CMtQueue

/**
 * Method InsertItem enters a critical section before adding an item to the tail of 
 * the queue. After it leaves the critical section, it releases a semaphore to
 * indicate that an item has been added to the queue.
 * rec is theitem to be added to the queue.
 */
template <class T> void CMtQueue<T>::insertItem (const T& theItem)
{
	::EnterCriticalSection (&m_csQAccess);
	m_theQueue.push_back (theItem);
	::LeaveCriticalSection (&m_csQAccess);
	SetEvent (m_theEvent);
} // insertItem

/**
 * Method WaitItem waits for a single object and then enters a critical section
 * where it gets and then removes the first item in the queue if the 
 * queue is not empty.
 * rec is the item retrieved from the queue.
 * WaitTime is the time(in milli secs) to wait for a semaphore.
 * RET returns the success of the operation.
 */
template <class T> bool CMtQueue<T>::waitItem (T& theItem, DWORD theWaitTime)
{
	bool isItem = false;
	DWORD theResult = 0;

	// wait for an item to arrive
	theResult = WaitForSingleObject (m_theEvent, theWaitTime);
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
			isEmpty = m_theQueue.empty ();
			isItem = true;
		} // if 
		if (isEmpty)
		{
			// if no more elements in queue reset the event.
			ResetEvent (m_theEvent);
		} // if 
		::LeaveCriticalSection (&m_csQAccess);
	} // if 
	// return the result
	return isItem;
} // waitItem

/**
 * Method getItem enters a critical section
 * where it gets and then removes the first item in the queue if the 
 * queue is not empty. If the queue is empty the method returns NULL.
 * rec is the item retrieved from the queue.
 */
template <class T> bool CMtQueue<T>::getItem (T& theItem) 
{
	bool isEmpty = false;
	bool isItem = false;
	DWORD theResult = 0;

	theResult = WaitForSingleObject (m_theEvent, 0);
	// First check if the state of the specified object is signalled.
	if (theResult == WAIT_OBJECT_0)
	{
		::EnterCriticalSection (&m_csQAccess);
		// check the queue - if there is an item, get it
		isEmpty = m_theQueue.empty ();
		if (!isEmpty)
		{
			theItem = m_theQueue.front ();
			m_theQueue.pop_front ();
			isEmpty = m_theQueue.empty ();
			isItem = true;
		} // if 
		if (isEmpty)
		{
			// if no more elements in queue reset the event.
			ResetEvent (m_theEvent);
		} // if 
		::LeaveCriticalSection (&m_csQAccess);
	} // if 
	// return the result
	return isItem;
} // getItem

/**
 * Method getItemNoDec returns a pointer to the first item in the queue.
 * The method does not block but will return NULL if there is nothing
 * in the queue. The method also does not decrement the signal count
 * received for each item in the queue. This can be done by obtaining
 * the queue semaphore and manipulating it directly.
 */
template <class T> bool CMtQueue<T>::getItemNoDec (T& theItem) 
{
	bool isEmpty = false;
	bool isItem = false;

	// wait for an item to arrive
	::EnterCriticalSection (&m_csQAccess);
	// check the queue - if there is an item, get it
	isEmpty = m_theQueue.empty ();
	if (!isEmpty)
	{
		theItem = m_theQueue.front ();
		m_theQueue.pop_front ();
		isEmpty = m_theQueue.empty ();
		isItem = true;
	} // if 
	if (isEmpty)
	{
		// if no more elements in queue reset the event.
		ResetEvent (m_theEvent);
	} // if 
	::LeaveCriticalSection (&m_csQAccess);
	// return the result
	return isItem;
} // getItemNoDec

/**
 * Method getQSemaphore returns the semaphore associated with the queue.
 */
template <class T> HANDLE CMtQueue<T>::getQSemaphore (void) const
{
	return m_theEvent;
} // getQSemaphore

/** 
 * Methd clear this method enters a critical section before it removes all messages 
 * from the queue. The memory referenced by the elements in the queue is not freed
 * currently.
 */
template <class T> void CMtQueue<T>::clear (void)
{
	::EnterCriticalSection (&m_csQAccess);
	// remove all the queue entries
	m_theQueue.clear ();
	// Clear the event as the queue is now empty.
	ResetEvent (m_theEvent);
	::LeaveCriticalSection (&m_csQAccess);
} // clear

/**
 * Method size enters a critical section before it the number of items in the queue.
 */
template <class T> int CMtQueue<T>::size (void) 
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
template <class T> bool CMtQueue<T>::isEmpty (void) 
{
	bool isEmpty = false;
	::EnterCriticalSection (&m_csQAccess);
	isEmpty = m_theQueue.empty ();
	::LeaveCriticalSection (&m_csQAccess);
	return isEmpty;
} // isEmpty

#endif	// MT_QUEUE_H
