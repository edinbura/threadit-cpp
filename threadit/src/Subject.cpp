/*-------------------------------------------------------------------------*/
/* Copyright (C) 2021 by Ashkel Software                                   */
/* ari@ashkel.com.au                                                       */
/*                                                                         */
/* This file is part of the threadit library.                              */
/*                                                                         */
/* The threadit library is free software; you can redistribute it and/or   */
/* modify it under the terms of The Code Project Open License (CPOL) 1.02  */
/*                                                                         */
/* The threadit library is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the CPOL       */
/* License for more details.                                               */
/*                                                                         */
/* You should have received a copy of the CPOL License along with this     */
/* software.                                                               */
/*-------------------------------------------------------------------------*/

/**
 * Title: CSubject
 * Description: class CSubject represents the subject of interest using the
 * Observer pattern. Observers can register thier interest in the subject and
 * are notified when there are changes or updates. This particular implementation
 * supports two mechamisms for updating observers: 
 * 1) CSubject can be subclassed and the observer will receive a refernce to the updated
 * subject. This is along the lines of the full Observer pattern. 
 * 2) CSubject can be used to inform observers of changes where a general instance 
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
#include <list>
#include <iterator>
#include <algorithm>
#include "Subject.h"

using namespace std;

/** 
 * Method CSubject is the constructor for the instance. This implementation creates
 * a critical section to ensure that there are no concurrency issues when attaching or
 * detaching or when notifications are sent to interested observers. 
 */
CSubject::CSubject()
{
	::InitializeCriticalSection (&m_theSection);
} // constructor CSubject

/**
 * Method ~CSubject is the destructor for the instance. This implementation will
 * release all references to the observers stored in the instance.
 */
CSubject::~CSubject()
{
	removeObservers();
  ::DeleteCriticalSection(&m_theSection);
} // destructor ~CSubject

/**
 * Method attach allows an oberver to register it's intent to monitor changes or updates to the 
 * Subject under consideration. While the observer is attached to the subject it will receive
 * update / change notifications. 
 * ptheObserver is a reference to the observer that wishes to register it's intent to monitor 
 * changes to the subject. Note that if the ptheObserver is already in the list then the 
 * ptheObserver observer will not be added again.
 */
void CSubject::attach (CObserver* ptheObserver) 
{
	::EnterCriticalSection (&m_theSection);
	// Add the observer to the list if it is not already in the list.
	if (std::find (m_theObserverList.begin (), m_theObserverList.end (), ptheObserver) == m_theObserverList.end ())
	{ 
		m_theObserverList.push_back (ptheObserver);
	} // if 
	::LeaveCriticalSection (&m_theSection);
} // attach

/**
 * Method detach allows an oberver to register it's intent to no longer monitor changes or 
 * updates to the Subject under consideration. 
 * ptheObserver is a reference to the observer that wishes to register it's intent to stop
 * monitoring changes to the subject.
 */
void CSubject::detach (CObserver * ptheObserver )
{
	::EnterCriticalSection (&m_theSection);
	// Find and remove the observer from the list
	m_theObserverList.remove (ptheObserver);
	::LeaveCriticalSection (&m_theSection);
} // detach

/**
 * Method removeObservers removes all observers that have attached to this subject.
 */
void CSubject::removeObservers ()
{
	::EnterCriticalSection (&m_theSection);
	// Find and remove the observer from the list
	m_theObserverList.clear ();
	::LeaveCriticalSection (&m_theSection);
} // removeObservers

/**
 * Method notifyOnUpdate is used to notify all interestd observers of updates to this subject.
 * When this method is called observers are notified by the OnUpdate method that they have 
 * implemented being invoked. This method will supply a reference to this subject that has changed.
 * This approach is normally used to provide a reference to instances that extend from CSubject.
 * NOTE: as this method is called within a critical section no calls to attach or detach should be
 * made.
 */ 
void CSubject::notifyOnUpdate ()
{
	CObserver* pObserver = NULL;
	std::list<CObserver*>::iterator theInterator;

	// Notify all the observers to update themselves
	::EnterCriticalSection (&m_theSection);
	for (theInterator = m_theObserverList.begin(); theInterator != m_theObserverList.end(); theInterator++) 
	{
		(*theInterator)->onUpdate (*this);
	} // for
	::LeaveCriticalSection (&m_theSection);
} // notifyOnUpdate

/**
 * Method notifyOnChange is used to notify all interestd observers of changes to a subject.
 * When this method is called observers are notified by the OnChange method that they have 
 * implemented being invoked. This method will supply a reference to the subject that has been
 * updated. In this case it is not necessary to extend from CSubject.
 * ptheSubject is a pointer to the object that represents the change that the observer is being
 * notified of. 
 * ptheSubject is a pointer to the information that decribes the change. 
 * NOTE: as this method is called within a critical section no calls to attach or detach should be
 * made.
 */ 
void CSubject::notifyOnChange (CObserver::VoidRef theSubject)
{
	CObserver* pObserver = NULL;
	std::list<CObserver*>::iterator theInterator;

	// Notify all the observers to update themselves
	::EnterCriticalSection (&m_theSection);
	for (theInterator = m_theObserverList.begin(); theInterator != m_theObserverList.end(); theInterator++) 
	{
		(*theInterator)->onChange (theSubject);
	} // for
	::LeaveCriticalSection (&m_theSection);
} // notifyOnChange

/**
 * Method getNumberOfObservers returns the number of observers currently interested in updates
 * or changes to the subject.
 */
int CSubject::getNumberOfObservers () const
{
	return (int)m_theObserverList.size ();
} // getNumberOfObservers

