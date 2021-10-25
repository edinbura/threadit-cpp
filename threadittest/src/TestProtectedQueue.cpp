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
 * Title: TestProtectedQueue
 * Description: TestProtectedQueue contains unit tests for the CProtectedQueue class. 
 * The purpose of these tests is to excercise as many methods of the CProtectedQueue
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
#include <protectedqueue.h>
#include <log4cpp/Category.hh>
#include <log4cpp/BasicConfigurator.hh>
#include <log4cpp/PropertyConfigurator.hh>

/** The number of elements to test. */
const int theMaxSize = 100;

/**
 * class CMySubject extends class CSubject. The class allows a value to be incremented
 * and the total number of increments to be counted. In this way the behaviour of the
 * observer can be evaluated.
 */
class CIntHolder
{
protected:
	int m_theValue;

public: 
	CIntHolder ()
	{
		m_theValue = 0;
	} // constructor CIntHolder

	CIntHolder (int theIntValue)
	{
		m_theValue = theIntValue;
	} // constructor CIntHolder

	~CIntHolder ()
	{
	} // destructor ~CIntHolder

	void set (int theIntValue)
	{
		m_theValue = theIntValue;
	} // set
		
	int getValue ()
	{
		return m_theValue;
	} // getValue

}; // class CIntHolder

/**
 * Test_ProtectedQueue_insertItem is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * is based on the size of the elements in the queue buffer as returned by the Size 
 * method.
 */
TEST (Test_ProtectedQueue_insertItem)
{
	int theValue = 0;
	CIntHolder* ptheInt = NULL;
	CIntHolder* theInts[theMaxSize];
	CProtectedQueue<CIntHolder> theQueue;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestProtectedQueue"));
	logger->info ("Testing - Test_ProtectedQueue_insertItem");

	for (int i = 0; i < theMaxSize; i++)
	{
		theInts[i] = new CIntHolder (i);
		theQueue.insertItem (theInts[i]);
	} // for 
	CHECK_EQUAL (theMaxSize, theQueue.size ()); 
} // TEST (Test_ProtectedQueue_insertItem)

/**
 * Test_ProtectedQueue_getItem is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * then checks that each of the elements can be retrieved from the queue and matches
 * what was put in the queue.
 */
TEST (Test_ProtectedQueue_getItem)
{
	int theValue = 0;
	CIntHolder* ptheInt = NULL;
	CIntHolder* theInts[theMaxSize];
	CProtectedQueue<CIntHolder> theQueue;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestProtectedQueue"));
	logger->info ("Testing - Test_ProtectedQueue_getItem");

	for (int i = 0; i < theMaxSize; i++)
	{
		theInts[i] = new CIntHolder (i);
		theQueue.insertItem (theInts[i]);
	} // for 
	CHECK_EQUAL (theMaxSize, theQueue.size ()); 

	for (int i = 0; i < theMaxSize; i++)
	{
		ptheInt = theQueue.getItem ();
		CHECK_EQUAL (ptheInt->getValue (), theInts[i]->getValue ());
	} // for 
	CHECK_EQUAL (0, theQueue.size ()); 
} // TEST (Test_ProtectedQueue_getItem)

/**
 * Test_ProtectedQueue_getItemNoDec is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * then checks that each of the elements can be retrieved from the queue and matches
 * what was put in the queue. Finally the method clears the semaphore count.
 */
