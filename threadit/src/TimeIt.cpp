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
 * Title: CTimeIt
 * Description: Class CTimeIt is used to measure elapsed time based on the
 * system tickcount. The methods in this class support an application to
 * peform measurements on processing time.
 *
 * Copyright: Copyright (c) 2008 Ashkel Software
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2006/12/19 02:38:06 $
 */

// Includes
#include "stdafx.h"
#include "TimeIt.h"
#include "limits.h"

/**
 * Method CTimeIt is the constructor for the class. This method initialises the
 * member variables for use.
 */
CTimeIt::CTimeIt()
{
	// Initialise the timing variables.
	m_TStart = 0;
	m_TStop	 = 0;
	m_IsTiming		= FALSE;
	m_TimeAllowed = 0;
	// Setup a Mutex for the timing information.
	m_TimeAccess = CreateMutex (NULL, FALSE, NULL);
} // constructor CTimeIt

/**
 * Method ~CTimeIt is	 the destructor for the class. This method frees any resources
 * allocated for use.
 */
CTimeIt::~CTimeIt()
{
	// Close resources in use.
	if (m_TimeAccess != NULL)
		CloseHandle (m_TimeAccess);
} // destructor CTimeIt

/**
 * Method IsExpired is provided for use in the WorkerMethodType function
 * to determine if the time allowed for processing has elapsed or not. If
 * the time available has elpased then the worker method should stop
 * processing and return the status WORKDONE_TIME_OUT in the cWorkDoneIt
 * result.
 * Elapsed returns the time elapsed so far for work execution in milliseconds.
 * If return value of zero and the method returns TRUE indicates that timing is
 * not currently in progress.
 * Method Expired returns true if the timer has expired.
 */
bool CTimeIt::isExpired (DWORD& Elapsed) const
{
	bool Expired;

	// Secure access to the timing variables.
	WaitForSingleObject (m_TimeAccess, INFINITE);
	// Get the time elapsed so far.
	timeElapsed (Elapsed);
	// Check if there is still time available.
	if (m_TimeAllowed == 0)
		Expired = FALSE;
	else
		Expired = (Elapsed > m_TimeAllowed);
	// Release the mutex.
	ReleaseMutex (m_TimeAccess);
	// Return the method result.
	return Expired;
} // IsExpired

/**
 * Method StartTiming is called to record the start of work execution timing.
 * TimeAllowed specifies the time allocated for work to be executed.
 * A value of zero indicates that there is no timing restriction.
 */
void CTimeIt::startTiming (DWORD TimeAllowed)
{
	// Secure access to the timing variables.
	WaitForSingleObject (m_TimeAccess, INFINITE);
	// Record the time allowed for work execution.
	if (TimeAllowed == INFINITE)
		m_TimeAllowed = 0;
	else
		m_TimeAllowed = TimeAllowed;
	// Start the timer.
	m_TStart = GetTickCount ();
	m_TStop = m_TStart + m_TimeAllowed;
	// Timing is in progress.
	m_IsTiming = TRUE;
	// Release the mutex.
	ReleaseMutex (m_TimeAccess);
} // StartTiming

/**
 * Method StopTiming is called to record the end of work execution timing.
 * Elapsed returns the time elapsed so far for work execution in milliseconds.
 */
void CTimeIt::stopTiming (DWORD& Elapsed)
{
	// Secure access to the timing variables.
	WaitForSingleObject (m_TimeAccess, INFINITE);
	// Get the time Elapsed so far.
	timeElapsed (Elapsed);
	// Stop timing.
	m_IsTiming = FALSE;
	// Release the mutex.
	ReleaseMutex (m_TimeAccess);
} // StopTiming

/**
 * Method TimeElapsed calculates how much time has so far elapsed since timing
 * was started.
 * Elapsed returns the time elapsed since timing was started.
 * Method TimeElapsed returns true if timing is currently in progress.
 */
bool CTimeIt::timeElapsed (DWORD& Elapsed) const
{
	bool IsTiming;
	DWORD theStopTime;

	// Secure access to the timing variables.
	WaitForSingleObject (m_TimeAccess, INFINITE);
	// Start the timer.
	theStopTime = GetTickCount ();
	// Calculate the time elapsed so far.
	// Get the time differences from the start to the end.
	if (theStopTime >= m_TStart)
		// The timer has not wrapped.
		Elapsed = theStopTime - m_TStart;
	else
		// The timer has wrapped around.
		Elapsed = (UINT_MAX - m_TStart) + theStopTime;
	// Indicate if timing is in progress or not.
	IsTiming = m_IsTiming;
	// Release the mutex.
	ReleaseMutex (m_TimeAccess);
	// Return result.
	return IsTiming;
} // TimeElapsed

/** 
 * Method getRemaining returns the number of milliseconds remaining to time. 
 */
unsigned long CTimeIt::getRemaining () const
{
	DWORD theElapsed = 0;
	DWORD theStopTime = 0;
	unsigned long theRemaining = 0;

	// Secure access to the timing variables.
	WaitForSingleObject (m_TimeAccess, INFINITE);
	// Start the timer.
	theStopTime = GetTickCount ();
	// Calculate the time elapsed so far.
	// Get the time differences from the start to the end.
	if (theStopTime >= m_TStart)
		// The timer has not wrapped.
		theElapsed = theStopTime - m_TStart;
	else
		// The timer has wrapped around.
		theElapsed = (UINT_MAX - m_TStart) + theStopTime;
  // Get the time remaining.
	theRemaining = m_TimeAllowed - theElapsed;
	if (static_cast<long>(theRemaining) < 0)
	{
		theRemaining = 0;
	} // if 
	// Release the mutex.
	ReleaseMutex (m_TimeAccess);
	// Return result.
	return theRemaining;
} // getRemaining

/**
 * Method reset resets the timing instance so that timing can be restarted without
 * having to create a new instance of this class.
 */
void CTimeIt::reset ()
{
	// Secure access to the timing variables.
	WaitForSingleObject (m_TimeAccess, INFINITE);
	// Reset the variables.
	m_TStart = 0;
	m_TStop	 = 0;
	m_IsTiming = false;
	m_TimeAllowed = 0;
	// Release the mutex.
	ReleaseMutex (m_TimeAccess);
} // Reset



