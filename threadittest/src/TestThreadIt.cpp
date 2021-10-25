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
#include <log4cpp/Category.hh>
#include <log4cpp/BasicConfigurator.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include "dataitem.h"
#include "uintdataitem.h"
#include "threadit.h"
#include "observer.h"
//#include "uintdataitem.h"
#include "threaditmessage.h"

#include <memory>
using namespace std;

/** The number of elements to test. */
const int theMaxSize = 100;
const int theMaxWait = 1000;
const int theMaxEvents = 100;

class CTestItSetupMsg 
{
  // Attributes
  public:

  public:
    CTestItSetupMsg::CTestItSetupMsg ()
    {
      // Initialise the parameters to the default values.
    } // cDetectorSetupMsg

    CTestItSetupMsg::~CTestItSetupMsg ()
    {
      // Initialise the parameters to the default values.
    } // cDetectorSetupMsg


}; // cDetectorSetupMsg

class CCallback : public CObserver 
{
  // Attributes
  protected:
    int m_theUpdateCount;
	int m_theChangeCount;
	CProtectedQueue<DataItemPtr> m_ptheDataItemQ;

  // Services
  public:	
    CCallback::CCallback ()
	{
      m_theUpdateCount = 0;
	  m_theChangeCount = 0;
	} // constructor CCallback
   
	bool onUpdate (const CSubject& ptheSubject)
	{
	  // The owner of ptheSubject is usually the supplier as only the supplier knows when
	  // all observers have utilised the instance. This means that the reference is only valid
	  // for the lifetime of the method invocation. In this case ptheSubject is usally the
	  // element that has changed and is always the same object instance for this observer.
      m_theUpdateCount++;
      return true;
	} // onUpdate

    bool onChange (VoidRef ptheSubject)
	{
	  // The owner of ptheSubject is usually the supplier as only the supplier knows when
	  // all observers have utilised the instance. This means that the reference is only valid
	  // for the lifetime of the method invocation.
      m_theChangeCount++;
			CThreadItCallback& theCallback = ptheSubject.cast<CThreadItCallback>();
			DataItemPtr* aPtr = new DataItemPtr;
			theCallback.getDataItem (*aPtr);
			if(*aPtr)
			{
				UINT* pabc = NULL;
				((CUintDataItem*)aPtr->get ())->getDataItem ((UINT*&) pabc);
				UINT ax;
				ax = *pabc;
				ax++;

			} // if 
			m_ptheDataItemQ.insertItem(aPtr);
      return true;
	} // onChange

	int getChangeCount ()
	{
      return m_theChangeCount;
	} // getChangeCount

	void getDataItem (DataItemPtr& ptheDataItem)
	{
    auto_ptr<DataItemPtr> theDataItem(m_ptheDataItemQ.waitItem(10000));
    if (theDataItem.get())
      ptheDataItem = *theDataItem;
	} // getDataItem


	int getUpdateCount ()
	{
	  return m_theUpdateCount;
	} // getUpdateCount

}; // class CCallback


// Constant Definitions:

/**
 * Work Instructions: The following constants define the work instructions 
 * that the CTestIt instance will perform on request. 
 */
/** THREADIT_TEST_START is work instruction message that allows the detector to 
 * process incoming packets of I and Q data. */
#define THREADIT_TEST_START              1
/** DETECTOR_STOP is a work instruction message that terminates
 * the current read operation. I and Q data packets received after the
 * receipt of this message are ignored/ */ 
#define THREADIT_TEST_STOP               2
/** DETECTOR_SETUP is a work instruction message that sets up the 
 * Detector's operating parameters. */
#define THREADIT_TEST_METHOD             3


class CTestIt : public CThreadIt
{
  protected:
  // Attributes
	CCallback m_theCallback;
	WorkPackItQ m_theEventQueue;
    log4cpp::Category* m_pthelogger;

  public:
	  CTestIt::CTestIt () : CThreadIt("threadit.CTestIt")
    {
      // Initialise the work package.
      testItInit ();
    } // CTestIt
 
    CTestIt::CTestIt (int Priority) : CThreadIt ("threadit.CTestIt", Priority)
    {
      // Initialise the work package.
      testItInit ();
    } // CTestIt

