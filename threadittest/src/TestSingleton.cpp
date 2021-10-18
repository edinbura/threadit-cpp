/**
 * Title: TestSingleton
 * Description: TestSingleton contains unit tests for the TSingleton class. 
 * The purpose of these tests is to excercise as many methods of the TSingleton
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
#include <TSingleton.h>
#include <LockableObject.h>
#include <SingletonObserver.h>
#include <log4cpp/Category.hh>
#include <log4cpp/BasicConfigurator.hh>

using namespace PDE;

/** The number of elements to test. */
const int theMaxSize = 100;

class CSingletonTester : public TSingleton<CSingletonTester>, public CLockableObject
{
private:
	int m_theValue;

	// You must make the template base a friend so it can use the protected constructor
	friend class TSingleton<CSingletonTester>;

protected:
	// Constructor must be private or protected so no other object can initiate its creation.
	CSingletonTester () 
	{
	} // constructor CSingletonTester 

public:		
	~CSingletonTester ()
	{
	} // destructor ~CSingletonTester;

		/**
		 * Method setTestValue is an example of using the locking mechanism built into the singleton
		 * to do thread-safe attribute changes.
		 */
	void setTestValue (int i)
	{		
		// Lock this section as we are changing an attribute		
		TKeyChain<CLockableObject> lock (this);
		// Set the attribute
		m_theValue = i;		
	} // setTestValue

	/**
	 * Method setTestValue is an example of using the locking mechanism built into the singleton
	 * to do thread-safe attribute changes.
	 */
	int getTestValue ()
	{ 
		// Lock this section as we are getting the attribute		
		TKeyChain<CLockableObject> lock (this);		
		return m_theValue; 
	} // getTestValue

	/**
	 * Method DebugShow writes out info that may be useful information to the stack.
	 */
	void DebugShow()
	{
		// Lock this section as we are displaying the attribute		
		TKeyChain<CLockableObject> lock (this);		

		m_ptheLogger->debug("CSingletonTester::DebugShow - this=0x%08X TestAttribute=%d ThreadID=%d\n", this, m_theValue, ::GetCurrentThreadId ());
	} // DebugShow

}; // class CSingletonTester

/**
 * Test_Singleton_createSingleton is a unit test that gets a set number of instances 
 * of the singleton and updates the test value. The method checks that only one instance
 * of the singleton has been created. The test also checks that the singleton is 
 * released on completion.
 */
TEST (Test_Singleton_createSingleton)
{
	CSingletonTester* theSingletons[theMaxSize];

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("Test_Singleton_createSingleton"));
	logger->info ("Testing - Test_Singleton_createSingleton");

	for (int i = 0; i < theMaxSize; i++)
	{
		theSingletons[i] = CSingletonTester::getInstance ();
	} // for 
	for (int i = 0; i < theMaxSize; i++)
	{
		theSingletons[i]->setTestValue (i);
	} // for 
	for (int i = 0; i < theMaxSize; i++)
	{
		theSingletons[i]->setTestValue (i);
		CHECK_EQUAL (theSingletons[i]->getTestValue (), i);
	} // for 
	theSingletons[0]->setTestValue (theMaxSize);
	// As a final check see that each instance has the same value.
	for (int i = 0; i < theMaxSize; i++)
	{
		CHECK_EQUAL (theSingletons[i]->getTestValue (), theMaxSize);
	} // for 
	//The Observer must be called to delete the memory taken up by the Singleton
	CSingletonObserver::getInstance()->removeAllAndDie ();
} // TEST (Test_Singleton_createSingleton)


