/**
 * Title: TestActive
 * Description: TestActive contains unit tests for the CActive class. 
 * The purpose of these tests is to excercise as many methods of the CActive
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
#include <active.h>
#include <log4cpp/Category.hh>
#include <vector>

/** The number of elements to test. */
const int theMaxSize = 2010;

/**
 * class CMyThread extends class CActive. CMyThread thus represents a 
 * thread of execution that is supported by CActive.Each of the methods
 * of CActive can be tested using instances of this class. 
 */
class CMyThread : public CActive
{
protected:
	UINT m_theId;
	volatile long m_theValue;
	volatile bool m_isThreadRunning;
private:
	log4cpp::Category& m_theLogger;

public: 
	CMyThread (UINT theId) :  CActive("threadit.TestActive")
		,m_theId(0)
		,m_theValue(0L)
		,m_isThreadRunning(false)
		,m_theLogger( log4cpp::Category::getInstance("threadit.TestActive") )
	{
		m_theId = theId;
		m_theValue = 0;
		m_isThreadRunning = true;
		m_theLogger.infoStream() << "CMyThread ctor: id='" << m_theId << "', value='"  << m_theValue << "' CActive instance '" << getThreadInstance() << "'";
	} // constructor CMyThread

	  CMyThread (UINT theId, int theIntValue) :  CActive("threadit.TestActive")
		,m_theId(theId)
		,m_theValue(theIntValue)
		,m_isThreadRunning(false)
		,m_theLogger( log4cpp::Category::getInstance("threadit.TestActive") )
	{
		m_theId = theId;
		m_theValue = theIntValue;
		m_isThreadRunning = true;
		m_theLogger.infoStream() << "CMyThread ctor: id='" << m_theId << "', value='"  << m_theValue << "' CActive instance '" << getThreadInstance() << "'";
	} // constructor CMyThread

	~CMyThread ()
	{
		m_isThreadRunning = false;
		waitForThreadToStop ();
		// m_theThreadCount-- after CActive is destroyed
		m_theLogger.infoStream() << "CMyThread dtor: id='" << m_theId << "', value='"  << m_theValue << "' CActive instance '" << getThreadInstance() << "'";
	} // destructor ~CMyThread

	/**
	 * Method threadRoutine represents the thread of execution of the CMyThread
	 * active class.
	 */
	void threadRoutine ()
	{
		while (m_isThreadRunning)
		{
			m_theValue++;					 
			Sleep (10);
		} // while (m_isThreadRunning)
	} // threadRoutine

	void stopThread ()
	{
		m_isThreadRunning = false;
		waitForThreadToStop ();
	} // stopThread

	void set (int theIntValue)
	{
		m_theValue = theIntValue;
	} // set
		
	int getValue ()
	{
		return m_theValue;
	} // getValue

}; // class CMyThread

/**
 * Test_Active_thread is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * is based on the size of the elements in the queue buffer as returned by the Size 
 * method.
 */
