// CGate.h: interface for the CGate class.
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


#if !defined(AFX_CGATE_H__EDCF19F4_EC02_43A1_BDB7_CC6BE27BCE11__INCLUDED_)
#define AFX_CGATE_H__EDCF19F4_EC02_43A1_BDB7_CC6BE27BCE11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EventWrap.h"

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
	class CGate : protected CEventWrap	
	{
	public:
		CGate(BOOL bManualReset=TRUE, BOOL bInitialState=FALSE, LPCTSTR lpName=NULL, LPSECURITY_ATTRIBUTES lpEventAttributes=NULL);
		virtual ~CGate();

		
		BOOL open(void);


		//-----------------------------------------------------------------------------
		// Name:	Close
		// Desc:	Closes gate, no threads can pass! (Uses UnSignal Event)
		// Returns: TRUE if successful
		//-----------------------------------------------------------------------------
		inline BOOL close(void) 
		{return CEventWrap::unSignalEvent();}



		//-----------------------------------------------------------------------------
		// Name:	Release
		// Desc:	Open gate temporarily, letting waiting threads proceed
		// Returns: TRUE if successful
		//----------------------------------------------------------------------------- 
		inline BOOL release(void) 
		{return CEventWrap::signalEvent();}



		//-----------------------------------------------------------------------------
		// Name:	WaitGateOpen
		// Desc:	Blocks threads until gate is open.
		// Returns: WaitForSingleObject return value
		//-----------------------------------------------------------------------------			
		inline DWORD waitGateOpen(DWORD dwMilliseconds=INFINITE) 
		{return CEventWrap::waitEventSignal(dwMilliseconds);}

	};

} // namespace PDE

#endif // !defined(AFX_CGATE_H__EDCF19F4_EC02_43A1_BDB7_CC6BE27BCE11__INCLUDED_)
