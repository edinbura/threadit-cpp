// TSingleton.h: interface for the TSingleton class.
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
// Purpose:		Provide template for thread-safe Singletons
//		
/////////////////////////////////////////////////////////////////////////////
//
// Notes:
//
//	 This version of a template can be inherited from by a class and that
// class should inherit all its OWN copy of the static variables.
//
//	 The type of the template should be that of the derived class (!)
// so that Instance returns the correct type.
//
//	 Inherit it like this: class CSingletonExample	: public TSingleton< CSingletonExample >
//	 Then in the class definiton define the template base as a friend: friend class TSingleton< CSingletonExample >;
// This is so we can get at the protected constructor.
//
//	 Think of this as a OO & Thread friendly global variable wrapper
//
//	 Virtual Functions are declared & implemented in the header file as I 
// was getting linker errors when implementing them in the cpp file (!)
//
//////////////////
//
// Changes:
// 
//	 This version USED TO inherit from CLockableObject - so the Singleton natively 
// knows how to lock and unlock itself for data access and mutation.
//	 
//	 The locking was utilised by using Lock & Unlock, or passing this
// through to a TKeyChain, like this:
//		TKeyChain < CLockableObject > lock (this);
//
//	 This feature was taken out incase you wanted to inherit from something
// else lockable - like the more complicated (but useful?) CFairRWLock.h
//
/////////////////////////////////////////////////////////////////////////////
//
// History:
//		v1 - Created
//
/////////////////////////////////////////////////////////////////////////////

#if !defined (TSINGLETON)
#define TSINGLETON

// Includes
#include "SingletonBase.h"
#include "KeyChain.h"
#include <log4cpp/Category.hh>

