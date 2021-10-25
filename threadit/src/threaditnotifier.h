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
 * This means that observers should never call methods on the CThreadItNotifier class within
 * a call from OnUpdate or OnChange.
 *
 * Copyright: Copyright (c) 2008 Ashkel Software 
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2008/12/19 02:38:06 $
 */

#ifndef THREADIT_NOTIFIER
#define THREADIT_NOTIFIER

// Include files
#include <list>
#include "threaditobserver.h"

//class CThreadIt;

/**
 * Class CThreadItNotifier represents the information being monitored for any changes or 
 * updates. Observers can register interest in the subject and will be notified
 * of updates and changes. 
 */
class CThreadItNotifier
{
	public:
		typedef std::list<CThreadItObserver> ThreadItObserverList;
	// Attributes
	protected:
		/** m_theObserverList is a list of observers that have registered thier interest
		 * in being notified when the subject (information) of interest is changed */
		ThreadItObserverList m_theObserverList;

	// Constructors and Destructors
	public :
		/** 
		 * Method CThreadItNotifier is the constructor for the instance. This implementation creates
		* a critical section to ensure that there are no concurrency issues when attaching or
		* detaching or when notifications are sent to interested observers. 
		*/
		CThreadItNotifier ();

		/**
		* Method ~CThreadItNotifier is the destructor for the instance. This implementation will
		* release all references to the observers stored in the instance.
		*/
		~CThreadItNotifier ();

	// Methods
	public :
		/**
		* Method attach allows an oberver to register it's intent to monitor changes or updates to the 
		* Subject under consideration. While the observer is attached to the subject it will receive
		* update / change notifications. 
		* ptheObserver is a reference to the observer that wishes to register it's intent to monitor 
		* changes to the subject. Note that if the ptheObserver is already in the list then the 
		* ptheObserver observer will not be added again.
		*/
		void attach (const CThreadItObserver& theObserver);

	/**
	 * Method detach allows an oberver to register it's intent to no longer monitor changes or 
	 * updates to the Subject under consideration. 
	 * ptheObserver is a reference to the observer that wishes to register it's intent to stop
	 * monitoring changes to the subject.
	 */
	void detach (const CThreadItObserver& theObserver);

	/**
	 * Method cleanExpiredObservers removes all observers that have expired. 
 	 */
	void cleanExpiredObservers ();

	/**
	 * Method notifyOnUpdate is used to notify all interestd observers of updates to this subject.
	 * When this method is called observers are notified by the OnUpdate method that they have 
	 * implemented being invoked. This method will supply a reference to this subject that has changed.
	 * This approach is normally used to provide a reference to instances that extend from CThreadItNotifier.
	 * NOTE: as this method is called within a critical section no calls to attach or detach should be
	 * made.
	 */ 
	void notify ();
	void notify (CICloneable& ptheObject);

	/**
	 * Method getNumberOfObservers returns the number of observers currently interested in updates
	 * or changes to the subject.
	 */
	int getNumberOfObservers ();

	/**
	 * Method removeObservers removes all observers that have attached to this subject.
	 */
	void removeObservers ();

}; // class CThreadItNotifier

#endif // #ifndef THREADIT_NOTIFIER
