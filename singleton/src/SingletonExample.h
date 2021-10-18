// CSingletonExample.h: interface for the CSingletonExample class.
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



#if !defined(AFX_CSINGLETONEXAMPLE_H__785A1E89_070B_4953_9FC5_ECDA5E6222D7__INCLUDED_)
#define AFX_CSINGLETONEXAMPLE_H__785A1E89_070B_4953_9FC5_ECDA5E6222D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TSingleton.h"
#include "LockableObject.h"

namespace PDE
{

	class CSingletonExample	 : public TSingleton< CSingletonExample >, public CLockableObject
	{
		//You must make the template base a friend so it can use the protected constructor
		friend class TSingleton< CSingletonExample >;

	protected:
		// Constructor must be private or protected so no other object can initiate its creation.
		CSingletonExample();	//Must have appropriate default constructor!

	public:		
		virtual ~CSingletonExample();

		//////////////////////////////////////////////////
		// Test attributes
		void setTestInt( const int i );
		int getTestInt(); 
		//////////////////////////////////////////////////

		void debugShow();

	private:
		int m_iTest;
	};

} // end namespace PDE

#endif // !defined(AFX_CSINGLETONEXAMPLE_H__785A1E89_070B_4953_9FC5_ECDA5E6222D7__INCLUDED_)
