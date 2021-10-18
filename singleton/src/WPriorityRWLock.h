// CWPriorityRWLock.h: interface for the CWPriorityRWLock class.
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
//	This implemenation always gives writers priority over readers
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


#if !defined(AFX_CWPRIORITYRWLOCK_H__9D1BCF8A_747A_4E11_BF14_54693059E93C__INCLUDED_)
#define AFX_CWPRIORITYRWLOCK_H__9D1BCF8A_747A_4E11_BF14_54693059E93C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RWLockableBase.h"
#include "SemaphoreWrap.h"
#include "Gate.h"


namespace PDE
{
	class CWPriorityRWLock	: protected CRWLockableBase
	{ 
	protected:
		// Valve used to ensure writing threads do so one at a time
		CSemaphoreWrap	m_WriteLock;			


		// Used to block / wake up all threads wanting to access data
		CGate			m_ReadGate;					



	public:
		CWPriorityRWLock();
		virtual ~CWPriorityRWLock();

		BOOL readLock(void);
		BOOL writeLock(void);

		BOOL readUnlock(void);
		BOOL writeUnlock(void);
	};

} // end namespace PDE


#endif // !defined(AFX_CWPRIORITYRWLOCK_H__9D1BCF8A_747A_4E11_BF14_54693059E93C__INCLUDED_)
