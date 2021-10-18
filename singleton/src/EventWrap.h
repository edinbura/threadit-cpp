// CEventWrap.h: interface for the CEventWrap class.
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
// Purpose:		Implement a wrapped Event type syncronisation object
//		
/////////////////////////////////////////////////////////////////////////////
//
// Notes:
//
//	Wraps up lifetime of a Win32 Event object.
//
//		An Event blocks all events at a WaitForMultipleObjects or 
//	WaitForSingleObject, until that Event is "Signaled" via a "Pulse" / "SetEvent"
//
//	 By default the Event encapsulated by this wrapper is unnamed, 
// manually resets, with no security attributes.	It starts off 
// Non-Signaled (Blocks).
//
//	 Auto reset event objects automatically change from signaled to nonsignaled 
// when a single waiting thread is released. 
//	
//
/////////////////////////////////////////////////////////////////////////////
//
// History:
//		v1 - Created
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_CEVENTWRAP_H__DE31DA86_E9C5_41B0_A096_AE92D9F57B49__INCLUDED_)
#define AFX_CEVENTWRAP_H__DE31DA86_E9C5_41B0_A096_AE92D9F57B49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


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
	class CEventWrap	
	{
	protected:
		HANDLE	m_hEvent;

	public:
		CEventWrap(BOOL bManualReset=TRUE, BOOL bInitialState=FALSE, LPCTSTR lpName=NULL, LPSECURITY_ATTRIBUTES lpEventAttributes=NULL);

		virtual ~CEventWrap();

		BOOL signalEvent(void);

		BOOL unSignalEvent(void);

		DWORD waitEventSignal(DWORD dwMilliseconds=INFINITE);
	}; // class CEventWrap

} // namespace PDE


#endif // !defined(AFX_CEVENTWRAP_H__DE31DA86_E9C5_41B0_A096_AE92D9F57B49__INCLUDED_)
