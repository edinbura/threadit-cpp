// CSingletonExample.cpp: implementation of the CSingletonExample class.
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
// Started:		01Dec02
//
// Purpose:		Example use of the TSingleton class template
//		
/////////////////////////////////////////////////////////////////////////////
//
// Notes:
//
//	This example shows how you may derive a class from the TSingleton template.
//
//	 Inherit the template this: class CSingletonExample	 : public TSingleton< CSingletonExample >
//	 Then in the class definiton define the template base as a friend: friend class TSingleton< CSingletonExample >;
// This is so we can get at the protected constructor.
//
//	It also shows you one possible method of locking using the simplest lock
// in this project, CLockableObject.
//
/////////////////////////////////////////////////////////////////////////////
//
// History:
//		v1 - Created
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SingletonExample.h"
#include "KeyChain.h"


namespace PDE
{
	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////
	CSingletonExample::CSingletonExample()
		:TSingleton< CSingletonExample > ()
	{
		
	}

	CSingletonExample::~CSingletonExample()
	{

	}



	//-----------------------------------------------------------------------------
	// Name:	SetTestInt()
	// Desc:	Example of using the locking mechanism built into the singleton
	//	to do thread-safe attribute changes
	//-----------------------------------------------------------------------------
	void CSingletonExample::setTestInt(const int i)
	{		
		//Lock this section as we are changing an attribute		
		TKeyChain< CLockableObject > lock( this );

		//Set the attribute
		m_iTest = i;		
	}



	//-----------------------------------------------------------------------------
	// Name:	SetTestInt()
	// Desc:	Example of using the locking mechanism built into the singleton
	//	to do thread-safe attribute changes
	//-----------------------------------------------------------------------------
	int CSingletonExample::getTestInt()
	{ 
		//Lock this section as we are getting the attribute		
		TKeyChain< CLockableObject > lock( this );		

		return m_iTest; 
	}



	//-----------------------------------------------------------------------------
	// Name:	DebugShow()
	// Desc:	Writes out info that may be useful for info to the stack.
	//-----------------------------------------------------------------------------
	void CSingletonExample::debugShow()
	{
		//Lock this section as we are displaying the attribute		
		TKeyChain< CLockableObject > lock( this );		

		m_ptheLogger->debug(" CSingletonExample::DebugShow - this=0x%08X TestAttribute=%d ThreadID=%d\n",this, m_iTest, ::GetCurrentThreadId());
	}

} // end namespace PDE
