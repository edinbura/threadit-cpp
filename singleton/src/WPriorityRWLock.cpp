// CWPriorityRWLock.cpp: implementation of the CWPriorityRWLock class.
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
// Started:		29Nov02
//
// Purpose:		Provide a base class for objects that are thread safe 
//	read & writable via locks.	
//		
/////////////////////////////////////////////////////////////////////////////
//
// Notes:
//
//	This implemenation always gives writers priority over readers
//
//	Classes can inherit from this and gain the thread safe read / write 
// mechanisms simply by calling ReadLock & ReadUnlock + 
// WriteLock & WriteUnlock in pairs.
//
/////////////////////////////////////////////////////////////////////////////
//
// History:
//		v1 - Created
//
/////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "WPriorityRWLock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace PDE
{
	//-----------------------------------------------------------------------------
	// Name:	CWPriorityRWLock Constructor
	// Desc:	Nullifies stuff
	//-----------------------------------------------------------------------------
	CWPriorityRWLock::CWPriorityRWLock()
		:CRWLockableBase(), m_WriteLock(), m_ReadGate()
	{

	}



	//-----------------------------------------------------------------------------
	// Name:	CWPriorityRWLock Destructor
	// Desc:	
	//-----------------------------------------------------------------------------
	CWPriorityRWLock::~CWPriorityRWLock()
	{

	}


	
	//-----------------------------------------------------------------------------
	// Name:	ReadLock
	// Desc:	Call just before reading stuff from this object.
	//			Call ReadUnlock when done.
	//			The thread that calls this will have it's excution halted until the 
	//			lock is successful.
	//
	// Returns: TRUE if successful
	//-----------------------------------------------------------------------------
	BOOL CWPriorityRWLock::readLock(void) 
	{
		//dwWaitResult is the result sent back from the wait command
		DWORD dwWaitResult = WAIT_FAILED;


		//Entered a critical section...
		CLockableObject::lock();

		//Wait if there are writers waiting to write or writing... wait until all the writers have finished...
		while (m_uiWriters > 0) 
		{
			//Unlock the critical section for a moment and call the reading wait stuff.
			//MUST unlock or everything else will be halted waiting for this critical section to be released!
			CLockableObject::unlock();

			//Without any params or wait checking, the wait is infinite - which may cause problems in certain applications...
			//TODO: Improve by using finite time + checking for wiat failures...
			dwWaitResult = m_ReadGate.waitGateOpen();

			//Relock this critical section
			CLockableObject::lock();

			//Loop back around to see if there are any writers...
			Sleep(1);
		}

		//If here we have achieved a lock!
		++m_uiReaders;


		//Leaving critical section...
		CLockableObject::unlock();

		return TRUE;
	}

	

	//-----------------------------------------------------------------------------
	// Name:	WriteLock
	// Desc:	Call just before writing stuff to this object.
	//			Call WriteUnlock when done.
	//			The thread that calls this will have it's excution halted until the 
	//			lock is successful.
	//
	// Returns: TRUE if successful
	//-----------------------------------------------------------------------------
	BOOL CWPriorityRWLock::writeLock(void) 
	{
		// Entered a critical section...
		CLockableObject::lock();
		
		// When m_uiWriters>0, it stops readers getting a lock.
		++m_uiWriters;

		//Leaving critical section...
		CLockableObject::unlock();
	
		// Wait until the writer's lock is available.	 Only one writer at a time may do it's business!
		// Without any params or wait checking, the wait is infinite - which may cause problems in certain applications...
		// TODO: Improve by using finite time + checking for wiat failures...
		m_WriteLock.waitSemSignal();		

		// Entered a critical section...
		CLockableObject::lock();

		// Close the reader barrier, allowing us to write to the object knowing that nothing is going to read from it until we unlock!
		m_ReadGate.close(); 

		//Leaving critical section...
		CLockableObject::unlock();

		return TRUE;
	}




	//-----------------------------------------------------------------------------
	// Name:	ReadUnlock
	// Desc:	Call ReadUnlock when done with section after calling ReadLock
	//
	// Returns: TRUE if successful
	//-----------------------------------------------------------------------------
	BOOL CWPriorityRWLock::readUnlock(void)
	{
		// Entered a critical section...
		CLockableObject::lock();

		// We can't unlock readers if we have none locked!
		if (m_uiReaders<1)
			return FALSE;

		// Decrement number of readers
		m_uiReaders--;

		// If there are no readers waiting, and a writer waiting, let it go...
		if ( (m_uiReaders==0) && (m_uiWriters>0)) 
		{
			m_WriteLock.postSemSignal();
		}

		//Leaving critical section...
		CLockableObject::unlock();

		return TRUE;
	}



	//-----------------------------------------------------------------------------
	// Name:	WriteUnlock
	// Desc:	Call WriteUnlock when done with section after calling WriteLock
	//
	// Returns: TRUE if successful
	//-----------------------------------------------------------------------------
	BOOL CWPriorityRWLock::writeUnlock(void)
	{
		// Entered a critical section...
		CLockableObject::lock();


		// We can't unlock writers if we have none locked!
		if (m_uiWriters<1)
			return FALSE;

		// Decrement writer count.
		m_uiWriters--;

		
		//Are there any more writers waiting?
		if (m_uiWriters > 0)	
		{
			//Yep, so let it write...
			m_WriteLock.postSemSignal();	
		}
		else
		{
			//No writers waiting, so open the gate and let all the readers take a look!
			m_ReadGate.open(); 
		}


		//Leaving critical section...
		CLockableObject::unlock();


		return TRUE;
	} 

}// end namespace PDE