TEST (Test_ProtectedQueue_getItemNoDec)
{
	int theValue = 0;
	int theCount = 0;
	HANDLE theSignal = NULL;
	CIntHolder* ptheInt = NULL;
	CIntHolder* theInts[theMaxSize];
	CProtectedQueue<CIntHolder> theQueue;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestProtectedQueue"));
	logger->info ("Testing - Test_ProtectedQueue_getItemNoDec");

	for (int i = 0; i < theMaxSize; i++)
	{
		theInts[i] = new CIntHolder (i);
		theQueue.insertItem (theInts[i]);
	} // for 
	CHECK_EQUAL (theMaxSize, theQueue.size ()); 

	for (int i = 0; i < theMaxSize; i++)
	{
		ptheInt = theQueue.getItemNoDec ();
		CHECK_EQUAL (ptheInt->getValue (), theInts[i]->getValue ());
	} // for 
	CHECK_EQUAL (0, theQueue.size ()); 

	// Now release the semaphore.
	theSignal = theQueue.getQSemaphore ();
	for (int i = 0; i < theMaxSize; i++)
	{
		if (ReleaseSemaphore (theSignal, 1, NULL))
		{
			theCount++;
		} // if 
	} // for 
	CHECK_EQUAL (theCount, theMaxSize);
} // TEST (Test_ProtectedQueue_getItemNoDec)

/**
 * Test_ProtectedQueue_waitItem is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * then checks that each of the elements can be retrieved from the queue and matches
 * what was put in the queue.
 */
TEST (Test_ProtectedQueue_waitItem)
{
	int theValue = 0;
	CIntHolder* ptheInt = NULL;
	CIntHolder* theInts[theMaxSize];
	CProtectedQueue<CIntHolder> theQueue;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestProtectedQueue"));
	logger->info ("Testing - Test_ProtectedQueue_waitItem");

	for (int i = 0; i < theMaxSize; i++)
	{
		theInts[i] = new CIntHolder (i);
		theQueue.insertItem (theInts[i]);
	} // for 
	CHECK_EQUAL (theMaxSize, theQueue.size ()); 

	for (int i = 0; i < theMaxSize; i++)
	{
		ptheInt = theQueue.waitItem (1000);
		CHECK_EQUAL (ptheInt->getValue (), theInts[i]->getValue ());
	} // for 
	CHECK_EQUAL (0, theQueue.size ()); 

	// Now check that the next wait actually takes the amount of time specified 
	// and returns a NULL;
	ptheInt = theQueue.waitItem (50);
	CHECK (ptheInt == NULL);
} // TEST (Test_ProtectedQueue_waitItem)

/**
 * Test_ProtectedQueue_other is a unit test checks the operation of the 
 * clear, size, isEmpty and exitAll method. The test is constrained
 * to take place in under a second to ensure that no wait periods are 
 * actually exceeded. 
 */
TEST (Test_ProtectedQueue_other)
{
	int theValue = 0;
	const int theMaxWait = 1000;
	CIntHolder* ptheInt = NULL;
	CIntHolder* theInts[theMaxSize];
	CProtectedQueue<CIntHolder> theQueue;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestProtectedQueue"));
	logger->info ("Testing - Test_ProtectedQueue_other");

	// This test must complete in under a second (theMaxWait) to ensure that no wait periods
	// have elapsed.
	UNITTEST_TIME_CONSTRAINT (theMaxWait/2);

	// Test that the queue can be cleared. Add a number of entries then check that
	// the specified number were added. Next clear the queue make sure it is empty.
	// After that check that items can be removed successfully.
	for (int i = 0; i < theMaxSize; i++)
	{
		theInts[i] = new CIntHolder (i);
		theQueue.insertItem (theInts[i]);
	} // for 
	CHECK_EQUAL (theMaxSize, theQueue.size ()); 

	theQueue.clear ();
	CHECK_EQUAL (0, theQueue.size ()); 
	// Check that the isEmpty method works.
	CHECK (theQueue.isEmpty ());

	// Now check that the queue is still in a good state.
	for (int i = 0; i < theMaxSize; i++)
	{
		theInts[i] = new CIntHolder (i);
		theQueue.insertItem (theInts[i]);
	} // for 
	CHECK_EQUAL (theMaxSize, theQueue.size ()); 
	for (int i = 0; i < theMaxSize; i++)
	{
		ptheInt = theQueue.waitItem (1000);
		CHECK_EQUAL (ptheInt->getValue (), theInts[i]->getValue ());
	} // for 
	CHECK_EQUAL (0, theQueue.size ()); 

} // TEST (Test_ProtectedQueue_other)

