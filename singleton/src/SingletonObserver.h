// CSingletonObserver.h: interface for the CSingletonObserver class.
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

#if !defined(AFX_CSINGLETONOBSERVER_H__198F893C_6763_49FC_A1F5_9939D643A6CC__INCLUDED_)
#define AFX_CSINGLETONOBSERVER_H__198F893C_6763_49FC_A1F5_9939D643A6CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "SingletonBase.h"
#include <stack>

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
	class CSingletonObserver : public CSingletonBase
	{
	public:
		virtual ~CSingletonObserver();			

		//Adds a singleton to be observed
		BOOL add (CSingletonBase* pCSingletonBase);			


		//Returns TRUE if stack contains this singleton
		//BOOL Contains(CSingletonBase* pCSingletonBase);


		//Returns TRUE if removal successful, or if it didn't exist anyway
		//BOOL Remove(CSingletonBase* pCSingletonBase);


		//Returns TRUE if removal successful, or if it didn't exist anyway
		BOOL removeAll();


		//Removes everything from stack, then kills itself
		static BOOL removeAllAndDie();

	protected:
		CSingletonObserver();		
		
		const CLockableObject* getKey();

		static CSingletonObserver* ms_pOnlyInstance;

		static CLockableObject ms_key;

		static BOOL m_bCanCreateInstance;

		std::stack<CSingletonBase*> m_stack;

	public:
		//Implement Generic instance grabbing techique from base
		void* getSingletonInstance (void** pOut);			
		static CSingletonObserver* getInstance();			

		//If the singleton has been destroyed, it will prevent further versions of itself created unless told otherwise.
		//This is to prevent methods erronously calling Instance after destruction causing a Singleton to be created that may never be cleaned up.
		static inline void enableInstanceCreation (BOOL bEnabled=TRUE) {m_bCanCreateInstance = bEnabled;}

		//Returns TRUE if further instance creation is permitted
		static inline BOOL canCreateInstance () {return m_bCanCreateInstance;}
	}; // class CSingletonObserver

} // end namespace PDE


#endif // !defined(AFX_CSINGLETONOBSERVER_H__198F893C_6763_49FC_A1F5_9939D643A6CC__INCLUDED_)
