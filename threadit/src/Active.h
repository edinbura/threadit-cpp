/*-------------------------------------------------------------------------*/
/* Copyright (C) 2021 by Ashkel Software                                   */
/* ari@ashkel.com.au                                                       */
/*                                                                         */
/* This file is part of the threadit library.                              */
/*                                                                         */
/* The threadit library is free software; you can redistribute it and/or   */
/* modify it under the terms of The Code Project Open License (CPOL) 1.02  */
/*                                                                         */
/* The threadit library is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the CPOL       */
/* License for more details.                                               */
/*                                                                         */
/* You should have received a copy of the CPOL License along with this     */
/* software.                                                               */
/*-------------------------------------------------------------------------*/

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
#if !defined (ACTIVE_H)
#define ACTIVE_H

// Includes
#include <windows.h>
#include <string.h>
#include <log4cpp/Category.hh>
/**
 * Class CActive is used as the base class that manages the lifecycle
 * of a thread within the application. Support is provided to 
 * execute a method on the class within a thread of execution. Methods
 * are provided to start, stop and wait for completion of a thread.
 */
class CActive 
{
	// constants
  public:
	  /** Maximum time to wait for a thread to terminate execution once the
			terminate flag has been set. */
	  static const ULONG THREAD_STOP_TIME = 10000;
	// Attributes
  private:
	  /** m_Thread is the handle to the thread of execution for the instance. */
	  volatile HANDLE	 m_theThread;
	  /** m_isThreadStart indicates if the thread is executing or not. This value
			is used to exit the thread of exection during shutdown. */
	  volatile bool m_isThreadStarted;
	  /** m_isThreadRunning is provided to assist in tracking when the thread is running
	    or when it has exited the main thread loop. */
	  volatile bool m_isThreadRunning;
	  /** m_theThreadId is the unique identity of the the thread within the process.*/
	  UINT m_theThreadId;
	  /** m_theThreadInstanceCount is the instance count assigned to this thread. 
	      If this is the nth CActive thread started the threads instance count will be n */
	  UINT m_theThreadInstanceCount;
	  /** m_theThreadName is the unique textual identity of the the thread within the process.*/
	  std::string m_theThreadName;
	  /** m_ptheLogger is the logger used to log information and errors for each instance of this class */	
	  log4cpp::Category* m_ptheLogger;
	  /** m_theThreadCount is the current total number of created CActive instances. */
	  static volatile UINT m_theThreadCount;
    /** m_theRunningThreadCount is the number of created CActive instances currently running. */
	  static volatile long m_theRunningThreadCount;
	  
	  // CRITICAL_SECTION to protect m_HandleSet
	  static CRITICAL_SECTION m_csHandleSet;
	  // a static intger, sole purpose is to initlize m_csHandleSet;
	  static int InitcsHandleSet;
	  // set for all the active thread handle, only used by KillAllThreads
	  static std::set<HANDLE> m_HandleSet;

	// Methods
  public:

	  /** 
	   * Method cActive is the constructor for the class. The method sets the 
	   * default thread priority for the active instance and starts the thread of 
	   * execution in a suspended state. 
	   * thePriority is the default priority of the thread instance. 
	   */
	 
	  
	  //used as a compromised way to quickly terminate all the threads created in CActive,
	  //After execution, don't try to access or stop any 
	  //threadit, otherwise would cause corruption.
	  //Any resouces associated with it will NOT be released properly, should only be used at the end
	  //of the program when it needs to terminate a process quickly.
	  static void KillAllThreads();
	  //this function always return 0, used to initlize m_csHandleSet
	  static int InitCSHandleSet();
	  CActive (int thePriority = THREAD_PRIORITY_NORMAL);

	  /** 
	   * Method cActive is the constructor for the class. The method sets the 
	   * default thread priority for the active instance and starts the thread of 
	   * execution in a suspended state. 
	   * thePriority is the default priority of the thread instance. 
	   * theThreadName is the name allocated to the this thread instance. 
	   */
	  CActive (const std::string& theThreadName, int thePriority = THREAD_PRIORITY_NORMAL);

  		 
	  /**
	   * Method ~cActive is the destructor for the class. In the case of the active
	   * instance, the destructor must ensure that the thread has terminated at 
	   * this point. The debug version of this class checks that the thread handle
	   * is invalid otherwise the method asserts. The thread handle must be invalid
	   * when the destructor is called as this indicates that the thread has terminated.
	   */
	  virtual ~CActive ();

