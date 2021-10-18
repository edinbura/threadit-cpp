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
#include <threaditworktargets.h>
#include <log4cpp/Category.hh>
#include <log4cpp/BasicConfigurator.hh>
#include <log4cpp/PropertyConfigurator.hh>

/** The number of elements to test. */
const int theMaxSize = 100;

/**
 * Test_TimeIt_insertItem is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * is based on the size of the elements in the queue buffer as returned by the Size 
 * method.
 */
TEST (Test_worktargets)
{
	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("Test_worktargets"));
	logger->info ("Testing - Test_worktargets");

	CThreadItWorkTargets theWorkTargets;
	CThreadItWorkTargets theWorkTargets1;
	CThreadItWorkTargets theWorkTargets2;
	CThreadIt* pThreadIt = NULL;

	// Add some work targets.
	for (int i = 0; i < theMaxSize; i++)
	{
		pThreadIt = new CThreadIt ();
		theWorkTargets.setWorkTarget (i, i, pThreadIt);
	} // for	 
	CHECK_EQUAL (theMaxSize, theWorkTargets.size ()); 
	theWorkTargets.reset ();
	CHECK_EQUAL (0, theWorkTargets.size ()); 

	// Add some work targets.
	for (int i = 0; i < theMaxSize; i++)
	{
		pThreadIt = new CThreadIt ();
		theWorkTargets1.setWorkTarget (i, i, pThreadIt);
	} // for	 
	for (int i = 0; i < theMaxSize; i++)
	{
		pThreadIt = new CThreadIt ();
		theWorkTargets2.setWorkTarget (i, i, pThreadIt);
	} // for	 
	theWorkTargets = theWorkTargets1 + theWorkTargets2;
	CHECK_EQUAL (theMaxSize, theWorkTargets.size ()); 
	theWorkTargets.reset ();
	CHECK_EQUAL (0, theWorkTargets.size ()); 

	theWorkTargets1.reset ();
	theWorkTargets2.reset ();
	for (int i = 0; i < theMaxSize / 2; i++)
	{
		pThreadIt = new CThreadIt ();
		theWorkTargets1.setWorkTarget (i, i, pThreadIt);
	} // for	 
	for (int i = theMaxSize / 2; i < theMaxSize; i++)
	{
		pThreadIt = new CThreadIt ();
		theWorkTargets2.setWorkTarget (i, i, pThreadIt);
	} // for	 
	theWorkTargets = theWorkTargets1 + theWorkTargets2;
	CHECK_EQUAL (theMaxSize, theWorkTargets.size ()); 
	theWorkTargets.reset ();
	CHECK_EQUAL (0, theWorkTargets.size ()); 

	CThreadItWorkTargets::WorkTargetType* aTarget = NULL;

	// Add some work targets.
	for (int i = 0; i < theMaxSize; i++)
	{
		pThreadIt = new CThreadIt ();
		theWorkTargets.setWorkTarget (i, i, pThreadIt);
	} // for	 
	CHECK_EQUAL (theMaxSize, theWorkTargets.size ()); 
	aTarget = theWorkTargets.getWorkTarget (theMaxSize + 1);
	CHECK (aTarget == NULL);

	aTarget = theWorkTargets.getWorkTarget (theMaxSize - 1);
	CHECK (aTarget->m_theSelectWorkInstruction == theMaxSize - 1);
	CHECK (aTarget->m_theDestWorkInstruction = theMaxSize - 1);
	CHECK (aTarget->m_ptheDestination == pThreadIt);

} // TEST (Test_worktargets)

