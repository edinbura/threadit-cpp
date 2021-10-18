// CSingleton.cpp: implementation of the CSingleton class.
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
// Started:		26Nov02
//
// Purpose:		Example Singleton derived from base class.
//		
/////////////////////////////////////////////////////////////////////////////
//
// Notes:
//
// Example implementation of a Singleton without using either templates
//
//	 This version should be used & changed if you want a true 
// Singleton rather then the container template version which just looks after
// an object that could very well be instantiated elsewhere.
//
//	 You should derive from TSingleton really, but if you can't get your head
// around that, then looking through this simpler version should fire a few
// brain cells up!
//
//	 You should not try to extend this class without changing it, as 
// the static variables used limit you to one sort of singleton, unless using
// the template mechanism as TSingleton does.
//
//	 Each type of singleton requires its own static objects, and when 
// inheriting from a base class you share that base class's statics
// among all those who derive from it.
//
/////////////////////////////////////////////////////////////////////////////
//
// History:
//		v1 - Created
//
/////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Singleton.h"
#include "SingletonObserver.h"

#include "LockableObject.h"
#include "KeyChain.h"

namespace PDE
{
	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////
	CSingleton*			 CSingleton::ms_pOnlyInstance=NULL;
	CLockableObject		 CSingleton::ms_key;
	volatile BOOL		 CSingleton::ms_bCanCreateInstance=TRUE;
	volatile BOOL		 CSingleton::ms_bSingletonCreatedFlag=FALSE;
	log4cpp::Category* CSingleton::m_ptheLogger = &(log4cpp::Category::getInstance ("CSingleton"));


	//-----------------------------------------------------------------------------
	// Name:	CSingletonObserver Constructor
	// Desc:	Leightweight, only inits stuff to NULL
	//-----------------------------------------------------------------------------
	CSingleton::CSingleton():CSingletonBase(), m_iTest(0)		
	{
		m_ptheLogger->debug (" CSingleton Constructor called - this=0x%08X ThreadID=%d\n", this, ::GetCurrentThreadId());
	}



	//-----------------------------------------------------------------------------
	// Name:	CSingletonObserver Destructor
	// Desc:	Locks destructor with critical section, kills off static ref to
	//	this instance - then destroys itself.
	//		Also sets m_bCanCreateInstance to FALSE, so no other instances of this
	// Singleton can be created.
	//-----------------------------------------------------------------------------
	CSingleton::~CSingleton()
	{
		m_ptheLogger->debug (" CSingleton Destructor called - this=0x%08X ThreadID=%d\n", this, ::GetCurrentThreadId());

		//Lock this section with this singleton's mutex, so we don't accidently delete ourselves twice or sommit
		//TKeyChain< CLockableObject > lock( ms_key );
		TKeyChain< CLockableObject > lock( &ms_key );

		//PDE 26Nov02 - Set m_bCanCreateInstance flag to false - if the singleton is being destroyed assume the program is going the same way!
		//PDE 28Nov02 - Also set m_bSingletonCreatedFlag to FALSE, as after this DESTRUCTOR has finshed, there will not be this Singleton in memory
		ms_bSingletonCreatedFlag = ms_bCanCreateInstance=FALSE;		

		//We are no longer pointing at anything...
		ms_pOnlyInstance=NULL;
	}


	
	//-----------------------------------------------------------------------------
	// Name:	GetSingletonInstance
	// Desc:	Generic instance grabbing techique
	// Returns: Can return NULL.	Do NOT clean this up, unless not using 
	//			CSingletonObserver
	//-----------------------------------------------------------------------------
	inline void* CSingleton::getSingletonInstance(void** pOut)
	{
		return *pOut = getInstance ();
	}



	//-----------------------------------------------------------------------------
	// Name:	Instance()
	// Desc:	Returns only instance of contained object if available, otherwise 
	//	returns NULL.
	// Returns: Can return NULL.	Do NOT clean this up, unless not using 
	//			CSingletonObserver
	//-----------------------------------------------------------------------------
	CSingleton* CSingleton::getInstance()
	{		
		if( ms_bCanCreateInstance && !ms_bSingletonCreatedFlag )
		{
			//The Booleans say we should check to see if an instance needs to be created.
			//Now we use the more reliable, but time consuming CRITICAL_SECTION lock

			//Lock this section with this singleton's critical section object
			//TKeyChain< CLockableObject > lock( ms_key );
			TKeyChain< CLockableObject > lock( &ms_key );


			//"Double-Checked Locking" - we are now in the Mutex section, which means only one thread at a time should be allowed through here.
			// This means that testing the boolean now will 100% be correct, as this thread of execution can't be preempted out before another completes
			if( ms_bCanCreateInstance && !ms_bSingletonCreatedFlag ) 
			{
				//Create the only Instance of the Singleton
				ms_pOnlyInstance = new CSingleton;

				//Set the instance creation flag
				ms_bSingletonCreatedFlag = ( ms_pOnlyInstance==NULL ? FALSE : TRUE );

				//Add this singleton to be Observed, so it's lifetime can be properly controlled
				addToObserver(ms_pOnlyInstance);				
			}			
		}

		//Could be NULL if we aren't allowed to create a Singleton
		return ms_pOnlyInstance;
	}


	
	//-----------------------------------------------------------------------------
	// Name:	DebugShow()
	// Desc:	Writes out info that may be useful for info to the stack.
	//-----------------------------------------------------------------------------
	void CSingleton::debugShow()
	{
		m_ptheLogger->debug(" CSingleton::DebugShow - this=0x%08X TestAttribute=%d ThreadID=%d\n", this, m_iTest, ::GetCurrentThreadId());
	}



	//-----------------------------------------------------------------------------
	// Name:	GetKey()
	// Desc:	Returns the CriticalSection object that locks / unlocks during
	// Instance and Destruction
	// Returns: Should never be NULL.	 DO NOT CLEAN UP THIS POINTER!
	//
	// TODO:	Does this method really have to be public or even exist?
	//-----------------------------------------------------------------------------
	const CLockableObject*	CSingleton::getKey()
	{
		return &ms_key;
	}



	//-----------------------------------------------------------------------------
	// Name:	SetTestInt()
	// Desc:	Example of using the locking mechanism built into the singleton
	//	to do thread-safe attribute changes
	//-----------------------------------------------------------------------------
	void CSingleton::setTestInt(const int i)
	{
		//Lock this section as we are changing an attribute
		static	CLockableObject cs; 
		cs.lock();		

		//Set the attribute
		m_iTest = i;

		//Could just let it fall out of scope and clean up like elsewhere, but this is tidier!
		cs.unlock();
	}



	//-----------------------------------------------------------------------------
	// Name:	SetTestInt()
	// Desc:	Example of using the locking mechanism built into the singleton
	//	to do thread-safe attribute changes
	//-----------------------------------------------------------------------------
	int CSingleton::getTestInt()
	{ 
		return m_iTest; 
	}
				
} // end namespace PDE
