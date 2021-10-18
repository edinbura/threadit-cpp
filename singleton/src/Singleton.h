// CSingleton.h: interface for the CSingleton class.
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


#if !defined(AFX_CSINGLETON_H__AF52A115_6900_41FC_8BA5_EDE8241EE8C9__INCLUDED_)
#define AFX_CSINGLETON_H__AF52A115_6900_41FC_8BA5_EDE8241EE8C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SingletonBase.h"
#include "LockableObject.h"
#include <log4cpp/Category.hh>

namespace PDE
{
	class CSingleton : public CSingletonBase
	{
	public:
		virtual ~CSingleton();
		//Returns Critical Section key from Singleton
		const CLockableObject*	getKey();
		void debugShow();

		//////////////////////////////////////////////////
		// Test attributes
		void setTestInt( const int i );
		int getTestInt(); 
		//////////////////////////////////////////////////			

	protected:
		CSingleton();			

		//m_iTest is only an attribute for the benefit of test stub
		int		m_iTest;

		//ms_pOnlyInstance holds the only Instance of this Singleton
		static CSingleton* ms_pOnlyInstance;

		//Only needs to be static because we are locking something classwide rather then instance wide
		static	CLockableObject ms_key;//CCriticalSection ms_key;

		//Flags if we are allowed to create an instance if necessary.	 FALSE when Singleton destroyed, but can be reset via mutators to allow another instance.
		//	This flag means there can only ever be 0..1 Instance in memory at one time, and prevents recreation after destruction by mistake.
		//The flag is made volatile to prevent the result being erronously cached in the registers by the compiler while another thread may have changed its value.
		static volatile BOOL ms_bCanCreateInstance;

		//volatile - Member data is loaded from memory each time it is accessed; disables certain optimizations.
		// - this ensures that the compiler does not optimise out our cheap lock hint, before going on to do the more expensive CRITICAL_SECTION mutex lock
		static volatile BOOL ms_bSingletonCreatedFlag;
						
		/** m_ptheLogger is the logger used to log information and errors for each instance of this class */
		static log4cpp::Category* m_ptheLogger;

	public:			
		//Implement Generic instance grabbing techique from base
		void* getSingletonInstance(void** pOut);			
		static	CSingleton* getInstance();			

		//If the singleton has been destroyed, it will prevent further versions of itself created unless told otherwise.
		//This is to prevent methods erronously calling Instance after destruction causing a Singleton to be created that may never be cleaned up.
		static	inline void enableInstanceCreation(BOOL bEnabled=TRUE) {ms_bCanCreateInstance = bEnabled;}
		//Returns TRUE if further instance creation is permitted
		static	inline BOOL canCreateInstance() {return ms_bCanCreateInstance;}
	}; // class CSingleton

} // end namespace PDE

#endif // !defined(AFX_CSINGLETON_H__AF52A115_6900_41FC_8BA5_EDE8241EE8C9__INCLUDED_)
