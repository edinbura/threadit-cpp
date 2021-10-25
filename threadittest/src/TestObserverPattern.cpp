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
 * Title: TestObserverPattern
 * Description: TestObserverPattern contains unit tests for the CSubject and CObserver 
 * classes. The purpose of these tests is to excercise as many methods of the CSubject
 * class as possible so that the class behaviour is correct and the class 
 * can be regression tested as part of an automated test suite. 
 *
 * Copyright: Copyright (c) 2008 Ashkel Software 
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2006/12/19 02:38:06 $
 */

#include "stdafx.h"
#include <UnitTest++.h>
#include <subject.h>
#include <observer.h>
#include <log4cpp\Category.hh>
#include <log4cpp/BasicConfigurator.hh>
#include <log4cpp/PropertyConfigurator.hh>

/**
 * class CMySubject extends class CSubject. The class allows a value to be incremented
 * and the total number of increments to be counted. In this way the behaviour of the
 * observer can be evaluated.
 */
class CMySubject : public CSubject
{
protected:
	int theValue;

public: 
	CMySubject ()
	{
		theValue = 0;
	} // constructor CMySubject

	~CMySubject ()
	{
	} // destructor ~CMyObserver

	void increment ()
	{
		theValue++;
	} // increment

	void reset ()
	{
		theValue = 0;
	} // reset
		
	int getValue ()
	{
		return theValue;
	} // increment

}; // class CMySubject

/** The number of elements to test. */
const int theMaxSize = 100;

/**
 * class CMyObserver extends CObserver and implements the	 virtual methods that
 * are invoked when updates or changes are made to the subjects. Each update or change
 * increments an update or change counter respectively. The number of changes made
 * can be queried and thus the behaviour of this class relative to the updates on the 
 * subject can be evaluated.
 */
class CMyObserver : public CObserver
{
protected:
	int m_theUpdates;
	int m_theChanges;

public: 
	CMyObserver ()
	{
		m_theUpdates = 0;
		m_theChanges = 0;
	} // constructor

	~CMyObserver ()
	{
	} // destructor ~CMyObserver

	bool onUpdate (const CSubject& theSubject)
	{
		m_theUpdates++;
		return true;
	} // onUpdate
		
	bool onChange (VoidRef ptheSubject)
	{
		m_theChanges++;
		return true;
	} // onChange

	int getUpdates ()
	{
		return m_theUpdates;
	}	 // getUpdates

	int getChanges ()
	{
		return m_theChanges;
	}	 // getChanges

}; // class CMyObserver


/**
 * Test_ObserverPattern_Attach_Observers is a unit test that adds the many number
 * of observers to a single subject. The test checks that there are that many
 * number of observers on the subject. 
 */
TEST (Test_ObserverPattern_Attach_Observers)
{
	CMySubject aSubject;
	CMyObserver* ptheObserver = NULL;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestObserverPattern"));
	logger->info ("Testing - Test_ObserverPattern_Attach_Observers");
	// Create and attach the observers to the subject.
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheObserver = new CMyObserver ();
		aSubject.attach (ptheObserver);
	} // for 
	// Check that the subject has the attached number of observers.
	CHECK_EQUAL (theMaxSize, aSubject.getNumberOfObservers ());
} // TEST (Test_ObserverPattern_Attach_Observers)

/**
 * Test_ObserverPattern_Dettach_Observers attaches a given number of observers to a single
 * subject. Each of the observers is then detached from the subject. At the end of the test
 * there should not be any observers on the subject.
 */
TEST (Test_ObserverPattern_Dettach_Observers)
{
	CMySubject aSubject;
	CMyObserver* ptheObserver = NULL;
	CMyObserver* ptheObserverList[theMaxSize];

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestObserverPattern"));
	logger->info ("Testing - Test_ObserverPattern_Dettach_Observers");
	// Add the requisite number of observers.
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheObserver = new CMyObserver ();
		ptheObserverList[i] = ptheObserver;
		aSubject.attach (ptheObserver);
	} // for 
	CHECK_EQUAL (theMaxSize, aSubject.getNumberOfObservers ());
	// Detach the number of observers.
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheObserver = ptheObserverList[i];
		aSubject.detach (ptheObserver);
	} // for 
	CHECK_EQUAL (0, aSubject.getNumberOfObservers ());
} // TEST (Test_ObserverPattern_Dettach_Observers)

/**
 * Test_ObserverPattern_notifyOnChange attaches many observers on a single subject.
 * The subject is updated once only and each observer is checked to see that it received
 * the change notification.
 */
