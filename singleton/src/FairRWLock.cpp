// CFairRWLock.cpp: implementation of the CFairRWLock class.
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
//	This implemenation give readers & writers same priority, based on a 
// first-come-first-served approach
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
#include "FairRWLock.h"



namespace PDE
{
	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	CFairRWLock::CFairRWLock()
		:CRWLockableBase(), m_WriteLock(1), m_uiReadersWaiting(0)
	{

	}

	CFairRWLock::~CFairRWLock()
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
	BOOL CFairRWLock::readLock(void) 
	{
		//dwWaitResult is the result sent back from the wait command
		DWORD dwWaitResult = WAIT_FAILED;

		//Entered a critical section...
		CLockableObject::lock();

		//Check to see if we have at least one writer waiting... 
		while (m_uiWriters > 0) 
		{
			//Check to see if we're the first reader in the line, then increment the readers waiting value
			if (m_uiReadersWaiting++ == 0)
			{
				//Unlock section...
				CLockableObject::unlock();

				//Get the Writers lock (preventing any more writers from starting before we get a turn to read)
				//TODO: Do version with proper waiting, rather then dangerous Infinite
				m_WriteLock.waitSemSignal();

				//We have a lock!

				//Reenter a critical section...
				CLockableObject::lock();

				//If readers waiting is more then 1 (which is us), then tell everyone that reading is ok now
				if (m_uiReadersWaiting>1)
				{
					//Open floodgates, all readers can have a go at going thru! 
					m_ReadGate.open();
				}
			}
			else
			{
				//We're not the first reader in line...

				//Unlock section...
				CLockableObject::unlock();

				//Wait until signaled that we have a lock...
				//TODO: Do version with proper waiting, rather then dangerous Infinite
				m_ReadGate.waitGateOpen();

				//Reenter a critical section...
				CLockableObject::lock();
			}


			//If here we have one less reader waiting...
			m_uiReadersWaiting--;
		}

		//If here we have one more reader with a lock...
		++m_uiReaders;

		//Unlock section...
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
	BOOL CFairRWLock::writeLock(void) 
	{
		// Entered a critical section...
		CLockableObject::lock();
		
		// When m_uiWriters>0, it stops readers getting a lock.
		++m_uiWriters;

		//Unlock section...
		CLockableObject::unlock();
	
		// Wait until the writer's lock is available.	 Only one writer at a time may do it's business!
		// Without any params or wait checking, the wait is infinite - which may cause problems in certain applications...
		// TODO: Improve by using finite time + checking for wiat failures...
		m_WriteLock.waitSemSignal();		
		

		//Re-entering critical section...
		CLockableObject::lock();


		//Close the gate to readers, we have a writer
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
	BOOL CFairRWLock::readUnlock(void)
	{
		// Entered a critical section...
		CLockableObject::lock();

		// We can't unlock readers if we have none locked!
		if (m_uiReaders<1)
			return FALSE;

		// Decrement number of readers, then test if we are the last...
		if (--m_uiReaders == 0)
		{
			//If we are the last reader, release the access block on the writers (as the first reader inline now blocks off writers, as it sort of joins the writers queue)
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
	BOOL CFairRWLock::writeUnlock(void)
	{
		// Entered a critical section...
		CLockableObject::lock();

		// We can't unlock writers if we have none locked!
		if (m_uiWriters<1)
			return FALSE;

		// Decrement writer count.
		m_uiWriters--;

		// Now let whoever's next have a chance		
		m_WriteLock.postSemSignal();	
		
		//Leaving critical section...
		CLockableObject::unlock();

		return TRUE;
	} 
} // end namespace PDE

