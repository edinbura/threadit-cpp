/**
 * Title: CActive
 * Description: Class cActive is the base class for Active Objects in a system.
 * The class provides a standardised approach to manage Active Objects in a software
 * system. Class cActive provides basic methods to instantiate, control and terminate
 * an active object. An active object is a C++ class whose instance contains
 * a single thread of execution.
 *
 * Copyright: Copyright (c) 2008 Ashkel Software
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2006/12/19 02:38:06 $
 */

// Includes
#include "stdafx.h"
#include "Active.h"
#include "strutil.h"
#include <string.h>
#include <process.h>
#include <assert.h>

using namespace std;

// Anonymous namespace
namespace
{
	/** Maximum number of characters in an error message returned by 
	  	strerror_s as defined in the MSDN Run-Time Library Reference. */
	static const size_t MAX_ERROR_MSG_SIZE = 94;
	/** performanceLogger is static and intended for logging static counters via a different Category name */
	static log4cpp::Category& performanceLogger = log4cpp::Category::getInstance ("perf.threadit.CActive");
}

// Initialise the count of the number of created CActive instances.
volatile UINT CActive::m_theThreadCount = 0;
volatile long CActive::m_theRunningThreadCount = 0;//sh 19/10/2010
CRITICAL_SECTION CActive::m_csHandleSet;
int InitcsHandleSet = CActive::InitCSHandleSet();
std::set<HANDLE> CActive::m_HandleSet;
/** 
 * Method initializeThread is called to set the initial state of the thread
 * as required by the various constructors.
 * thePriority is the default priority of the thread instance.
 * initialiseThread returns zero if the thread is created successfully otherwise
 * the _beginthreadex error number is return.
 */
errno_t CActive::initialiseThread (int thePriority)
{
	errno_t anError = 0;

	// The thread is not initially running.
	m_isThreadRunning = false;
	m_theThreadId = 0;
	m_isThreadStarted = false;
	// Set an empty thread name.
	m_theThreadName = "CActive";
	// Create the thread of execution for the instance and set it state to suspended.
	m_theThread = (HANDLE)_beginthreadex (NULL,0, threadStub, this, CREATE_SUSPENDED, &m_theThreadId);
	::EnterCriticalSection (&m_csHandleSet);
  m_HandleSet.insert(static_cast<HANDLE>(m_theThread));
	::LeaveCriticalSection (&m_csHandleSet);
	// We have added another thread.
	m_theThreadInstanceCount = ++m_theThreadCount;
	// Check that the thread has been created successfully.
	if (m_theThread != NULL)
	{
		// Set the default thread priority.
		SetThreadPriority (m_theThread, thePriority);
	} 
	else
	{
    _get_errno (&anError);
	} // if 
	return anError;
} // initialiseThread
 void CActive::KillAllThreads()
  {
	  ::EnterCriticalSection (&m_csHandleSet);
	  std::set<HANDLE>::iterator it = m_HandleSet.begin();
	  while (m_HandleSet.size())
	  {
		  ::TerminateThread(*it, (DWORD)-2);
		 // ::CloseHandle(*it);
		  m_HandleSet.erase(it++);
	  }
	  ::LeaveCriticalSection (&m_csHandleSet);
  }
 int CActive::InitCSHandleSet()
 {
	 ::InitializeCriticalSection(&m_csHandleSet);
	 return 0;
 }
/**
 * Method cActive is the constructor for the class. The method sets the
 * default thread priority for the active instance and starts the thread of
 * execution in a suspended state.
 * thePriority is the default priority of the thread instance.
 */
CActive::CActive (int thePriority)
{
	::OutputDebugStringA("\nWARNING: CActive will be without a thread name.");
	char theErrorMsg[MAX_ERROR_MSG_SIZE];
	string theMsg;
	errno_t anError;
	
	anError = initialiseThread (thePriority);
	if (anError == 0)
	{
		// put threadId in the message rather than the Category name
		// reduces the number of unique Categories created
		// m_theThreadName += string (".") + strutil::toString (m_theThreadId); 
		theMsg = "Thread Id " + strutil::toString(::GetCurrentThreadId());
		theMsg += "starting a new thread, instance=" + strutil::toString (m_theThreadInstanceCount);
		theMsg += ", Id=" + strutil::toString (m_theThreadId);
	}
	else
	{
		strerror_s (theErrorMsg, MAX_ERROR_MSG_SIZE, anError);
		theMsg = "error starting a new thread, instance=" + strutil::toString (m_theThreadInstanceCount);
		theMsg += ", Id=" + strutil::toString (m_theThreadId);
		theMsg += string (theErrorMsg);
	} // if 
	// Start a logger for this instance.
	m_ptheLogger = &(log4cpp::Category::getInstance (m_theThreadName));
	// Display a log message for the constructor outcome.
	if (anError != 0) 
	{
		m_ptheLogger->critStream() << "initialiseThread() instance count " << m_theThreadInstanceCount << " failed with error code " << anError;
		m_ptheLogger->error (theMsg);
	}
	else
	{
		if (m_ptheLogger->isDebugEnabled ()) {m_ptheLogger->debug (theMsg); } // if
	} // if 
} // constructor CActive

