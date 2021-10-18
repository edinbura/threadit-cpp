// CSemaphoreWrap.h: implementation of the CSemaphoreWrap class.
//
// Copyright (C) 2002	 Paul D. Evans - All rights reserved.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	 02111-1307	 USA
//
//////////////////////////////////////////////////////////////////////////////
//
// Author:		Paul D. Evans
//
// E-Mail comments / suggestions to: paul@enjoysoftware.co.uk
//		It would be nice to hear of any positive uses of this code!
//
// Homepage:	http://www.enjoysoftware.co.uk
//
// Started:		28Nov02
//
// Purpose:		Implement a wrapped Semaphore type syncronisation object
//		
/////////////////////////////////////////////////////////////////////////////
//
// Notes:
//
//	Wraps up lifetime of a Win32 Semaphore object.
//
//	Semaphores are most useful when threads are in a "Producer + Consumer"
// relationship - like a queue of consumers being served by one producer.
//	E.G. a octopus shopping assistant (producer) at a til may be able to 
// serve more then one customer (consumer), but there still may be a queue of
// consumers waiting to be served too.	If the octopus can serve 8 people, then
// and there are only 8 people in the queue, then they will all be served immediately.
//
//	Where Mutexes (CRITICAL_SECTION) has concept of singluar ownership,
// a Semaphore section can have multiple owners.
//
//	A Semeaphore may be thought as a special case Mutex who's count of owners
// is always either 0 or 1.
//
//THUS: A constuctor with...
// ...an InitalCount of 0 means that it will block - similar to if you created a mutex with initial ownership
// ...an InitalCount > 0 (n) means that it will let n number of threads go through without blocking
//
//	A Semaphores count will always be >=0.	At zero it blocks.	At n, it will 
//let n threads thru.
//
//	WaitForSingleObject basically decements the count for the thread.
// if the count is 0 when encountering a WaitForSingleObject, 
// WaitForSingleObject blocks at that point.	Using PostSemSignal releases ownership
//	
//
/////////////////////////////////////////////////////////////////////////////
//
// History:
//		v1 - Created
//
/////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "SemaphoreWrap.h"


namespace PDE
{

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	//An InitalCount of 0 means that it will block - similar to if you created a mutex with initial ownership
	//An InitalCount > 0 (n) means that it will let n number of threads go through without blocking
	const DWORD CSemaphoreWrap::msc_lInitialCount = 0;	
	const DWORD CSemaphoreWrap::msc_lMaxCount = 0x7ffffff;	



	//-----------------------------------------------------------------------------
	// Name:	CSemaphoreWrap constructor
	// Desc:	Populates internal handle with a Created Semaphore
	// Params:	DEF: lInitialCount = msc_lInitialCount = 0
	//			An InitalCount of 0 means that it will block - similar to if you created a mutex with initial ownership
	//			An InitalCount > 0 (n) means that it will let n number of threads go through without blocking
	//			DEF: lMaxAvailable = msc_lMaxCount = 0x7ffffff
	//			lMaxAvailable =		
	//-----------------------------------------------------------------------------
	CSemaphoreWrap::CSemaphoreWrap(long lInitialCount, long lMaxAvailable, LPCTSTR lpName, LPSECURITY_ATTRIBUTES lpSemaphoreAttributes)
		:m_hSemaphore(NULL)
	{	 
		m_hSemaphore = CreateSemaphore( 
			lpSemaphoreAttributes,				// no security attributes
			lInitialCount,	// initial count
			lMaxAvailable,		// maximum count
			lpName);			// named semaphore (unamed if using def parameter NULL)
	}



	//-----------------------------------------------------------------------------
	// Name:	CSemaphoreWrap destructor
	// Desc:	Closes semaphore handle - which should trigger the destruction of
	//	the object, and cause things to be unblocked if stuff still is 
	//	(with any unluck!)
	//-----------------------------------------------------------------------------
	CSemaphoreWrap::~CSemaphoreWrap()
	{
		CloseHandle(m_hSemaphore);
		m_hSemaphore=NULL;
	}



	//-----------------------------------------------------------------------------
	// Name:	WaitSemSignal
	// Desc:	Waits until the Semaphore is signaled (count > 0)
	// Params:	dwMilliseconds - Default INIFINITE.	 Can be time to wait in ms.
	//				- Use 0 if you want it to return straight away if it' blocked
	//					or not.
	// Returns: Result of WaitForSingleObject called on the internal Semaphore handle
	//-----------------------------------------------------------------------------
	DWORD CSemaphoreWrap::waitSemSignal(DWORD dwMilliseconds) 
	{
		DWORD dwWaitForSingleObjectRet = WAIT_FAILED;
		if (m_hSemaphore)
			dwWaitForSingleObjectRet = WaitForSingleObject(m_hSemaphore, INFINITE);

		return dwWaitForSingleObjectRet;
	}



	//-----------------------------------------------------------------------------
	// Name:	PostSemSignal
	// Desc:	Posts a signal to the semaphore, telling it has finished (waking up 
	//	iHowMany threads, and INCREMENTING the count (++count).
	// Params:	DEF: iHowMany = 1.	States how many threads have signaled they have 
	//	finished.
	// Returns: Previous count of semaphores from ReleaseSemaphore
	//-----------------------------------------------------------------------------
	long CSemaphoreWrap::postSemSignal(int iHowMany) 
	{
		long lPreviousCount=0;

		if (m_hSemaphore)
			ReleaseSemaphore(m_hSemaphore, iHowMany, &lPreviousCount);

		return lPreviousCount;
	}
} // end namespace PDE