namespace PDE
{
	template<class T = CSingletonBase> class TSingleton : public CSingletonBase//, public CLockableObject
	{
		// Attributes
	protected:
	//ms_pOnlyInstance holds the only Instance of this Singleton
	//static TSingleton<T>* ms_pOnlyInstance;
	static T* ms_pOnlyInstance;

	//Only needs to be static because we are locking something classwide rather then instance wide
	static CLockableObject ms_key;

	//Flags if we are allowed to create an instance if necessary.	 FALSE when Singleton destroyed, but can be reset via mutators to allow another instance.
	//	This flag means there can only ever be 0..1 Instance in memory at one time, and prevents recreation after destruction by mistake.
	//The flag is made volatile to prevent the result being erronously cached in the registers by the compiler while another thread may have changed its value.
	static volatile BOOL ms_bCanCreateInstance;

	//volatile - Member data is loaded from memory each time it is accessed; disables certain optimizations.
	// - this ensures that the compiler does not optimise out our cheap lock hint, before going on to do the more expensive CRITICAL_SECTION mutex lock
	static volatile BOOL ms_bSingletonCreatedFlag;
	/** m_ptheLogger is the logger used to log information and errors for each instance of this class */
	log4cpp::Category* m_ptheLogger;
	
	// Constructors / destructors
	protected:
	//Constructor has to be private as part of the singleton way
	TSingleton<T>():CSingletonBase()
	{ 
		// Obtain the logger for reporting the status of this singletone.
		m_ptheLogger = &(log4cpp::Category::getInstance ("TSingleton"));
		m_ptheLogger->debug ("TSingleton<T> Constructor called - this=0x%08X ThreadID=%d\n", this, ::GetCurrentThreadId());
	}; // constructor TSingleton

	public:
	//Destructor must be virtual and public so everything can be cleaned up properly.
	virtual ~TSingleton ()
	{			
		m_ptheLogger->debug ("TSingleton<T> Destructor called - this=0x%08X ThreadID=%d\n", this, ::GetCurrentThreadId());

		//Lock this section with this singleton's mutex, so we don't accidently delete ourselves twice or sommit
		//TKeyChain< CLockableObject > lock( ms_key );
		TKeyChain <CLockableObject> lock (&ms_key);
			

		//PDE 26Nov02 - Set m_bCanCreateInstance flag to false - if the singleton is being destroyed assume the program is going the same way!
		//PDE 28Nov02 - Also set m_bSingletonCreatedFlag to FALSE, as after this DESTRUCTOR has finshed, there will not be this Singleton in memory
		ms_bSingletonCreatedFlag = ms_bCanCreateInstance=FALSE;		

		//We are no longer pointing at anything...
		ms_pOnlyInstance=NULL;			
	}; // destructor ~TSingleton

	// Services
	public:	 

	//Returns Critical Section key from Singleton
	const CLockableObject* getKey()
	{
		return &ms_key;
	}; // GetKey

	//Show debug stuff
	virtual void debugShow () 
	{
		m_ptheLogger->debug (" template< class T >TSingleton<T>::DebugShow - this=0x%08X TestAttribute=%d ThreadID=%d\n", this, ::GetCurrentThreadId());
	};

	void log (std::string theMsg) 
	{
		if (theMsg != NULL)
		{
			m_ptheLogger->info (theMsg);
		}
		else
		{
			m_ptheLogger->info ("Message to log is NULL");
		} // if 
	};

	//Implement Generic instance grabbing techique from base
	virtual void* getSingletonInstance(void** pOut)
	{
		return *pOut = getInstance();
	};			

	//-----------------------------------------------------------------------------
	// Name:	Instance()
	// Desc:	Returns only instance of contained object if available, otherwise returns NULL.
	// Returns: Can return NULL.	Do NOT clean this up, unless not using CSingletonObserver
	//-----------------------------------------------------------------------------		
	static T* getInstance()
	{		
		if( ms_bCanCreateInstance && !ms_bSingletonCreatedFlag )
		{
			//The Booleans say we should check to see if an instance needs to be created.
			//Now we use the more reliable, but time consuming CRITICAL_SECTION lock

			//Lock this section with this singleton's critical section object				
			TKeyChain<CLockableObject> lock (&ms_key);


			//"Double-Checked Locking" - we are now in the Mutex section, which means only one thread at a time should be allowed through here.
			// This means that testing the boolean now will 100% be correct, as this thread of execution can't be preempted out before another completes
			if (ms_bCanCreateInstance && !ms_bSingletonCreatedFlag) 
			{
				//Create the only Instance of the Singleton
				ms_pOnlyInstance = new T;

				//This alternative would crash the system (although will compile).	Tried this to get away from needing to make this template a friend
				//ms_pOnlyInstance = (T*)new TSingleton<T>;

				//Set the instance creation flag
				ms_bSingletonCreatedFlag = (ms_pOnlyInstance == NULL ? FALSE : TRUE);

				//Add this singleton to be Observed, so it's lifetime can be properly controlled
				addToObserver (ms_pOnlyInstance);				
			}			
		}

		//Could be NULL if we aren't allowed to create a Singleton
		return ms_pOnlyInstance;
	}; // Instance

	//If the singleton has been destroyed, it will prevent further versions of itself created unless told otherwise.
	//This is to prevent methods erronously calling Instance after destruction causing a Singleton to be created that may never be cleaned up.
	static void enableInstanceCreation (BOOL bEnabled=TRUE) {ms_bCanCreateInstance = bEnabled;}

	//Returns TRUE if further instance creation is permitted
	static BOOL canCreateInstance () {return ms_bCanCreateInstance;}

	}; // template< class T = CSingletonBase> class TSingleton

	//////////////////////////////////////////////////////////////////////
	// Static variable init
	//////////////////////////////////////////////////////////////////////
	template <class T> T* TSingleton<T>::ms_pOnlyInstance=NULL;
	template <class T> volatile BOOL TSingleton<T>::ms_bCanCreateInstance=TRUE;
	template <class T> volatile BOOL TSingleton<T>::ms_bSingletonCreatedFlag=FALSE; 
	template <class T> CLockableObject TSingleton<T>::ms_key;

} // end namespace PDE

#endif // !defined (TSINGLETON)