/** 
 * Method cActive is the constructor for the class. The method sets the 
 * default thread priority for the active instance and starts the thread of 
 * execution in a suspended state. 
 * thePriority is the default priority of the thread instance. 
 * theThreadName is the name allocated to the this thread instance. 
 */
CActive::CActive (const string& theThreadName, int thePriority)
{
	char theErrorMsg[MAX_ERROR_MSG_SIZE];
	string theMsg;
	errno_t anError;
	
	anError = initialiseThread (thePriority);
	if (anError == 0)
	{
		// put threadId in the message rather than the Category name
		// reduces the number of unique Categories created
		m_theThreadName = theThreadName + string (".CActive");
		theMsg = "Thread Id " + strutil::toString(::GetCurrentThreadId());
		theMsg += " is starting a new thread, instance=" + strutil::toString (m_theThreadInstanceCount);
		theMsg += ", Id=" + strutil::toString (m_theThreadId);
	}
	else
	{
		strerror_s (theErrorMsg, MAX_ERROR_MSG_SIZE, anError);
		m_theThreadName = theThreadName + string (".CActive");
		theMsg = "error starting a new thread, instance=" + strutil::toString (m_theThreadInstanceCount);
		theMsg += ", Id=" + strutil::toString (m_theThreadId);
		theMsg += string (theErrorMsg);
	} // if 
	// Start a logger for this instance.
	m_ptheLogger = &(log4cpp::Category::getInstance (m_theThreadName));
	// Display a log message for the constructor outcome.
	if (anError != 0)
	{
		m_ptheLogger->critStream() << "initialiseThread() instance count " << m_theThreadInstanceCount << " failed with error code " << anError;
		m_ptheLogger->error (theMsg);
	}
	else
	{
		if (m_ptheLogger->isDebugEnabled ()) {m_ptheLogger->debug (theMsg); } // if
	} // if 
} // constructor CActive

/**
 * Method ~cActive is the destructor for the class. In the case of the active
 * instance, the destructor must ensure that the thread has terminated at
 * this point. The debug version of this class checks that the thread handle
 * is invalid otherwise the method asserts. The thread handle must be invalid
 * when the destructor is called as this indicates that the thread has	terminated.
 * NB: We may have to implement a semaphore that allows the destructor to continue
 * only after the thread has exited.
 */
CActive::~CActive ()
{
	if (m_ptheLogger->isDebugEnabled ()) 
	{
		m_ptheLogger->debugStream() << "release thread instance=" << m_theThreadInstanceCount << ", Id=" << m_theThreadId;
	} // if
	// Check that the thread of execution has terminated otherwise stop the
	// program execution (in debug mode).
	assert (m_theThread == INVALID_HANDLE_VALUE);
	// We have deleted another thread.
	m_theThreadCount--;
} // ~CActive

/**
 * Method StartThread causes the instance to start execution. The thread of
 * execution for the instance is activated and starts execution.
 */
void CActive::startThread()
{
	DWORD theResult;

	// Stat the thread of execution.
	theResult = ResumeThread (m_theThread);
	// Check that the thread has actually started.
	assert (theResult != 0xFFFFFFFF);
	if (theResult != 0xFFFFFFFF)
	{
		m_isThreadStarted = true;
	} // if
} // startThread

/**
 * Method WaitForThreadToStop waits for the thread of execution of the active
 * instance to terminate. 
 */
void CActive::waitForThreadToStop(ULONG timeout)
{
	// Wait for the thread to terminate.
	if ((m_isThreadStarted) && (m_theThread != INVALID_HANDLE_VALUE))
	{
		if (WaitForSingleObject (m_theThread, timeout) != WAIT_OBJECT_0)
		{
			if (m_ptheLogger->isErrorEnabled ()) {m_ptheLogger->error ("thread failed to terminate in a timely fashion"); } // if
		}	 // if
		CloseHandle (m_theThread);
		if (m_ptheLogger->isInfoEnabled ()) {m_ptheLogger->info ("closing thread handle"); } // if
	}
	else
	{
		// Check if the thread has not yet started but the thread has been created.
		if (m_theThread != INVALID_HANDLE_VALUE)
		{
			TerminateThread (m_theThread, 0);
			CloseHandle (m_theThread);
		  if (m_ptheLogger->isErrorEnabled ()) {m_ptheLogger->error ("thread handle closed after terminating the thread"); } // if
		}
		else
		{
			// If m_isThreadRunning then the thread has not completed a suitable exit sequence.
			if (m_isThreadRunning)
			{
				if (m_ptheLogger->isErrorEnabled ()) {m_ptheLogger->error ("unable to terminate thread due to invalid handle"); } // if
			} // if 
		} // if 
	} // if
	m_theThread = INVALID_HANDLE_VALUE;
} // waitForThreadToStop

