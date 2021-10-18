#include "stdafx.h"
#include "singleton.h"
#include "stdio.h"
#include <iostream>

using namespace std;



// 
// this is a yabo(r) trick to ensure
// the singleton destruction on
// program termination
// 
class CSingletonCleaner
{
public:
	~CSingletonCleaner()
	{
		CSingleton::Destroy();
	}
} CSingletonCleanerInst;
 
// 
// static members
// 
CSingleton* CSingleton::_instance = 0;
int CSingleton::_refCount = 0;
HANDLE CSingleton::_mutex = NULL;
log4cpp::Category* m_ptheLogger = &(log4cpp::Category::getInstance ("CSingleton"));
 
CSingleton::CSingleton()
{
	// Create the mutex
	_mutex = CreateMutex(NULL, FALSE, NULL);
	if ( _mutex == NULL ) 
	{
		m_ptheLogger->debug("CreateMutex error: %d\n", GetLastError());
	}
 

	// do some initialization
}
 
CSingleton::~CSingleton()
{
	// release resources
}
 
CSingleton* CSingleton::Instance()
{
	DWORD dwResult;
 
	// Try to lock the mutex
	dwResult = WaitForSingleObject(_mutex, INFINITE);
 
	// Check if the mutex is locked
	if( dwResult != WAIT_OBJECT_0 )
	{
		m_ptheLogger->error("WaitForSingleObject error: %d\n", GetLastError());
		return NULL;
	}
 
	// Create the new instance, protected by a
	// try-catch block to let us release the mutex
	// during an exception
	try
	{
		if( _instance == 0 ) // if first time ...
		{
			_instance = new CSingleton(); // ... create a new 'solo' instance
		}
		_refCount++; //increase reference counter
	}
	catch( ... )
	{
		// On exception, release the mutex and throw the exception 
		if( ! ReleaseMutex( _mutex ) )
		{
			// Unable to release mutex, deal with error
		}
		throw;
	}
 
	// Release the mutex
	if( ! ReleaseMutex( _mutex ) )
	{
		// Unable to release mutex, deal with error
	}
	return _instance;
}
 
void CSingleton::Release()
{
	if( --_refCount < 1 )
	{
		Destroy();
	}
}
 
void CSingleton::Destroy()
{
	DWORD dwResult;
 
	// Try to lock the mutex
	dwResult = WaitForSingleObject(_mutex, INFINITE);
 
	// Check if the mutex is locked
	if( dwResult != WAIT_OBJECT_0 )
	{
		m_ptheLogger->error("WaitForSingleObject error: %d\n", GetLastError());
		return;
	}
 
	// Destroy the singleton instance, protected by a
	// try-catch block to let us release the mutex
	// during an exception
	try
	{
		if( _instance != 0 )
		{
			delete( _instance );
			_instance = 0;
		}
	}
	catch( ... )
	{
		// On exception, release the mutex and throw the exception 
		if( ! ReleaseMutex( _mutex ) )
		{
			// Unable to release mutex, deal with error
		}
		throw;
	}
 
	// Release the mutex
	if( ! ReleaseMutex( _mutex ) )
	{
		// Unable to release mutex, deal with error
	}
}

void CSingleton::method()
{
	m_ptheLogger->info ("Method of the singleton class\n");
}
