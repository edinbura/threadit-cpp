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
 * Title: TestTimeIt
 * Description: TestTimeIt contains unit tests for the CTimeIt class. 
 * The purpose of these tests is to excercise as many methods of the CTimeIt
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
#include <timeit.h>
#include <log4cpp/Category.hh>
#include <log4cpp/BasicConfigurator.hh>
#include <log4cpp/PropertyConfigurator.hh>


/**
 * Test_TimeIt_timing is a unit test that adds the specified number of elements
 * into the queue and verfies that the insertItem method adds these elements. The test
 * is based on the size of the elements in the queue buffer as returned by the Size 
 * method.
 */
TEST (Test_TimeIt_timing)
{
	CTimeIt aTimer;
	DWORD theStart = 0;
	DWORD theStop = 0;
	DWORD theElapsedTime = 0;
	bool isTiming = false;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestTimeIt"));
	logger->info ("Testing - Test_TimeIt_timing");

	theStart = GetTickCount ();
	aTimer.startTiming (200);
	Sleep (150);
	isTiming =	aTimer.timeElapsed (theElapsedTime);
	aTimer.stopTiming (theElapsedTime);
	CHECK_EQUAL (aTimer.isExpired (theElapsedTime), false);
	CHECK (isTiming);
	CHECK_CLOSE ((int)theElapsedTime, 150, 25);
	isTiming = aTimer.timeElapsed (theElapsedTime);
	CHECK_EQUAL (isTiming, false);
	CHECK_CLOSE ((int)theElapsedTime, 150, 25);

	aTimer.reset(); 
	isTiming = aTimer.timeElapsed (theElapsedTime);
	CHECK_EQUAL (isTiming, false);

	aTimer.startTiming(200);
	Sleep(125);
	// positive time remaining and not expired yet
	CHECK ( aTimer.getRemaining() > 0);
	CHECK_EQUAL (false, aTimer.isExpired (theElapsedTime));

	Sleep(125);
	// no time remaining now, already expired (but still timing until stopped)
	CHECK_EQUAL ( 0, aTimer.getRemaining() );
	CHECK_EQUAL (true, aTimer.isExpired (theElapsedTime));

	isTiming = aTimer.timeElapsed (theElapsedTime);
	CHECK_EQUAL (true, isTiming);

	aTimer.stopTiming (theElapsedTime);
	CHECK_CLOSE ((int)theElapsedTime, 250, 25);
} // TEST (Test_TimeIt_timing)

