// CSimpleTimer.cpp: implementation of the CSimpleTimer class.
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


#include "stdafx.h"
#include "SimpleTimer.h"


namespace PDE
{
	//-----------------------------------------------------------------------------
	// Name:	Elapsed
	// Desc:	Gets elapsed time between starting and either now or stop time
	// Returns: If timer stopped - elapsed time between start and stopped time
	//			If timer not stopped - elapsed time between start and now
	//-----------------------------------------------------------------------------
	double CSimpleTimer::getElapsed()
	{
		if (m_clkStop==0)
		{
			//Just use current time
			return (double) ( clock() - m_clkStart ) / CLOCKS_PER_SEC; 
		}
		else
		{
			//Send back elapsed time between start and stopped
			return (double) ( m_clkStop - m_clkStart ) / CLOCKS_PER_SEC; 
		}
	}

} // end namespace PDE
