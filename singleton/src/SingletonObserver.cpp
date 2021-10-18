// CSingletonObserver.cpp: implementation of the CSingletonObserver class.
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
// Purpose:		Provide lifetime management for Singletons
//		
/////////////////////////////////////////////////////////////////////////////
//
// Notes:
//
//	 This class loosely follows an Observer style OO Pattern.
//
//	 It manages the lifetime (specifically the destruction) of all Singletons 
//	that register with it.
//
//	 A program that uses Singletons should call RemoveAllAndDie() as it's 
// very last action when the singletons should no longer be in use.
//
/////////////////////////////////////////////////////////////////////////////
//
// History:
//		v1 - Created
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SingletonObserver.h"
#include "LockableObject.h"
#include "KeyChain.h"
using namespace std;

namespace PDE
{
	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////
	BOOL CSingletonObserver::m_bCanCreateInstance = TRUE;
	CLockableObject CSingletonObserver::ms_key;
	CSingletonObserver* CSingletonObserver::ms_pOnlyInstance = NULL;

	//-----------------------------------------------------------------------------
	// Name:	CSingletonObserver Constructor
	// Desc:	Leightweight, only inits stuff to NULL
	//-----------------------------------------------------------------------------
	CSingletonObserver::CSingletonObserver(): CSingletonBase()		
	{
	}

	//-----------------------------------------------------------------------------
	// Name:	CSingletonObserver Destructor
	// Desc:	Locks destructor with critical section, kills off static ref to
	//	this instance - then destroys itself.
	//		Also sets m_bCanCreateInstance to FALSE, so no other instances of this
	// Singleton can be created.
	//-----------------------------------------------------------------------------
	CSingletonObserver::~CSingletonObserver()
	{
		//Lock this section with this singleton's mutex, so we don't accidently delete ourselves twice or sommit
		//TKeyChain< CLockableObject > lock( ms_key );
		TKeyChain< CLockableObject > lock( &ms_key );

		//PDE 26Nov02 - Set m_bCanCreateInstance flag to false - if the singleton is being destroyed assume the program is going the same way!
		m_bCanCreateInstance=FALSE;
		//We are no longer pointing at anything...
		ms_pOnlyInstance=NULL;
	}

	//-----------------------------------------------------------------------------
	// Name:	GetSingletonInstance
	// Desc:	Generic instance grabbing techique
	// Returns: Can return NULL.	Do NOT clean this up, unless not using 
	//			CSingletonObserver
	//-----------------------------------------------------------------------------
	inline void* CSingletonObserver::getSingletonInstance(void** pOut)
	{		
		return *pOut = getInstance();
	}

	//-----------------------------------------------------------------------------
	// Name:	Instance()
	// Desc:	Returns only instance of contained object if available, otherwise 
	//	returns NULL.
	// Returns: Can return NULL.	Do NOT clean this up, unless not using 
	//			CSingletonObserver
	//-----------------------------------------------------------------------------
	inline CSingletonObserver* CSingletonObserver::getInstance()
	{
		if( m_bCanCreateInstance && !ms_pOnlyInstance ) //Race condition could exist here, but this check is cheap...
		{
			//This section should only be entered once, if it's more then once it's racing, which means we should use the more expensive mutex style mechanism
			//Lock this section with this singleton's mutex
			//TKeyChain< CLockableObject > lock( ms_key );
			TKeyChain< CLockableObject > lock( &ms_key );

			if( m_bCanCreateInstance && !ms_pOnlyInstance ) // Double-checked locking
			{
				ms_pOnlyInstance = new CSingletonObserver;				
			}			
		}
		return ms_pOnlyInstance;		
	}



	//-----------------------------------------------------------------------------
	// Name:	Add
	// Desc:	Adds a singleton to be observed
	// Returns: Always returns TRUE at present!
	//----------------------------------------------------------------------------- 
	BOOL CSingletonObserver::add(CSingletonBase* pCSingletonBase)
	{
		//TODO: Cannot check if singleton is already there!
		m_stack.push(pCSingletonBase);
		return TRUE;
	}

	//Returns TRUE if stack contains this singleton
	//BOOL CSingletonObserver::Contains(CSingletonBase* pCSingletonBase)
	//{
	//	return TRUE;
	//}

	//Returns TRUE if removal successful, or if it didn't exist anyway
	//BOOL CSingletonObserver::Remove(CSingletonBase* pCSingletonBase)
	//{
	//	return TRUE;
	//}

	//-----------------------------------------------------------------------------
	// Name:	RemoveAll
	// Desc:	Returns TRUE if removal successful, or if it didn't exist anyway
	// Returns: Always returns TRUE at present!
	//----------------------------------------------------------------------------- 
	BOOL CSingletonObserver::removeAll()
	{
		CSingletonBase* p=NULL;
		
		//Lock this section with this singleton's mutex, so we don't mess with the stack accidently
		//TKeyChain< CLockableObject > lock( ms_key );
		TKeyChain< CLockableObject > lock( &ms_key );

		//Loop while the stack isn't empty
		while( !m_stack.empty() )
		{
			//Set pointer to the top of the stack
			p = m_stack.top();
			//Take top item from the stack
			m_stack.pop();
			//Delete previous stack top
			if (p)	delete p;
		}
		//Just being tidy, don't need to set this to NULL unless some gimp extends this bit of code and tries to use the pointer (duh)
		p=NULL;
		return TRUE;
	}

	//-----------------------------------------------------------------------------
	// Name:	RemoveAllAndDie
	// Desc:	Removes everything from stack, then kills itself.
	//			Should be the last thing application calls, so memory cleanup is
	//	performed.
	// Returns: Always returns TRUE at present!
	//----------------------------------------------------------------------------- 
	BOOL CSingletonObserver::removeAllAndDie()
	{
		CSingletonObserver* pCSingletonObserver = CSingletonObserver::getInstance();

		//If we can get an instance of the manager...
		if (pCSingletonObserver)
		{
			pCSingletonObserver->removeAll();			
			delete pCSingletonObserver;
			pCSingletonObserver=NULL;
			return TRUE;
		}
		else
		{
			//Nothing there?
			return TRUE;
		}
	}

	//-----------------------------------------------------------------------------
	// Name:	GetKey()
	// Desc:	Returns the CriticalSection object that locks / unlocks during
	// Instance and Destruction
	// Returns: Should never be NULL.	 DO NOT CLEAN UP THIS POINTER!
	//
	// TODO:	Does this method really have to be public or even exist?
	//-----------------------------------------------------------------------------
	const CLockableObject*	CSingletonObserver::getKey()
	{
		return &ms_key;
	}

} // end namespace PDE