/**
 * Method isThreadRunning can be called to check that the thread is
 * currently executing.
 */
bool CActive::isThreadRunning ()
{ 
	bool isRunning = false;

	isRunning = (m_theThread != INVALID_HANDLE_VALUE);
	return isRunning;
} // isThreadRunning

/**
 * Method getThreadId returns the identity allocated to the thread
 * started for this method execution.
 */
UINT CActive::getThreadId ()
{
	return m_theThreadId;
} // getThreadId

/**
 * Method getThreadInstance returns the thread instance couunt. 
 * If this is the nth CActive thread started the threads instance
 * count will be n.
 */
UINT CActive::getThreadInstance () const
{
	return m_theThreadInstanceCount;
} // getThreadInstance

/**
 * Method getThreadName returns the textual identity allocated to the thread
 * started for this CActive instance. 
 */
string CActive::getThreadName () const
{
	return m_theThreadName;
} // getThreadName

/**
 * Method setThreadName sets the textual identity allocated to the thread
 * started for this CActive instance. Setting the thread name in the constructor
 * is the recommended approach. This method is provided in case using the constructor
 * is not easily achieved. This method should not be called if the thread has already 
 * been started in a call to startThread. The preferred approach is to name the 
 * thread in the constructor.
 * Method setThreadName returns true if the thread name has been set successfully.
 * THe method returns false if the thread has already started execution, unless called on CActive's thread.
 */
bool CActive::setThreadName (const string& theThreadName) 
{
	bool isSuccess = false;

	if (!m_isThreadStarted || GetCurrentThreadId() == m_theThreadId)
	{
		m_theThreadName = theThreadName + string(".CActive.") + strutil::toString (m_theThreadId); 
		// Replace the logger with a new one with the thread name.
		m_ptheLogger = &(log4cpp::Category::getInstance (m_theThreadName));
		isSuccess = true;
	} // if
	return isSuccess;
} // setThreadName

/**
 * Method getThreadCount returns the number of currently running threads. 
 */
UINT CActive::getThreadCount ()
{
	//sh 19/10/2010 the number of instances of CActive is not the number of currently running thread,
	// CActive could still be alive when the thread is not running any more
	//return m_theThreadCount; 
	return m_theRunningThreadCount;
} // getThreadCount

/**
 * Method waitForZeroThreads will wait for all the CActive threads to exit
 * before returning from the method. 
 * theTimeOut is the number of seconds to wait before returning in the 
 * case that not all the threads have completed execution. The smallest
 * value to wait is one second.
 * Method waitForZeroThreads returns true if all threads have exited.
 * The method returns false if there is a time out.
 */
bool CActive::waitForZeroThreads (ULONG theTimeOut)
{
	int theNumThreads;
	bool isSuccess = false;
	bool isWaiting = true;
	ULONG theSeconds = 0;

	do
	{
		theNumThreads = getThreadCount ();
		if (theNumThreads > 0)
		{ 
			Sleep (1000);
			theSeconds++;
		}
		else
		{
			isWaiting = false;
		} // if
		if (theSeconds >= theTimeOut)
		{
			isWaiting = false;
		} // if 
	} while (isWaiting);
	// Check that all threads have exited (or have never been started). 
	if (theNumThreads == 0)
	{
		isSuccess = true;
	} // if 
	return isSuccess;
} // waitForZeroThreads

/**
 * Method ThreadStub is the method that is used to identify the function
 * that represents the thread for the instance.
 * pObject is the pointer to the active object that provides the method
 * that executes as part of the thread.
 */