TEST (Test_Active_thread)
{
	int testIteration = 1;
	int testThreadCount = 100;// theMaxSize;

	int theValue = 0, i;
	std::vector< std::shared_ptr<CMyThread> > theThreads;
	theThreads.resize(testThreadCount);
	std::vector< std::shared_ptr<CMyThread> >::iterator iter;
	std::vector< std::shared_ptr<CMyThread> >::reverse_iterator rev_iter;

	std::vector<log4cpp::Category*>::size_type categories_t = log4cpp::Category::getCurrentCategories()->size();
	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("threadit.TestActive"));
	logger->notice (m_details.testName);

	while (testIteration--)
	{
	unsigned int activeThreads = CActive::getThreadCount();
	
	for (i = 0, iter = theThreads.begin();
		iter != theThreads.end(); 
		++iter)
	{
		iter->reset(new CMyThread (++i));
		if ( (*iter)->getThreadId() )
			(*iter)->startThread ();
	} // for 
	logger->noticeStream() << testThreadCount << " threads created";

	Sleep (10*testThreadCount);

	// Make sure that every thread has run.
	for (iter = theThreads.begin();
		iter != theThreads.end(); 
		++iter)
	{
		Sleep (10);
		theValue = (*iter)->getValue ();
		CHECK (theValue > 10);
	} // for 
	// Make sure that every thread has a thread identity value.
	for (iter = theThreads.begin();
		iter != theThreads.end(); 
		++iter)
	{
		theValue = (*iter)->getThreadId ();
		CHECK (theValue != 0);
	} // for 

	// added partly to plug a memory leak, 
	// also to test destroying a running thread
	for (rev_iter = theThreads.rbegin();
		rev_iter != theThreads.rend(); 
		++rev_iter)
		//i = testThreadCount - 1; i >= 0; --i)
	{
		rev_iter->reset();
	}

	logger->noticeStream() << testThreadCount << " threads deleted";
	// This only counts CActive threads and the test will fail if any other CActive threads
	// are started /stopped outside of this Unit Test.
	CHECK_EQUAL(activeThreads, CActive::getThreadCount());
	logger->critStream() << "log4cpp Categories count was='" << categories_t << "' now ='" 
		<< (UINT) log4cpp::Category::getCurrentCategories()->size() << "'"; 
	} // while 
	logger->notice (m_details.testName);
} // TEST (Test_Active_thread)

/**
 * Test_Active_stop_thread is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * is based on the size of the elements in the queue buffer as returned by the Size 
 * method.
 */
TEST (Test_Active_stop_thread)
{
	int testIteration = 1;
	int testThreadCount = 100;/// theMaxSize;

	int theValue = 0, i;
	CMyThread* ptheThread = NULL;
	std::vector< std::shared_ptr<CMyThread> > theThreads;
	theThreads.resize(testThreadCount);
	std::vector< std::shared_ptr<CMyThread> >::iterator iter;
	std::vector< std::shared_ptr<CMyThread> >::reverse_iterator rev_iter;

	std::vector<log4cpp::Category*>::size_type categories_t = log4cpp::Category::getCurrentCategories()->size();
	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("threadit.TestActive"));
	logger->notice (m_details.testName);

	while (testIteration--)
	{
	unsigned int activeThreads = CActive::getThreadCount();

	for (i = 0, iter = theThreads.begin();
		iter != theThreads.end(); 
		++iter)
	{
		iter->reset(new CMyThread (++i));
		if ( (*iter)->getThreadId() )
			(*iter)->startThread ();
	} // for 
	logger->noticeStream() << testThreadCount << " threads created";

	Sleep (10*testThreadCount);

	for (iter = theThreads.begin();
		iter != theThreads.end(); 
		++iter)
	{
		Sleep (10);
		theValue = (*iter)->getValue ();
		CHECK (theValue > 10);
	} // for 
	for (iter = theThreads.begin();
		iter != theThreads.end(); 
		++iter)
	{
		Sleep (10);
		(*iter)->stopThread ();
	} // for 
	// Check that every thread has stopped.
	for (iter = theThreads.begin();
		iter != theThreads.end(); 
		++iter)
	{
		Sleep (10);
		CHECK_EQUAL ((*iter)->isThreadRunning (), false);
	} // for 
	logger->noticeStream() << testThreadCount << " threads stopped";

	for (rev_iter = theThreads.rbegin();
		rev_iter != theThreads.rend(); 
		++rev_iter)
		//i = testThreadCount - 1; i >= 0; --i)
	{
		rev_iter->reset();
	}

	logger->noticeStream() << testThreadCount << " threads deleted";
	// This only counts CActive threads and the test will fail if any other CActive threads
	// are started /stopped outside of this Unit Test.
	CHECK_EQUAL(activeThreads, CActive::getThreadCount());
	logger->critStream() << "log4cpp Categories count was='" << categories_t << "' now ='" 
		<< (UINT) log4cpp::Category::getCurrentCategories()->size() << "'"; 
	} // while 
	logger->notice (m_details.testName);
} // TEST (Test_Active_stop_thread)

