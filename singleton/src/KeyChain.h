// TKeyChain.h: interface for the CLockGuard class.
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
// Purpose:		Makes using CLockableObject even easier / safer
//		
/////////////////////////////////////////////////////////////////////////////
//
// Notes:
//
// This template class further abstracts away from the likes of 
//	CLockableObject - which you may wish to replace with your own mechanism
//
// Encapsulating the CLockableObject like this means that even if an exception
//is thrown, if declared as an automatic object on destruction it should unlock
//the section anyway!
//
// Key must implement:
//		.Lock()
//		.Unlock()
//
/////////////////////////////////////////////////////////////////////////////
//
// History:
//		v1				- Created
//		v1.1 01Dec02	- Changed to use pointers rather then refs.
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_TKeyChain_H__395FB08F_8DB9_406E_A2DC_888152D0398E__INCLUDED_)
#define AFX_TKeyChain_H__395FB08F_8DB9_406E_A2DC_888152D0398E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



namespace PDE
{

	template< class Key > class TKeyChain
	{
	public:
		
		//Create a TKeyChain, and use the key to lock immediately
		//inline TKeyChain( Key& key ) : m_key( key ) { lock(); }
		inline TKeyChain( Key* pKey ) : m_pKey( pKey ) { lock(); } 

		//On Destruction, unlock the section guard and release the key.
		//Take out inline and replace with virtual if inheriting from this class
		inline ~TKeyChain() { unlock(); }
		

		//Lock the section
		//void Lock() { m_key.lock(); }
		void lock() { if (m_pKey) m_pKey->lock(); }


		//Unlock the section
		//void Unlock() { m_key.Unlock(); }
		void unlock() { if (m_pKey)m_pKey->unlock(); }		

		
	private:
		//Key& m_key;
		Key* m_pKey;

	};

} // end namespace PDE

#endif // !defined(AFX_TKeyChain_H__395FB08F_8DB9_406E_A2DC_888152D0398E__INCLUDED_)
