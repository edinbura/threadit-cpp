// CSingletonBase.cpp: implementation of the CSingletonBase class.
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
// Purpose:		Interface / Shared methods for Singletons
//		
/////////////////////////////////////////////////////////////////////////////
//
// Notes:
//
//	 Provides base class for singletons - all singletons should inherit from 
// this.	Having this as a base class allows CSingletonObserver to properly
// keep track of specialised singletons, in a generic way.
//
/////////////////////////////////////////////////////////////////////////////
//
// History:
//		v1 - Created
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SingletonBase.h"

#include "LockableObject.h"
#include "SingletonObserver.h"

namespace PDE
{
	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------------
	// Name:	CSingletonBase Constructor
	// Desc:	Leightweight, only inits stuff to NULL
	//-----------------------------------------------------------------------------
	CSingletonBase::CSingletonBase(void)
	{
	}

	//-----------------------------------------------------------------------------
	// Name:	CSingletonBase Destructor
	// Desc:	Here just because
	//-----------------------------------------------------------------------------
	CSingletonBase::~CSingletonBase()
	{
	}
	
	//-----------------------------------------------------------------------------
	// Name:	GetManagerKey
	// Desc:	Generic instance grabbing techique
	// Returns: Can be NULL. Critical Section (key) from SingletonManager.
	//				***DO NOT CLEAN UP MEMORY FROM THIS POINTER!***
	//-----------------------------------------------------------------------------
	const CLockableObject* CSingletonBase::getManagerKey()
	{
		//TODO: Use manager!
		CSingletonObserver *pCSingletonObserver = CSingletonObserver::getInstance();

		if (pCSingletonObserver)
		{
			//Have to upcast to CSingletonBase because GetKey is protected
			return reinterpret_cast<CSingletonBase*> (pCSingletonObserver)->getKey();
		}
		else
		{
			return NULL;
		}		
	}
	
	//-----------------------------------------------------------------------------
	// Name:	AddToObserver
	// Desc:	Adds a singleton to the Singleton Instance Manager
	// Returns: TRUE if successful
	//-----------------------------------------------------------------------------
	BOOL CSingletonBase::addToObserver( CSingletonBase* pCSingletonBase )
	{
		if (pCSingletonBase == NULL)
			return FALSE;

		/////////////////////////////////
		// Add Singleton to manager...
		CSingletonObserver *pCSingletonObserver = CSingletonObserver::getInstance ();
		if (pCSingletonObserver && pCSingletonBase)
		{
			// CSingletonObserver is responsible for deleting ms_pOnlyInstance
			pCSingletonObserver->add (pCSingletonBase);
		}
		else
		{
			return FALSE;
		}
		/////////////////////////////////
		return TRUE;
	}
	


	//-----------------------------------------------------------------------------
	// Name:	AddToObserver
	// Desc:	Adds this singleton to the Singleton Instance Manager
	// Returns: TRUE if successful
	//-----------------------------------------------------------------------------
	BOOL CSingletonBase::addToObserver( )
	{
		return addToObserver(this);
	}

} // namespace PDE
