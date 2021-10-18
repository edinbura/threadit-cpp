/////////////////////////////////////////////////////////////////////////////
//	 TSingletonContainer.cpp: implementation of the TSingletonContainer class.
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
// Purpose:		Provide template for a thread-safe Singleton Container
//		
/////////////////////////////////////////////////////////////////////////////
//
// Notes:
//
//	 The template version of the Singleton can be used to look after one
// single instance of a normally instatiatable object.	So really this is a 
// Singleton container.
//
//	 Class T can be any class that has a public + default contructor that you 
// want to be contained by the singleton.
//
//	 This of course means there could be more then one instance of the
// contained object, but the whole application *should* just look to the 
// singleton's own version of that object.
//
//	 Think of this as a OO & Thread friendly global variable wrapper
//
/////////////////////////////////////////////////////////////////////////////
//
// History:
//		v1 - Created
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SingletonContainer.h"
#include "SingletonObserver.h"


namespace PDE
{
	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////
	template< class T >TSingletonContainer< T >* TSingletonContainer<T>::ms_pOnlyInstance=NULL;
	template< class T >CLockableObject TSingletonContainer<T>::ms_key;
	template< class T >volatile BOOL	TSingletonContainer<T>::ms_bCanCreateInstance=TRUE;
	template< class T >volatile BOOL	TSingletonContainer<T>::ms_bSingletonCreatedFlag=FALSE;



	//-----------------------------------------------------------------------------
	// Name:	TSingletonContainer Constructor
	// Desc:	Leightweight, only inits stuff to NULL
	//-----------------------------------------------------------------------------
	template< class T >TSingletonContainer<T>::TSingletonContainer():
		CSingletonBase(),
		m_actualObject()
	{

	}



	//-----------------------------------------------------------------------------
	// Name:	TSingletonContainer Destructor
	// Desc:	Locks destructor with critical section, kills off static ref to
	//	this instance - then destroys itself.
	//		Also sets m_bCanCreateInstance to FALSE, so no other instances of this
	// Singleton can be created.
	//-----------------------------------------------------------------------------
	template< class T >TSingletonContainer<T>::~TSingletonContainer()
	{
		//Lock this section with this singleton's mutex, so we don't accidently delete ourselves twice or sommit
		CLockGuard< CLockableObject > lock( ms_key );

		//PDE 26Nov02 - Set m_bCanCreateInstance flag to false - if the singleton is being destroyed assume the program is going the same way!
		//PDE 28Nov02 - Also set m_bSingletonCreatedFlag to FALSE, as after this DESTRUCTOR has finshed, there will not be this Singleton in memory
		ms_bSingletonCreatedFlag = ms_bCanCreateInstance=FALSE;

		//We are no longer pointing at anything...
		ms_pOnlyInstance=NULL;
	}



	//-----------------------------------------------------------------------------
	// Name:	GetSingletonInstance()
	// Desc:	Generic Singleton Instance grabbing techique
	// Returns: Can return NULL.	Do NOT clean this up, unless not using 
	//			CSingletonObserver
	//-----------------------------------------------------------------------------
	template< class T > inline void* TSingletonContainer<T>::getSingletonInstance(void** pOut)
	{
		getInstance();
		return *pOut = ms_pOnlyInstance;
	}



	//-----------------------------------------------------------------------------
	// Name:	Instance()
	// Desc:	Returns only instance of contained object if available, otherwise 
	//	returns NULL.
	// Returns: Can return NULL.	Do NOT clean this up, unless not using 
	//			CSingletonObserver
	//-----------------------------------------------------------------------------
	template< class T > inline T* TSingletonContainer<T>::getInstance()
	{		
		//First use inexpensive "Lock Hint" Boolean check...
		if( ms_bCanCreateInstance && !ms_bSingletonCreatedFlag )
		{
			//The Booleans say we should check to see if an instance needs to be created.
			//Now we use the more reliable, but time consuming CRITICAL_SECTION lock

			//Lock this section with this singleton's critical section object
			CLockGuard< CLockableObject > lock( ms_key );

			
			//"Double-Checked Locking" - we are now in the Mutex section, which means only one thread at a time should be allowed through here.
			// This means that testing the boolean now will 100% be correct, as this thread of execution can't be preempted out before another completes
			if( ms_bCanCreateInstance && !ms_bSingletonCreatedFlag ) 
			{
				//Create the only Instance of the Singleton
				ms_pOnlyInstance = new CSingleton< T >;				

				//Set the instance creation flag
				ms_bSingletonCreatedFlag = ( ms_pOnlyInstance==NULL ? FALSE : TRUE );

				//Add this singleton to be Observed, so it's lifetime can be properly controlled
				AddToObserver(ms_pOnlyInstance);
			}			
		}


		//Check to see if we do have an instance to interrogate for the object
		if (!ms_pOnlyInstance)
		{
			//Return the instace held by this singleton
			return &m_pOnlyInstance->m_actualObject;
		}
		else
		{
			//No Singleton to interogate, return NULL
			return NULL;
		}
	}



	//-----------------------------------------------------------------------------
	// Name:	DebugShow()
	// Desc:	Writes out info that may be useful for info to the stack.
	//-----------------------------------------------------------------------------
	template< class T > void TSingletonContainer<T>::debugShow()
	{
		printf(" TSingletonContainer::DebugShow - this=0x%08X Object=0x%08X ThreadID=%d\n",
					 this, &m_actualObject, ::GetCurrentThreadId());
	}



	//-----------------------------------------------------------------------------
	// Name:	GetKey()
	// Desc:	Returns the CriticalSection object that locks / unlocks during
	// Instance and Destruction
	// Returns: Should never be NULL.	 DO NOT CLEAN UP THIS POINTER!
	//
	// TODO:	Does this method really have to be public or even exist?
	//-----------------------------------------------------------------------------
	template< class T > const CLockableObject*	TSingletonContainer<T>::getKey()
	{
		return &ms_key;
	}
} // end namespace PDE
