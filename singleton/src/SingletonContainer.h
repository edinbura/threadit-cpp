// TSingletonContainer.h: interface for the TSingletonContainer class.
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

#if !defined(AFX_TSINGLETON_CONTAINER_H__A185CE92_F31D_40C1_AF37_47C16C658F19__INCLUDED_)
#define AFX_TSINGLETON_CONTAINER_H__A185CE92_F31D_40C1_AF37_47C16C658F19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SingletonBase.h"

namespace PDE
{
	template< class T >
		class TSingletonContainer : public CSingletonBase
	{
	public:
		virtual ~TSingletonContainer();
		virtual void debugShow();			


	protected:
		TSingletonContainer();			

		const CLockableObject*	getKey();

		//ms_pOnlyInstance holds the only instance of the actual Singleton container
		static TSingletonContainer< T >* ms_pOnlyInstance;

		//m_actualObject holds the object that the Singleton is containing.
		T m_actualObject;

		//Only needs to be static because we are locking something classwide rather then instance wide
		static	CLockableObject ms_key;

		//Flags if we are allowed to create an instance if necessary.	 FALSE when Singleton destroyed, but can be reset via mutators to allow another instance.
		//	This flag means there can only ever be 0..1 Instance in memory at one time, and prevents recreation after destruction by mistake.
		//The flag is made volatile to prevent the result being erronously cached in the registers by the compiler while another thread may have changed its value.
		static	volatile BOOL ms_bCanCreateInstance;

		//volatile - Member data is loaded from memory each time it is accessed; disables certain optimizations.
		// - this ensures that the compiler does not optimise out our cheap lock hint, before going on to do the more expensive CRITICAL_SECTION mutex lock
		static	volatile BOOL ms_bSingletonCreatedFlag;


	public:
		//Implement Generic instance grabbing techique from base
		void* getSingletonInstance(void** pOut);			
		static	T* getInstance();			

		//If the singleton has been destroyed, it will prevent further versions of itself created unless told otherwise.
		//This is to prevent methods erronously calling Instance after destruction causing a Singleton to be created that may never be cleaned up.
		static	inline void enableInstanceCreation(BOOL bEnabled=TRUE) {ms_bCanCreateInstance = bEnabled}

		//Returns TRUE if further instance creation is permitted
		static	inline BOOL canCreateInstance() {return ms_bCanCreateInstance;}
	};

}

#endif // !defined(AFX_TSINGLETON_CONTAINER_H__A185CE92_F31D_40C1_AF37_47C16C658F19__INCLUDED_)