TEST (Test_ObserverPattern_notifyOnChange)
{
	CMySubject aSubject;
	CMyObserver* ptheObserver = NULL;
	CMyObserver* ptheObserverList[theMaxSize];

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestObserverPattern"));
	logger->info ("Testing - Test_ObserverPattern_notifyOnChange");
	// Attach a number of observers to the subject.
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheObserver = new CMyObserver ();
		ptheObserverList[i] = ptheObserver;
		aSubject.attach (ptheObserver);
	} // for 
	CHECK_EQUAL (theMaxSize, aSubject.getNumberOfObservers ());
	// Modify the subject and notify all the observers of the change.
	aSubject.increment ();
	aSubject.notifyOnChange (aSubject);
	// Check that there are the requisite number of updates on each observer.
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheObserver = ptheObserverList[i];
		CHECK_EQUAL (1, ptheObserver->getChanges ());
		CHECK_EQUAL (0, ptheObserver->getUpdates ());
	} // for 
	// Remove all observers from the subject.
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheObserver = ptheObserverList[i];
		aSubject.detach (ptheObserver);
	} // for 
	CHECK_EQUAL (0, aSubject.getNumberOfObservers ());
	// Check that there are no more updates.
	// Change the subject and notify the observers.
	aSubject.increment ();
	aSubject.notifyOnChange (aSubject);
	// Check that there are the requisite number of updates on each observer.
	// Which should not have changed.
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheObserver = ptheObserverList[i];
		CHECK_EQUAL (1, ptheObserver->getChanges ());
		CHECK_EQUAL (0, ptheObserver->getUpdates ());
	} // for 
} // TEST (Test_ObserverPattern_notifyOnChange)

/**
 * Test_ObserverPattern_notifyOnUpdate attaches many observers on a single subject.
 * The subject is update once only and each observer is checked to see that it received
 * the update notification.
 */
TEST (Test_ObserverPattern_notifyOnUpdate)
{
	CMySubject aSubject;
	CMyObserver* ptheObserver = NULL;
	CMyObserver* ptheObserverList[theMaxSize];

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestObserverPattern"));
	logger->info ("Testing - Test_ObserverPattern_notifyOnUpdate");
	// Attach a number of observers to the subject.
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheObserver = new CMyObserver ();
		ptheObserverList[i] = ptheObserver;
		aSubject.attach (ptheObserver);
	} // for 
	CHECK_EQUAL (theMaxSize, aSubject.getNumberOfObservers ());
	// Modify the subject and notify all the observers of the update.
	aSubject.increment ();
	aSubject.notifyOnUpdate ();
	// Check that there are the requisite number of updates.
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheObserver = ptheObserverList[i];
		CHECK_EQUAL (0, ptheObserver->getChanges ());
		CHECK_EQUAL (1, ptheObserver->getUpdates ());
	} // for 
	// Remove all observers from the subject.	 
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheObserver = ptheObserverList[i];
		aSubject.detach (ptheObserver);
	} // for 
	CHECK_EQUAL (0, aSubject.getNumberOfObservers ());
	// Change the subject and notify the observers.
	aSubject.increment ();
	aSubject.notifyOnUpdate ();
	// Check that there are the requisite number of updates on each observer.
	// Which should not have changed.
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheObserver = ptheObserverList[i];
		CHECK_EQUAL (0, ptheObserver->getChanges ());
		CHECK_EQUAL (1, ptheObserver->getUpdates ());
	} // for 
} // TEST (Test_ObserverPattern_notifyOnUpdate)

/**
 * Test_ObserverPattern_notify is a similar test to all the above except in this case
 * there is a subject per observer. Each subject is modified and both the change and
 * update methods are invoked. The test checks to see that the requisite number of 
 * updates are made for each case.
 */
TEST (Test_ObserverPattern_notify)
{
	CMySubject* ptheSubject = NULL;
	CMySubject* ptheSubjectList[theMaxSize];
	CMyObserver* ptheObserver = NULL;
	CMyObserver* ptheObserverList[theMaxSize];

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestObserverPattern"));
	logger->info ("Testing - Test_ObserverPattern_notify");
	// Create each subject in turn and add an observer.
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheObserver = new CMyObserver ();
		ptheObserverList[i] = ptheObserver;
		ptheSubject = new CMySubject ();
		ptheSubjectList[i] = ptheSubject;
		ptheSubject->attach (ptheObserver);
		CHECK_EQUAL (1, ptheSubject->getNumberOfObservers ());
	} // for 
	// Change the subject and notify the observer using a change and an 
	// update notification.
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheSubject = ptheSubjectList[i];
		ptheSubject->increment ();
		ptheSubject->notifyOnChange (*ptheSubject);
		ptheSubject->notifyOnUpdate ();
	} // for 
	// Check that there are the requisite number of updates and
	// changes recorded by each obsever.
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheObserver = ptheObserverList[i];
		CHECK_EQUAL (1, ptheObserver->getChanges ());
		CHECK_EQUAL (1, ptheObserver->getUpdates ());
	} // for 
	// Remove each observer from each subject.
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheObserver = ptheObserverList[i];
		ptheSubject = ptheSubjectList[i];
		ptheSubject->detach (ptheObserver);
		CHECK_EQUAL (0, ptheSubject->getNumberOfObservers ());
	} // for 
	// Increment and notify on each subject. 
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheSubject = ptheSubjectList[i];
		ptheSubject->increment ();
		ptheSubject->notifyOnChange (*ptheSubject);
		ptheSubject->notifyOnUpdate ();
	} // for 
	// Check that there are the requisite number of updates which 
	// should not have changed from the last change and update notification.
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheObserver = ptheObserverList[i];
		CHECK_EQUAL (1, ptheObserver->getChanges ());
		CHECK_EQUAL (1, ptheObserver->getUpdates ());
	} // for 
} // TEST (Test_ObserverPattern_notify)

