// CSimpleTimer.h: interface for the CSimpleTimer class.
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
// Purpose:		Simple stopwatch style timer
//		
/////////////////////////////////////////////////////////////////////////////
//
// Notes:
//
//	 Call Start to start the timer, Stop to stop and Elapsed to get the 
// amount of seconds elapsed between those two events.
//
//	 You may call Start, and then Elapsed without calling stop to get current
// elapsed time.
//
/////////////////////////////////////////////////////////////////////////////
//
// History:
//		v1 - Created
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSIMPLETIMER_H__D66F2586_595B_4E9C_8B3E_BBA4E300781D__INCLUDED_)
#define AFX_CSIMPLETIMER_H__D66F2586_595B_4E9C_8B3E_BBA4E300781D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <time.h>

namespace PDE
{
	class CSimpleTimer
	{
	public:
		inline CSimpleTimer() : m_clkStart(0), m_clkStop(0) { }
		virtual ~CSimpleTimer(){}

		inline void start () { m_clkStart = clock(); }
		inline void stop () { m_clkStop = clock(); }
		inline void reset () { m_clkStart = m_clkStop = 0; }

		double getElapsed ();			

	protected:
		clock_t m_clkStart;
		clock_t m_clkStop;
	}; // class CSimpleTimer

} // end namespace PDE

#endif // !defined(AFX_CSIMPLETIMER_H__D66F2586_595B_4E9C_8B3E_BBA4E300781D__INCLUDED_)
