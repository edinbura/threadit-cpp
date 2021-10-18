// CLockGuard.h: interface for the CLockGuard class.
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
// E-Mail comments / suggestions to: paulsdsrubbish@hotmail.com
//		It would be nice to hear of any positive uses of this code!
//
// Started:		26Nov02
//
// Purpose:		Simple Interface style template for CCriticalSection 
//		
/////////////////////////////////////////////////////////////////////////////
//
// Notes:
//
// This template class further abstracts away from the likes of 
//	CCriticalSection - which you may wish to replace with your own mechanism
//
// Key must implement:
//		.Aquire()
//		.Release()
//
/////////////////////////////////////////////////////////////////////////////
//
// History:
//		v1 - Created
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_CLOCKGUARD_H__395FB08F_8DB9_406E_A2DC_888152D0398E__INCLUDED_)
#define AFX_CLOCKGUARD_H__395FB08F_8DB9_406E_A2DC_888152D0398E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
// This warning is really stupid. Of course CLockGuard is not assignable!
#pragma warning(disable: 4512)
#endif // _MSC_VER > 1000

namespace PDE
{

	template< class Key > class CLockGuard	
	{
	public:
		
		//Create a LockGuard, and lock immediately
		inline CLockGuard( Key& key ) : m_key( key ) { lock(); }

		//On Destruction, unlock the guard and release the key.
		//Take out inline and replace with virtual if inheriting from this class
		inline ~CLockGuard() { unlock(); }
		
		void lock() { m_key.lock(); }
		void unlock() { m_key.unlock(); }
		
	private:
		Key& m_key;

	};

} // end namespace PDE

#endif // !defined(AFX_CLOCKGUARD_H__395FB08F_8DB9_406E_A2DC_888152D0398E__INCLUDED_)
