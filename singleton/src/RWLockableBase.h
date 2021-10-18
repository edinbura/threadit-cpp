// CRWLockableBase.h: interface for the CRWLockableBase class.
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
// Purpose:		Create a lightweight base class / interface for objects 
//	that are thread safe read & writable via locks.
//		
/////////////////////////////////////////////////////////////////////////////
//
// Notes:
//
//	 Classes that inherit must overide the Read(Un)Lock and Write(Un)Lock 
// methods.
//
//	 No of reader / writers with locks are declared as protected here.
//	 Public accessors are provided - no mutators for obvious reasons!
//
/////////////////////////////////////////////////////////////////////////////
//
// History:
//		v1 - Created
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRWLOCKABLEBASE_H__A8E19EC7_BA8C_4B88_AA2D_59FD77A46869__INCLUDED_)
#define AFX_CRWLOCKABLEBASE_H__A8E19EC7_BA8C_4B88_AA2D_59FD77A46869__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "LockableObject.h"


namespace PDE
{

	class CRWLockableBase	 : protected CLockableObject
	{

	protected:
		// Count of writers waiting / holding on for a lock
		unsigned int	m_uiWriters; 


		// Count of readers waiting / holding on for a lock
		unsigned int	m_uiReaders;



	public:
		CRWLockableBase();
		virtual ~CRWLockableBase();

		inline unsigned int getNoReaderLocks() { return m_uiReaders;}
		inline unsigned int getNoWriterLocks() { return m_uiWriters;}


		virtual BOOL readLock(void)=0;
		virtual BOOL writeLock(void)=0;

		virtual BOOL readUnlock(void)=0;
		virtual BOOL writeUnlock(void)=0;

	};

} // end namespace PDE
#endif // !defined(AFX_CRWLOCKABLEBASE_H__A8E19EC7_BA8C_4B88_AA2D_59FD77A46869__INCLUDED_)
