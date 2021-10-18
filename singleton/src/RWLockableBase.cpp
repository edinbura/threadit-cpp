// CRWLockableBase.cpp: implementation of the CRWLockableBase class.
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



#include "stdafx.h"
#include "RWLockableBase.h"



namespace PDE
{
	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	CRWLockableBase::CRWLockableBase():
		m_uiWriters(0), m_uiReaders(0)		
	{

	}

	CRWLockableBase::~CRWLockableBase()
	{

	}


} // end namespace PDE
