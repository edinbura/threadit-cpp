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
 * Title: TestThreadItObserver
 * Description: TestThreadItObserver contains unit tests for the CThreadItNotifier and CThreadItObserver 
 * classes. The purpose of these tests is to excercise as many methods of the CThreadItObserver and CThreadItNotifier
 * class as possible so that the class behaviour is correct and the class 
 * can be regression tested as part of an automated test suite. 
 *
 * Copyright: Copyright (c) 2008 Ashkel Software 
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2010/05/21 02:38:06 $
 */

#include "stdafx.h"
#include "UnitTest++.h"
#include <log4cpp/Category.hh>
#include <log4cpp/BasicConfigurator.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include "threaditnotifier.h"
#include "threaditobserver.h"
#include "threadit.h"


#define THREADIT_OBSERVER_TEST_METHOD 1
#define THREADIT_NOTIFIER_TEST_METHOD 1

//CClient is the observing class inheriting from CThreadItObserver and 
//it has a method called digest which gets called once the Notifier notifies
class CClient:public CThreadIt,public CThreadItObserver
{
    protected: 

		 log4cpp::Category* m_pthelogger;
	//	 CThreadItObserver *m_ptheObserver;

    public:

		
		CClient()		{
			m_ptheLogger = &(log4cpp::Category::getInstance ("CClient"));
		   // Setup all the work packages and associated messages..
  		   setWorkerMethod ((WorkerMethodType)&CClient::digest, THREADIT_OBSERVER_TEST_METHOD);
		   setPeriod (100);
		}

		~CClient()
		{
			// Terminate the work processing thread.
			stopThread ();
			// Wait for the thread to stop.
			 waitForThreadToStop ();
		}

		bool digest(CWorkPackIt* pWorkPack, CWorkPackIt*& pWorkDone);

};

bool CClient::digest(CWorkPackIt* pWorkPack, CWorkPackIt*& pWorkDone)
{
      m_ptheLogger->debug("Worker method digesting the notifications");
	  return true;
}

//CServer is the notifying class which registers the observers through attach
//and once there is a change in subject it calls notify to indicate to observers

class CServer:public CThreadIt,public CThreadItNotifier
{
  protected:
	
	 
  public:

	  CServer ()
      {
		   m_ptheLogger = &(log4cpp::Category::getInstance ("CCServer"));
		   // Setup all the work packages and associated messages..
  		    setPeriod (100);
	
		 
      }  // Constructor

	  ~CServer()
	  {
      // Terminate the work processing thread.
      stopThread ();
      // Wait for the thread to stop.
      waitForThreadToStop ();
      } //Destructor

	  
};

//Test Class contains both Server and Client instance which start thier threads
//Client registers to Server as observer
//Then manually the notify is called to test the observer is getting notified
class Test_Class
{
private:

    CServer *m_ptheServer;
	CClient *m_ptheClient;

public:

	Test_Class();

	~Test_Class();

	void attach();

	void callNotify();

	void attachObserver();

	void start();
};


Test_Class::Test_Class()
{
	m_ptheServer = new CServer();
	m_ptheClient = new CClient();
}

Test_Class::~Test_Class()
{
	delete m_ptheServer;
	delete m_ptheClient;
}

void Test_Class::attach()
{	
//    m_ptheServer->attach(m_ptheClient,THREADIT_OBSERVER_TEST_METHOD);
}

void Test_Class::callNotify()
{
	m_ptheServer->notify();
}


TEST (Test_ThreadIt_Observer)
{
	Test_Class *ptheTestClass=new Test_Class();

    ptheTestClass->attach();
	ptheTestClass->callNotify();

	delete ptheTestClass;
}