    CTestIt::~CTestIt ()
    {
      // Terminate the work processing thread.
      stopThread ();
      // Wait for the thread to stop.
      waitForThreadToStop ();
    } // ~cDetector

    void testItInit ()
    {
      m_ptheLogger = &(log4cpp::Category::getInstance ("threadit.CTestIt"));
      // Setup all the work packages and associated messages..
  	  setWorkerMethod ((WorkerMethodType)&CTestIt::setup, THREADIT_TEST_METHOD);
			addObserver (&m_theCallback);
      setPeriod (100);
    }  // testItInit

	/**
	 * Method setup is an example worker method implementation. 
	 * pWorkPack is provided by the call to startWork. The caller of startWork must
	 * provide a pWorkPack allocated from the heap. Any internal information should
	 * also be provided from the heap. Ownership is then relinquished and the memory
	 * should no longer be referenced. The memory allocated should be freed by 
	 * this method.
	 * pWorkDone is returned by this method and is recieved by a recipient waiting
	 * on the getWork method. This method will allocate pWorkDone and any instance
	 * it provides on the heap. Once it has been returned by this method the memory
	 * reference by pWorkDone and its data members should not be accessed. This 
	 * means that the memory allocated and returned by the getWork method call 
	 * should be freed by the recipient of the WorkDone work packit. 
	 */
    bool setup (CWorkPackIt* pWorkPack, CWorkPackIt*& pWorkDone)
    {
      CTestItSetupMsg* pMsg = NULL;
	  pWorkDone = new CWorkPackIt ();

	  //pWorkDone->m_SendResult = true;
	  //pWorkDone->m_theInstruction = pWorkPack->m_theInstruction;
	  //pWorkDone->m_UseDefaultQ = true;
	  //pWorkDone->m_theWorkPackID = pWorkPack->m_theWorkPackID;

	  *pWorkDone = *pWorkPack;

	  //*pWorkDone = *pWorkPack;
      // Get the incoming message.  
      pMsg = (CTestItSetupMsg*)pWorkPack->m_ptheObject;
      if (pMsg != NULL)
      { 
        // Setup the parameters for the instance that sinks our data.
				if (!pWorkPack->m_isObjectInCallback)
				{
					//  Free the memory in use.
					delete pMsg;
				} // if 
      } // if 
			delete pWorkPack;
      // Setup the Fft processing software.
      // Return the work status.
      pWorkDone->m_theStatus = 0;
      // Return the method status.
      return true;
    } // Setup

	/**
	 * Method periodicMethod is an example periodicMethod method implementation. 
	 * pWorkPack is provided by ThreadIt and cannot be changed.
	 * pWorkDone is returned by this method and is recieved by a recipient waiting
	 * on the getWork method. This method will allocate pWorkDone and any instance
	 * it provides on the heap. Once it has been returned by this method the memory
	 * reference by pWorkDone and its data members should not be accessed. This 
	 * means that the memory allocated and returned by the getWork method call 
	 * should be freed by the recipient of the WorkDone work packit. 
	 */
    bool periodicMethod (CWorkPackIt WorkPack, CWorkPackIt*& pWorkDone)
    {
      pWorkDone = new CWorkPackIt ();

	  *pWorkDone = WorkPack;
	  pWorkDone->m_isSendResult = true;
	  pWorkDone->m_isNotifyWithCallback = true;
	  pWorkDone->m_isUseDefaultQ = true;
	  return true;
    }  // periodicMethod

	/**
	 * Method eventMethod is an example eventMethod method implementation. 
	 * pWorkPack is provided by ThreadIt and cannot be changed.
	 * pWorkDone is returned by this method and is recieved by a recipient waiting
	 * on the getWork method. This method will allocate pWorkDone and any instance
	 * it provides on the heap. Once it has been returned by this method the memory
	 * reference by pWorkDone and its data members should not be accessed. This 
	 * means that the memory allocated and returned by the getWork method call 
	 * should be freed by the recipient of the WorkDone work packit. 
	 */
    bool eventMethod (CWorkPackIt WorkPack, CWorkPackIt*& pWorkDone)
    {
      pWorkDone = new CWorkPackIt ();

	  *pWorkDone = WorkPack;
	  pWorkDone->m_isSendResult = true;
	  pWorkDone->m_isNotifyWithCallback = true;
	  pWorkDone->m_isUseDefaultQ = false;
	  pWorkDone->m_ptheWorkDoneQ = &m_theEventQueue;
	  return true;
    }  // periodicMethod

    
	int getChangeCount ()
	{
      return m_theCallback.getChangeCount ();
	} // getChangeCount