	  /**
	   * Method StartThread causes the instance to start execution. The thread of 
	   * execution for the instance is activated and starts execution. 
	   */
	  virtual void startThread ();
  	
	  /**
	   * Method WaitForThreadToStop waits for the thread of execution of
	   * the active instance to terminate. If the thread fails to
	   * terminate within the timeout period, it is left running, and the
	   * fact logged.
	   */
	  virtual void waitForThreadToStop (ULONG timeout = THREAD_STOP_TIME);

	  /**
	   * Method isThreadRunning can be called to check that the thread is
	   * currently executing.
	   */
	  bool isThreadRunning ();

	  /**
	   * Method getThreadId returns the identity allocated to the thread
	   * started for this method execution.
	   */
	  UINT getThreadId ();

	  /**
	   * Method getThreadName returns the textual identity allocated to the thread
	   * started for this CActive instance. 
	   */
	  std::string getThreadName () const;

	  /**
	   * Method setThreadName sets the textual identity allocated to the
	   * thread started for this CActive instance. Setting the thread
	   * name in the constructor is the recommended approach. This
	   * method is provided in case using the constructor is not easily
	   * achieved. This method should not be called if the thread has
	   * already been started in a call to startThread, unless called on
	   * the thread itself. The preferred approach is to name the thread
	   * in the constructor.  Method setThreadName returns true if the
	   * thread name has been set successfully.  THe method returns
	   * false if the thread has already started execution, unless
	   * called on CActive's thread.
	   */
	  bool setThreadName (const std::string& theThreadName);

	  /**
	   * Method getThreadCount returns the number of currently running threads. 
	   */
	  static UINT getThreadCount ();

	  /**
	   * Method getThreadInstance returns the thread instance couunt. 
	   * If this is the nth CActive thread started the threads instance
     * count will be n.
     */
    UINT getThreadInstance () const;

	  /**
	   * Method waitForZeroThreads will wait for all the CActive threads to exit
	   * before returning from the method. 
	   * theTimeOut is the number of seconds to wait before returning in the 
     * case that not all the threads have completed execution.
	   * Method waitForZeroThreads returns true if all threads have exited.
	   * The method returns false if there is a time out.
	   */
	  static bool waitForZeroThreads (const ULONG theTimeOut);

	  /**
 	   * Method setThreadName is called to set the name of a thread.
 	   * @param[in] theThreadName is the name to set for the the thread.
	   * @param[in] theThreadId is the id of the thread whose name is to be set. Set to -1 if
 	   * calling from within the thread to be named.
	   * This function is documented as being callable from outside of the thread which is being
 	   *  named, however it appears that it works more reliably if called from within the code of
	   * the thread being name, passing a threadId of -1 to indicate "current thread"
	   */
  void setThreadName (const std::string& theThreadName, const DWORD theThreadId /* = -1*/);

  private:

	  /**
	   * Method ThreadStub is the method that is used to identify the function 
	   * that represents the thread for the instance. 
	   * pObject is the pointer to the active object that provides the method
	   * that executes as part of the thread.
	   */ 
	  static unsigned int __stdcall threadStub (void *pObject);

	  /**
	   * Method ThreadRoutine is the default thread for the active object. The 
	   * derived class must override this method that will then represent the 
	   * thread of control for the active object.
	   */
	  virtual void threadRoutine () = 0;

    /// not copiable
    CActive(const CActive&);
    const CActive& operator=(const CActive&);

    /** 
	   * Method initializeThread is called to set the initial state of the thread
	   * as required by the constructors.
	   * thePriority is the default priority of the thread instance.
	   * initialiseThread returns zero if the thread is created successfully otherwise
	   * the _beginthreadex error number is return.
	   */
	  errno_t initialiseThread (int thePriority);

		/**
		 * Method getInstanceLogger returns the logger for this class instance and allows 
		 * the instance to be used by the associated CActive thread.
		 */
		log4cpp::Category* getInstanceLogger ();
	  
}; // class CActive

#endif //	 !defined (ACTIVE_H)
