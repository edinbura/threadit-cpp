// CSemaphoreWrap.h: interface for the CSemaphoreWrap class.
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


#if !defined(AFX_CSEMAPHOREWRAP_H__65F66F34_F34D_418F_88D4_E5D9A2B8E6BE__INCLUDED_)
#define AFX_CSEMAPHOREWRAP_H__65F66F34_F34D_418F_88D4_E5D9A2B8E6BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////
// Only define windows.h if not already defined
#if !defined(_INC_WINDOWS)
// This define means that a gina may not compile.	 Below reduces compile time, nothing more
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
/////////////////////////////////////////////////


namespace PDE
{
	class CSemaphoreWrap	
	{
	protected:
		HANDLE m_hSemaphore;

	public:
		static const DWORD msc_lInitialCount;
		static const DWORD msc_lMaxCount;

	public:
		CSemaphoreWrap(long lInitialCount=msc_lInitialCount, long lMaxAvailable=msc_lMaxCount, LPCTSTR lpName=NULL, LPSECURITY_ATTRIBUTES lpSemaphoreAttributes=NULL);
		virtual ~CSemaphoreWrap();		

		DWORD waitSemSignal(DWORD dwMilliseconds=INFINITE);

		long postSemSignal(int iHowMany=1);
	};
} // end namespace PDE

#endif // !defined(AFX_CSEMAPHOREWRAP_H__65F66F34_F34D_418F_88D4_E5D9A2B8E6BE__INCLUDED_)
