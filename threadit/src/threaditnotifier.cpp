/**
 * Title: CThreadItNotifier
 * Description: class CThreadItNotifier represents the subject of interest using the
 * Observer pattern. Observers can register thier interest in the subject and
 * are notified when there are changes or updates. This particular implementation
 * supports two mechamisms for updating observers: 
 * 1) CThreadItNotifier can be subclassed and the observer will receive a refernce to the updated
 * subject. This is along the lines of the full Observer pattern. 
 * 2) CThreadItNotifier can be used to inform observers of changes where a general instance 
 * representing the change is transferred to the observer. This is similar to the 
 * java approach.
 * See the CObserver class for the partner class in this pattern.
 * This class uses critical sections to ensure that there are no concurency issues
 * when modifying the observer lists and when updates are presented to observers.
 *
 * Copyright: Copyright (c) 2008 Ashkel Software 
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2008/12/19 02:38:06 $
 */

// Include files
#include "stdafx.h"
#include <map>
#include <iterator>
#include <algorithm>
#include "icloneable.h"
#include "threaditnotifier.h"

using namespace std;

template<class T> struct map_data_compare : public std::binary_function<typename T::value_type, typename T::mapped_type, bool> 
{ 
	public: 
    bool operator() (typename T::value_type &pair,  typename T::mapped_type i) const 
    { 
      return pair.first == i; 
    } 
}; 

/** 
 * Method CThreadItNotifier is the constructor for the instance. This implementation creates
 * a critical section to ensure that there are no concurrency issues when attaching or
 * detaching or when notifications are sent to interested observers. 
 */
CThreadItNotifier::CThreadItNotifier()
{
} // constructor CThreadItNotifier

/**
 * Method ~CThreadItNotifier is the destructor for the instance. This implementation will
 * release all references to the observers stored in the instance.
 */
CThreadItNotifier::~CThreadItNotifier()
{
	removeObservers();
} // destructor ~CThreadItNotifier

/**
 * Method attach allows an oberver to register it's intent to monitor changes or updates to the 
 * Subject under consideration. While the observer is attached to the subject it will receive
 * update / change notifications. 
 * ptheObserver is a reference to the observer that wishes to register it's intent to monitor 
 * changes to the subject. Note that if the ptheObserver is already in the list then the 
 * ptheObserver observer will not be added again.
 */
void CThreadItNotifier::attach (const CThreadItObserver& theObserver) 
{
	// Add the observer to the list if it is not already in the list.
	bool isInList = false;
	ThreadItObserverList::iterator theIterator;

	// Check and see if the observer is already in the list or not. 
	for (theIterator = m_theObserverList.begin (); theIterator != m_theObserverList.end (); theIterator++) 
	{
		if ((*theIterator) == theObserver)
		{
			isInList = true;
			break;
		} // if 
	} // for
	if (!isInList)
	{
		m_theObserverList.push_back (theObserver);
	} // if 
} // attach

/**
 * Method detach allows an oberver to register it's intent to no longer monitor changes or 
 * updates to the Subject under consideration. 
 * ptheObserver is a reference to the observer that wishes to register it's intent to stop
 * monitoring changes to the subject.
 */
void CThreadItNotifier::detach (const CThreadItObserver& theObserver )
{
	bool isInList = false;
	ThreadItObserverList::iterator theIterator;

	// Find and remove all the observers from the list
	do
	{
		isInList = false;
		for (theIterator = m_theObserverList.begin (); theIterator != m_theObserverList.end (); theIterator++) 
		{
			if ((*theIterator) == theObserver)
			{
				isInList = true;
				break;
			} // if 
		} // for
		if (isInList)
		{
			m_theObserverList.erase (theIterator);
		} // if 
	} while (isInList);
} // detach

/**
 * Method removeObservers removes all observers that have attached to this subject.
 */
void CThreadItNotifier::removeObservers ()
{
	// Find and remove the observer from the list
	m_theObserverList.clear ();
} // removeObservers

/**
 * Method cleanExpiredObservers removes all observers that have expired. 
 */
void CThreadItNotifier::cleanExpiredObservers ()
{
	bool isExpired = false;
	ThreadItObserverList::iterator theIterator;

	// Find and remove all the observers from the list
	do
	{
		isExpired = false;
		for (theIterator = m_theObserverList.begin (); theIterator != m_theObserverList.end (); theIterator++) 
		{
			if (!(*theIterator).isValid ())
			{
				isExpired = true;
				break;
			} // if 
		} // for
		if (isExpired)
		{
			detach (*theIterator);
		} // if 
	} while (isExpired);
} // cleanExpiredObservers

/**
 * Method notifyOnUpdate is used to notify all interestd observers of updates to this subject.
 * When this method is called observers are notified by the OnUpdate method that they have 
 * implemented being invoked. This method will supply a reference to this subject that has changed.
 * This approach is normally used to provide a reference to instances that extend from CThreadItNotifier.
 * NOTE: as this method is called within a critical section no calls to attach or detach should be
 * made.
 */ 
void CThreadItNotifier::notify ()
{
	ThreadItObserverList::iterator theIterator;

	// Notify all the observers to update themselves
	for (theIterator = m_theObserverList.begin(); theIterator != m_theObserverList.end(); theIterator++) 
	{
		(*theIterator).notify ();
	} // for
} // notify

void CThreadItNotifier::notify (CICloneable& theObject)
{
	ThreadItObserverList::iterator theIterator;

	// Notify all the observers to update themselves
	for (theIterator = m_theObserverList.begin(); theIterator != m_theObserverList.end(); theIterator++) 
	{
		(*theIterator).notify (theObject);
	} // for
} // notify

/**
 * Method getNumberOfObservers returns the number of observers currently interested in updates
 * or changes to the subject.
 */
int CThreadItNotifier::getNumberOfObservers ()
{
	return (int)m_theObserverList.size ();
} // getNumberOfObservers

