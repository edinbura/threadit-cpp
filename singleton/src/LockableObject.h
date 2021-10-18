// CLockableObject.h: interface for the CLockableObject class.
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
// Purpose:		Wrapper for Win32 API CRITICAL_SECTION object
//		
/////////////////////////////////////////////////////////////////////////////
//
// Notes:
//
//	Wraps up lifetime of CRITICAL_SECTION object.
//
/////////////////////////////////////////////////////////////////////////////
//
// History:
//		v1 - Created
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(LOCKABLE_OBJECT)
#define LOCKABLE_OBJECT

#include <windows.h>

namespace PDE
{

	class CLockableObject	 
	{
	public:
		inline CLockableObject() { InitializeCriticalSection(&m_CriticalSection); }

		//Make below virtual if planning to inherit from this class!
		virtual inline ~CLockableObject() { DeleteCriticalSection(&m_CriticalSection); }
		
		//Locks Section (Acquires mutex control)
		inline void lock() { EnterCriticalSection(&m_CriticalSection); }

		//Releases Section (Releases mutex control)
		inline void unlock() { LeaveCriticalSection(&m_CriticalSection); }

	private:
		//Does not need to be static
		CRITICAL_SECTION m_CriticalSection;
	}; // CLockableObject
} // namespace PDE

#endif // !defined(LOCKABLE_OBJECT)