	int getUpdateCount ()
	{
	  return m_theCallback.getUpdateCount ();
	} // getUpdateCount

    WorkPackItQ* getEventWorkQueue ()
	{
      return &m_theEventQueue;
	} // getEventWorkQueue

	void getDataItem (DataItemPtr& ptheDataItem)
	{
		m_theCallback.getDataItem (ptheDataItem);
	} // getDataItem

}; // class TestIt

/**
 * Test_ThreadIt_insertItem is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * is based on the size of the elements in the queue buffer as returned by the Size 
 * method.
 */
TEST (Test_ThreadIt_startWork)
{
  bool isSuccess = false;
  ULONG theWorkId = 0;
  CTestIt theTest;
  CWorkPackIt* ptheWork;
  CWorkPackIt* ptheResult;
  
  log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestThreadIt"));
  logger->info ("Testing - Test_ThreadIt_startWork");

  UNITTEST_TIME_CONSTRAINT (theMaxWait/4);

//  You are not the owner of the data anymore.

  ptheWork = new CWorkPackIt ();
  ptheWork->m_theInstruction = THREADIT_TEST_METHOD; 
  ptheWork->m_isSendResult = true;
  isSuccess = theTest.startWork (ptheWork, theWorkId);
  CHECK (isSuccess);
  ptheResult = theTest.getWork (2000);
  CHECK (ptheResult != NULL);
  CHECK (ptheResult->m_theInstruction == THREADIT_TEST_METHOD);
  CHECK (ptheResult->m_theWorkPackID == theWorkId);
  delete ptheResult;
} // TEST (Test_ThreadIt_startWork)

/**
 * Test_ThreadIt_insertItem is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * is based on the size of the elements in the queue buffer as returned by the Size 
 * method.
 */
TEST (Test_ThreadIt_startWork_with_callback_1)
{
  bool isSuccess = false;
  ULONG theWorkId = 0;
  CTestIt theTest;
  CWorkPackIt* ptheWork;
  CWorkPackIt* ptheResult;
  
  // Always best to let thread it delete the incoming work.


  log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestThreadIt"));
  logger->info ("Testing - Test_ThreadIt_startWork_with_callback_1");

  UNITTEST_TIME_CONSTRAINT (theMaxWait/4); 

  ptheWork = new CWorkPackIt ();
  ptheWork->m_theInstruction = THREADIT_TEST_METHOD; 
  ptheWork->m_isSendResult = true;
  ptheWork->m_isNotifyWithCallback = true;
  isSuccess = theTest.startWork (ptheWork, theWorkId);
  CHECK (isSuccess);
  ptheResult = theTest.getWork (10000);
  CHECK (ptheResult != NULL);
  CHECK (ptheResult->m_theInstruction == THREADIT_TEST_METHOD);
  CHECK (ptheResult->m_theWorkPackID == theWorkId);
  DataItemPtr ptheDataItem;
  theTest.getDataItem(ptheDataItem); // syncronises with observer
  CHECK_EQUAL (1, theTest.getChangeCount ());
  CHECK_EQUAL (0, theTest.getUpdateCount ());
} // TEST (Test_ThreadIt_startWork_with_callback_1)

/**
 * Test_ThreadIt_insertItem is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * is based on the size of the elements in the queue buffer as returned by the Size 
 * method.
 */
