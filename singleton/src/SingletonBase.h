// CSingletonBase.h: interface for the CSingletonBase class.
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

#if !defined(AFX_CSINGLETONBASE_H__4E8AF7B0_753E_4A4F_9E4A_7E55254F083E__INCLUDED_)
#define AFX_CSINGLETONBASE_H__4E8AF7B0_753E_4A4F_9E4A_7E55254F083E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////
// Only define windows.h if not already defined
#if !defined(_INC_WINDOWS)
// This define means that a gina may not compile.	 Below reduces compile time, nothing more
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>	//For the BOOL type, believe it or not
#endif
/////////////////////////////////////////////////
	

namespace PDE
{
	class CLockableObject;

	//Singleton base is a lightweight, almost interface base class for Singletons...
	class CSingletonBase	
	{
	public:
		// Destructor should be virtual and public so it can be more easily destroyed
		virtual ~CSingletonBase ();		

	protected:
		//Constructor must be hidden from public eyes as per singleton pattern.
		CSingletonBase (void);

		//Adds a singleton to the Singleton Instance Manager
		static BOOL addToObserver (CSingletonBase* pCSingletonBase);
		
		//Adds THIS singleton to the Singleton Instance Manager
		BOOL addToObserver ();

		//Returns Critical Section (key) from CSingletonObserver
		static const CLockableObject* getManagerKey ();

		//Returns Critical Section key from Singleton
		virtual const CLockableObject* getKey (){return NULL;}//=0;

	public:
		//Generic instance grabbing techique
		virtual void* getSingletonInstance (void** /* pOut */) {return NULL;};//=0;
	}; // class CSingletonBase

} // namespace PDE

#endif // !defined(AFX_CSINGLETONBASE_H__4E8AF7B0_753E_4A4F_9E4A_7E55254F083E__INCLUDED_)
