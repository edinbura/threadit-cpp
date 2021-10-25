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
 * Title: TestMtQueue
 * Description: TestMtQueue contains unit tests for the CMtQueue class. 
 * The purpose of these tests is to excercise as many methods of the CMtQueue
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
#include "UnitTestPP.h"
#include "TimeHelpers.h"
#include "TestResults.h"
#include "TestDetails.h"
#include <mtqueue.h>
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
 * Test_CMtQueue_insertItem is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * is based on the size of the elements in the queue buffer as returned by the Size 
 * method.
 */
TEST (Test_CMtQueue_insertItem)
{
	int theValue = 0;
	CIntHolder* ptheInt = NULL;
	CIntHolder* theInts[theMaxSize];
	CMtQueue<CIntHolder> theQueue;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestCMtQueue"));
	logger->info ("Testing - Test_CMtQueue_insertItem");

	for (int i = 0; i < theMaxSize; i++)
	{
		theInts[i] = new CIntHolder (i);
		theQueue.insertItem (*theInts[i]);
	} // for 
	CHECK_EQUAL (theMaxSize, theQueue.size ()); 
} // TEST (Test_CMtQueue_insertItem)

/**
 * Test_CMtQueue_getItem is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * then checks that each of the elements can be retrieved from the queue and matches
 * what was put in the queue.
 */
TEST (Test_CMtQueue_getItem)
{
	int theValue = 0;
	CIntHolder theInt;
	CIntHolder* theInts[theMaxSize];
	CMtQueue<CIntHolder> theQueue;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestCMtQueue"));
	logger->info ("Testing - Test_CMtQueue_getItem");

	for (int i = 0; i < theMaxSize; i++)
	{
		theInts[i] = new CIntHolder (i);
		theQueue.insertItem (*theInts[i]);
	} // for 
	CHECK_EQUAL (theMaxSize, theQueue.size ()); 

	for (int i = 0; i < theMaxSize; i++)
	{
		theQueue.getItem (theInt);
		CHECK_EQUAL (theInt.getValue (), theInts[i]->getValue ());
	} // for 
	CHECK_EQUAL (0, theQueue.size ()); 
} // TEST (Test_CMtQueue_getItem)

/**
 * Test_CMtQueue_getItemNoDec is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * then checks that each of the elements can be retrieved from the queue and matches
 * what was put in the queue. Finally the method clears the semaphore count.
 */
TEST (Test_CMtQueue_getItemNoDec)
{
	int theValue = 0;
	int theCount = 0;
	HANDLE theSignal = NULL;
	CIntHolder theInt;
	CIntHolder* theInts[theMaxSize];
	CMtQueue<CIntHolder> theQueue;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestCMtQueue"));
	logger->info ("Testing - Test_CMtQueue_getItemNoDec");

	for (int i = 0; i < theMaxSize; i++)
	{
		theInts[i] = new CIntHolder (i);
		theQueue.insertItem (*theInts[i]);
	} // for 
	CHECK_EQUAL (theMaxSize, theQueue.size ()); 

	for (int i = 0; i < theMaxSize; i++)
	{
		theQueue.getItemNoDec (theInt);
		CHECK_EQUAL (theInt.getValue (), theInts[i]->getValue ());
	} // for 
	CHECK_EQUAL (0, theQueue.size ()); 

} // TEST (Test_CMtQueue_getItemNoDec)

/**
 * Test_CMtQueue_waitItem is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * then checks that each of the elements can be retrieved from the queue and matches
 * what was put in the queue.
 */
TEST (Test_CMtQueue_waitItem)
{
	int theValue = 0;
	bool isItem = false;
	CIntHolder theInt;
	CIntHolder* theInts[theMaxSize];
	CMtQueue<CIntHolder> theQueue;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestCMtQueue"));
	logger->info ("Testing - Test_CMtQueue_waitItem");

	for (int i = 0; i < theMaxSize; i++)
	{
		theInts[i] = new CIntHolder (i);
		theQueue.insertItem (*theInts[i]);
	} // for 
	CHECK_EQUAL (theMaxSize, theQueue.size ()); 

	for (int i = 0; i < theMaxSize; i++)
	{
		isItem = theQueue.waitItem (theInt, 1000);
		CHECK_EQUAL (theInt.getValue (), theInts[i]->getValue ());
	} // for 
	CHECK_EQUAL (0, theQueue.size ()); 

	// Now check that the next wait actually takes the amount of time specified 
	// and returns a NULL;
	isItem = theQueue.waitItem (theInt, 50);
	CHECK (isItem == false);
} // TEST (Test_CMtQueue_waitItem)

/**
 * Test_CMtQueue_other is a unit test checks the operation of the 
 * clear, size, isEmpty and exitAll method. The test is constrained
 * to take place in under a second to ensure that no wait periods are 
 * actually exceeded. 
 */
TEST (Test_CMtQueue_other)
{
	int theValue = 0;
	const int theMaxWait = 1000;
	CIntHolder theInt;
	CIntHolder* theInts[theMaxSize];
	CMtQueue<CIntHolder> theQueue;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestCMtQueue"));
	logger->info ("Testing - Test_CMtQueue_other");

	// This test must complete in under a second (theMaxWait) to ensure that no wait periods
	// have elapsed.
	UNITTEST_TIME_CONSTRAINT (theMaxWait/2);
	//TestDetails const details("testname", "suitename", "filename", 10);
	// TimeConstraint t(10, details, 10);
	//TimeHelpers::SleepMs(20);

	// Test that the queue can be cleared. Add a number of entries then check that
	// the specified number were added. Next clear the queue make sure it is empty.
	// After that check that items can be removed successfully.
	for (int i = 0; i < theMaxSize; i++)
	{
		theInts[i] = new CIntHolder (i);
		theQueue.insertItem (*theInts[i]);
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
		theQueue.insertItem (*theInts[i]);
	} // for 
	CHECK_EQUAL (theMaxSize, theQueue.size ()); 
	for (int i = 0; i < theMaxSize; i++)
	{
		theQueue.waitItem (theInt, 1000);
		CHECK_EQUAL (theInt.getValue (), theInts[i]->getValue ());
	} // for 
	CHECK_EQUAL (0, theQueue.size ()); 

} // TEST (Test_CMtQueue_other)