TEST (Test_ThreadIt_startWork_with_callback_2)
{
  bool isSuccess = false;
  ULONG theWorkId = 0;
  CTestIt theTest;
  CWorkPackIt* ptheWork;
  CWorkPackIt* ptheResult;
  
  log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestThreadIt"));
  logger->info ("Testing - Test_ThreadIt_startWork_with_callback_2");

  UNITTEST_TIME_CONSTRAINT (theMaxWait/4);

  ptheWork = new CWorkPackIt ();
  ptheWork->m_theInstruction = THREADIT_TEST_METHOD;
  ptheWork->m_isSendResult = false;

	CWorkPackIt* aPack = new CWorkPackIt ();
	ptheWork->m_isObjectInCallback = true;
  ptheWork->m_isNotifyWithCallback = true;
	ptheWork->m_ptheObject = NULL;
	UINT* p = new UINT (100);
	CUintDataItem* ptheItem = new CUintDataItem ();
	ptheWork->m_ptheDataItem.reset ((CUintDataItem*)ptheItem);
	ptheItem->setDataItem (p);
  isSuccess = theTest.startWork (ptheWork, theWorkId);
  CHECK (isSuccess);
  ptheResult = theTest.getWork (50);
  CHECK (ptheResult == NULL);
	DataItemPtr m_ptheDataItem;
	theTest.getDataItem (m_ptheDataItem);
  CHECK_EQUAL (1, theTest.getChangeCount ());
  CHECK_EQUAL (0, theTest.getUpdateCount ());

	UINT* pabc = NULL;
	if (m_ptheDataItem != NULL)
	{
		((CUintDataItem*)m_ptheDataItem.get())->getDataItem ((UINT*&)pabc);
		UINT ax;
		ax = *pabc;
		ax++;
	} // if 
} // TEST (Test_ThreadIt_startWork_with_callback_2)

/**
 * Test_ThreadIt_insertItem is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * is based on the size of the elements in the queue buffer as returned by the Size 
 * method.
 */
TEST (Test_ThreadIt_startWork_with_periodic_method)
{
  int theCount = 0;
  bool isSuccess = false;
  ULONG theWorkId = 0;
  CTestIt theTest;
  CWorkPackIt* ptheResult;
  
  log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestThreadIt"));
  logger->info ("Testing - Test_ThreadIt_startWork_with_periodic_method");

  UNITTEST_TIME_CONSTRAINT (theMaxWait * 2);

  theTest.setPeriodicMethod ((PeriodicMethodType)&CTestIt::periodicMethod);
  Sleep (600);
  do 
  {
    ptheResult = theTest.getWork (50);
	if (ptheResult != NULL)
	{
	  theCount++;
    DataItemPtr ptheDataItem;
    theTest.getDataItem(ptheDataItem); // syncronises with observer
	} // if 
  } while (ptheResult != NULL);
  CHECK (theCount > 4);
  CHECK (theTest.getChangeCount () > 4);
  CHECK_EQUAL (0, theTest.getUpdateCount ());
} // TEST (Test_ThreadIt_startWork_with_periodic_method)

/**
 * Test_ThreadIt_startWork_with_event_method is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * is based on the size of the elements in the queue buffer as returned by the Size 
 * method.
 */
TEST (Test_ThreadIt_startWork_with_event_method)
{
  int theCount = 0;
  bool isSuccess = false;
  ULONG theWorkId = 0;
  ULONG theEventId = 2;
  HANDLE theEventHandle;
  CTestIt theTest;
  CWorkPackIt* ptheResult;
  WorkPackItQ* ptheQueue;
  
  log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestThreadIt"));
  logger->info ("Testing - Test_ThreadIt_startWork_with_event_method");

  // Handle that is signalled when the event is simulated to occur. 
  theEventHandle = CreateSemaphore (NULL, 0, LONG_MAX, NULL);


  UNITTEST_TIME_CONSTRAINT (theMaxWait * 2);

  theTest.setEventMethod (theEventId, (EventMethodType)&CTestIt::eventMethod, theEventHandle);
  Sleep (200);
  // Now test the execution of the event. Send theMaxEvents signals.
  ReleaseSemaphore (theEventHandle, theMaxEvents, NULL);
  // Let the work take place. 
  Sleep (250);
  // Make sure we get that many events back.
  ptheQueue = theTest.getEventWorkQueue ();
  CHECK (ptheQueue != NULL);
  do 
  {
    ptheResult = ptheQueue->waitItem (50);
	if (ptheResult != NULL)
	{
	  theCount++;
    DataItemPtr ptheDataItem;
    theTest.getDataItem(ptheDataItem); // syncronises with observer
	} // if 
  } while (ptheResult != NULL);
  CHECK (theCount == theMaxEvents);
  CHECK (theTest.getChangeCount () == theMaxEvents);
  CHECK_EQUAL (0, theTest.getUpdateCount ());
} // TEST (Test_ThreadIt_startWork_with_event_method)