unsigned int __stdcall CActive::threadStub (void* pObject)
{
	int theNumThreads = 0;
	//string theThreadName ("CActive");//sh 13/10/2010 
	log4cpp::Category* ptheLogger = NULL;
	CActive* pThreadObject = NULL;

	// Get the name of the thread allocated to CActive instance.
	pThreadObject = static_cast<CActive*>(pObject);
	pThreadObject->m_isThreadRunning = true;
	pThreadObject->setThreadName( pThreadObject->m_theThreadName, -1);
	//theThreadName = pThreadObject->getThreadName (); //sh 13/10/2010 marked as memory leak by memory validator. 
	//don't know why. Anyway no one is using theThreadName, so remove it; 
	// Start a logger for this instance.
	//ptheLogger = &(log4cpp::Category::getInstance (theThreadName));
	ptheLogger = pThreadObject->getInstanceLogger ();
	// Indicate that the thread is started.
	InterlockedIncrementAcquire(&m_theRunningThreadCount);//sh 19/10/2010

	if (::performanceLogger.isDebugEnabled())
	{
	::EnterCriticalSection(&m_csHandleSet);
		::performanceLogger.debugStream() << "Count=" << m_theThreadCount << ", Running=" << m_theRunningThreadCount << ", Handles=" << m_HandleSet.size();
	::LeaveCriticalSection (&m_csHandleSet);
	}
	try
	{
		// The thread of execution is represented by the Thread method of the derived
		// class. Start execution of the thread routine.
		pThreadObject->threadRoutine ();
	} // try
  catch (std::exception& ex)
  {
      ptheLogger->error("threadstub has exited with an exception condition %s, with exception type %s", ex.what(), typeid(ex).name());
  }
	catch (...)
	{
		ptheLogger->error ("threadstub has exited with an exception condition");
	} // catch
	if (ptheLogger->isDebugEnabled ()) 
	{
		ptheLogger->debug ("threadstub exiting"); 
		theNumThreads = CActive::getThreadCount ();
		ptheLogger->debug ("There are still " + strutil::toString (theNumThreads) + " threads");
	} // if
	// On exit from the method that represents the thread of control, stop
	// thread execution.
	pThreadObject->m_isThreadRunning = false;
	InterlockedDecrementAcquire(&m_theRunningThreadCount);//sh 19/10/2010

	if (::performanceLogger.isDebugEnabled())
	{
	::EnterCriticalSection(&m_csHandleSet);
		::performanceLogger.debugStream() << "Count=" << m_theThreadCount << ", Running=" << m_theRunningThreadCount << ", Handles=" << m_HandleSet.size();
	::LeaveCriticalSection (&m_csHandleSet);
	}
	::EnterCriticalSection (&m_csHandleSet);
  if (!m_HandleSet.empty()) //why is this needed?
    m_HandleSet.erase(static_cast<HANDLE>(pThreadObject->m_theThread) );
	::LeaveCriticalSection (&m_csHandleSet);
	_endthreadex(0);
	
	// Return the method result.
	return 0;
} // threadStub


/**
 * Method getInstanceLogger returns the logger for this class instance and allows 
 * the instance to be used by the associated CActive thread.
 */
log4cpp::Category* CActive::getInstanceLogger ()
{
  return m_ptheLogger;
} // getInstanceLogger


/**
 * Method setThreadName is called to set the name of a thread.
 * @param[in] theThreadName is the name to set for the the thread.
 * @param[in] theThreadId is the id of the thread whose name is to be set. Set to -1 if
 * calling from within the thread to be named.
 * This function is documented as being callable from outside of the thread which is being
 *  named, however it appears that it works more reliably if called from within the code of
 * the thread being name, passing a threadId of -1 to indicate "current thread"
 */
void CActive::setThreadName (const std::string& theThreadName, const DWORD theThreadId)
{
  #define MS_VC_EXCEPTION 0x406D1388
  #define BUFFER_LEN 160

  typedef struct tagTHREADNAME_INFO
  {
	  DWORD dwType;  // must be 0x1000
	  LPCSTR szName; // pointer to name (in user address space) buffer must include terminator character
	  DWORD dwThreadID; // thread ID (-1 == caller thread)
	  DWORD dwFlags; // reserved for future use, must be zero
  } THREADNAME_INFO;

  THREADNAME_INFO ThreadInfo;

  char szSafeThreadName[BUFFER_LEN]; // buffer can be any size, just make sure it is large enough!
  // Seems only the first 31 are shown in DevStudio though.
  memset (szSafeThreadName, 0, sizeof(szSafeThreadName)); // ensure all characters are NULL before
  strncpy_s (szSafeThreadName, sizeof(szSafeThreadName), theThreadName.c_str (), theThreadName.length() ); // copying name
  //szSafeThreadName[BUFFER_LEN - 1] = '\0';
  ThreadInfo.dwType = 0x1000;
  ThreadInfo.szName = szSafeThreadName;
  ThreadInfo.dwThreadID = theThreadId;
  ThreadInfo.dwFlags = 0;
  __try
  {
	RaiseException(MS_VC_EXCEPTION, 0, sizeof(ThreadInfo) / sizeof(DWORD), (DWORD*)&ThreadInfo);
  }
  __except(EXCEPTION_EXECUTE_HANDLER)
  {
	// do nothing, just catch the exception so that you don't terminate the application
  } //
} // setThreadName
