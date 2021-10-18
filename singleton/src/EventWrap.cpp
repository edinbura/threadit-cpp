// CEventWrap.cpp: implementation of the CEventWrap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EventWrap.h"

namespace PDE
{

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////


	//-----------------------------------------------------------------------------
	// Name:	CEventWrap constructor
	// Desc:	Populates internal handle with a Created Event
	// Params:	DEF: bManualReset = TRUE;		- TRUE mean you have to manually Signal and UnSignal the event
	//			DEF: bInitialState = FALSE;		- FALSE means unsignaled, TRUE means signaled
	//			DEF: lpName = NULL;				- NULL means unnamed.	 Can be named though.
	//			DEF: lpEventAttributes = NULL - Windows security attributes.	NULL means use Windows default.
	//-----------------------------------------------------------------------------
	CEventWrap::CEventWrap(BOOL bManualReset, BOOL bInitialState, LPCTSTR lpName, LPSECURITY_ATTRIBUTES lpEventAttributes)
	{ 
		m_hEvent = CreateEvent(
			lpEventAttributes,	// SD
			bManualReset,			// reset type
			bInitialState,				// initial state
			lpName								// object name
													 );
	}



	//-----------------------------------------------------------------------------
	// Name:	CEventWrap destructor
	// Desc:	Closes event handle - which should trigger the destruction of
	//	the object, and cause things to be unblocked if stuff still is 
	//	(with any unluck!)
	//-----------------------------------------------------------------------------
	CEventWrap::~CEventWrap()
	{
		//Closing the handle on the event should unblock any threads waiting?
		CloseHandle(m_hEvent);
		m_hEvent=NULL;
	}



	//-----------------------------------------------------------------------------
	// Name:	SignalEvent
	// Desc:	Event signaled, threads blocked waiting for the signal are unblocked
	//	From MSDN:
	//		The PulseEvent function sets the specified event object to the 
	//	signaled state and then resets it to the nonsignaled state after releasing 
	//	the appropriate number of waiting threads. 
	// Returns: PulseEvent val
	//-----------------------------------------------------------------------------
	BOOL CEventWrap::signalEvent(void) 
	{
		if (m_hEvent)
			return PulseEvent(m_hEvent);
		else
			return FALSE;
	}



	//-----------------------------------------------------------------------------
	// Name:	UnSignalEvent
	// Desc:	Event unsignaled, threads are now blocked again when waiting for 
	//	the signalled status
	// Returns: ::ResetEvent val
	//-----------------------------------------------------------------------------
	BOOL CEventWrap::unSignalEvent(void)
	{
		if (m_hEvent)
			return ::ResetEvent(m_hEvent);
		else
			return FALSE;
	}



	//-----------------------------------------------------------------------------
	// Name:	WaitEventSignal
	// Desc:	Waits until the Event is signaled 
	// Params:	dwMilliseconds - Default INIFINITE.	 Can be time to wait in ms.
	//				- Use 0 if you want it to return straight away if it' blocked
	//					or not.
	// Returns: Result of WaitForSingleObject called on the internal Semaphore handle
	//-----------------------------------------------------------------------------
	DWORD CEventWrap::waitEventSignal(DWORD dwMilliseconds) 
	{
		DWORD dwWaitForSingleObjectRet = WAIT_FAILED;

		if (m_hEvent)
			WaitForSingleObject(m_hEvent, dwMilliseconds);

		return dwWaitForSingleObjectRet;
	}

} // namespace PDE