/**
 * Test_ThreadIt_startWork_with_event_method is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * is based on the size of the elements in the queue buffer as returned by the Size 
 * method.
 */
TEST (Test_ThreadIt_startWork_with_many_event_methods)
{
  int theCount = 0;
  bool isSuccess = false;
  ULONG theWorkId = 0;
  ULONG theEventId = 2;
  HANDLE theEventHandles[CThreadIt::MAX_EVENT_METHODS];
  CTestIt theTest;
  CWorkPackIt* ptheResult;
  WorkPackItQ* ptheQueue;
  
  log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestThreadIt"));
  logger->info ("Testing - Test_ThreadIt_startWork_with_many_event_methods");

  // Handle that is signalled when the event is simulated to occur. 
  for (int i = 0; i < CThreadIt::MAX_EVENT_METHODS; i++)
  {
    theEventHandles[i] = CreateSemaphore (NULL, 0, LONG_MAX, NULL);
  } // for 
  for (int i = 0; i < CThreadIt::MAX_EVENT_METHODS; i++)
  {
    isSuccess = theTest.setEventMethod (theEventId, (EventMethodType)&CTestIt::eventMethod, theEventHandles[i]);
    CHECK_EQUAL (theEventId, i);
    CHECK_EQUAL (isSuccess, true);
  } // for 
  isSuccess = theTest.setEventMethod (theEventId, (EventMethodType)&CTestIt::eventMethod, theEventHandles[0]);
  CHECK_EQUAL (isSuccess, false);
  CHECK_EQUAL (theEventId, CThreadIt::MAX_EVENT_METHODS - 1);

  UNITTEST_TIME_CONSTRAINT (theMaxWait * 2);
  Sleep (200);
  // Now test the execution of the event. Send theMaxEvents signals to each handler. This means that there will
  // be MAX_EVENT_METHODS * theMaxEvents returned results.
  for (int i = 0; i < CThreadIt::MAX_EVENT_METHODS; i++)
  {
    ReleaseSemaphore (theEventHandles[i], theMaxEvents, NULL);
  } // for 

  // Let the work take place. 
  Sleep (250);
  // Make sure we get that many events back.
  ptheQueue = theTest.getEventWorkQueue ();
  CHECK (ptheQueue != NULL);
  do 
  {
    ptheResult = ptheQueue->waitItem (50);
	if (ptheResult != NULL)
	{
	  theCount++;
    DataItemPtr ptheDataItem;
    theTest.getDataItem(ptheDataItem); // syncronises with observer
	} // if 
  } while (ptheResult != NULL);
  CHECK_EQUAL (theCount, CThreadIt::MAX_EVENT_METHODS * theMaxEvents);
  CHECK_EQUAL (theTest.getChangeCount (), CThreadIt::MAX_EVENT_METHODS * theMaxEvents);
  CHECK_EQUAL (0, theTest.getUpdateCount ());
} // TEST (Test_ThreadIt_startWork_with_many_event_methods)

/**
 * Test_ThreadIt_threadItMessage is a unit test that tests the use
 * if CThreadItMessage. 
 */
/*
TEST (Test_ThreadIt_threadItMessage)
{
  int theCount = 0;
  bool isSuccess = false;
  ULONG theWorkId = 0;
  ULONG theEventId = 2;
  HANDLE theEventHandles[CThreadIt::MAX_EVENT_METHODS];
  CTestIt theTest;
  CWorkPackIt* ptheResult;
  WorkPackItQ* ptheQueue;
	CThreadItMessage* p_theMsg = NULL;
  
  log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestThreadIt"));
  logger->info ("Testing - Test_ThreadIt_threadItMessage");

	// TO BE IMPLEMENTED.



} // TEST (Test_ThreadIt_threadItMessage)
*/

