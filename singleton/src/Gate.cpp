// CGate.cpp: implementation of the CGate class.
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
// Started:		28Nov02
//
// Purpose:		Implement a Gate style synchronisation object
//		
/////////////////////////////////////////////////////////////////////////////
//
// Notes:
//
//	Wraps up lifetime of a Win32 Event object, using a Gate style interface
//
//	A gate object can be used to stop all threads from proceeding through a
// a point.
//
//	The Gate is initally closed (unsignalled).	It can be temporarily opened
// to let the current halted threads through by calling Release - much like
// a bouncer in a nightclub only letting the people waiting right at the 
// front through.
//
//	The Gate can be opened to all and not block any threads by calling open.
// To shut the gate again, call close.
//
/////////////////////////////////////////////////////////////////////////////
//
// History:
//		v1 - Created
//
/////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Gate.h"

namespace PDE
{

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------------
	// Name:	CGate Constructor
	// Desc:	Supplies stuff to CEventWrap
	//-----------------------------------------------------------------------------
	CGate::CGate(BOOL bManualReset, BOOL bInitialState, LPCTSTR lpName, LPSECURITY_ATTRIBUTES lpEventAttributes)
		:CEventWrap(bManualReset, bInitialState, lpName, lpEventAttributes)
	{

	}



	//-----------------------------------------------------------------------------
	// Name:	CGate Destructor
	// Desc:	Here just in case
	//-----------------------------------------------------------------------------
	CGate::~CGate()
	{

	}



	//-----------------------------------------------------------------------------
	// Name:	Open
	// Desc:	Opens the gate until further notice, rather then pulsing it 
	// Returns: TRUE if successful
	//-----------------------------------------------------------------------------
	BOOL CGate::open(void)
	{
		if (m_hEvent)
			return ::SetEvent(m_hEvent);
		else
			return FALSE;
	}

} // namespace PDE
