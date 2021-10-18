/**
 * Title: CThreadIt
 * Description: Class CThreadIt provides a mechanism to utilise a separate thread of
 * execution to perform processing on behalf of clients with a processing
 * completion notification being sent to the client. This would find use
 * when a User Interface must perform a lengthy process that may tie up
 * the screen response. The processing may be delegated to an instance
 * derived from this class for execution in separate thread thus not
 * impacting on the user response.
 *
 * Class CThreadIt supports the execution of work packages on request from
 * clients in a separate thread of execution. When processing is complete,
 * the client is advised by an asynchronoust notification mechanism.
 *
 * Class CThreadIt is used as the base class when implement methods that
 * perform work on behalf of clients. A work package is allocated to a
 * method that is then invoked each time the client requests the work
 * package be executed. The customised class provides these methods and the
 * work execuction protocol is defined by the CThreadIt class.
 *
 * The support consists of three classes noted as follows:
 * CWorkPackIt	 - This class describes the work to be performed.
 * CThreadIt		 - Implements the protocol that defines how clients are
 *								 able to execute work in separate thread.
 * Derived Class - Defines the processing to perform in response to work requests.
 *
 * Change history:
 * 2010-06-08: Use of shared pointers and ability to provide pointer to self.
 * When sending a message to another threadit with the intent of making that threadit
 * reply to use using a reference to ourself. The issue is that we can dissapear before
 * that threadit can reply to us. Our reference to ourselves can be obtained from the
 * this pointer and sent in the CWorkPackIt. So as long as there is a valid shared pointer
 * from which this instance was created we can use that approach.
 * Useful reference for this change:
 * http://stackoverflow.com/questions/712279/what-is-the-usefulness-of-enable-shared-from-this
 *
 * Copyright: Copyright (c) 2008 Ashkel Software
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2006/12/19 02:38:06 $
 */

// Includes
#include "stdafx.h"
#include "dataitem.h"
#include "ThreadIt.h"

static char const * const PARENT_CATEGORY = "threadit.";
	/** MODULE_NAME Name allocated to this module. This is used for logging and component
	  * identification purpose */
const std::string CThreadIt::MODULE_NAME = "CThreadIt";

/**
 * Method CThreadIt is the constructor for the class. The method sets the
 * initial values for the member variables, sets the thread to be executed
 * at default priority and starts thread execution.
 */
CThreadIt::CThreadIt () : CActive(std::string(PARENT_CATEGORY) + MODULE_NAME)  // any CActive created by CThreadIt should be identified as part of a CThreadIt
{
	// Perform the standard initialisation.
	threadItInit (std::string(PARENT_CATEGORY) + MODULE_NAME);
	// Start the thread execution.
	startThread ();
} // cTreadIt

/**
 * Method CThreadIt is the constructor for the class. The method sets the
 * initial values for the member variables, sets the priority of the thread
 * and then starts thread execution.
 * Priority specifies the priority of the thread of execution associated with this instance.
 */
CThreadIt::CThreadIt (int Priority) : CActive (std::string(PARENT_CATEGORY) + MODULE_NAME, Priority)  // any CActive created by CThreadIt should be identified as part of a CThreadIt
{
	// Perform the standard initialisation.
	threadItInit (std::string(PARENT_CATEGORY) + MODULE_NAME);
	// Start the thread execution.
	startThread ();
} // CThreadIt

/**
 * Method CThreadIt is the constructor for the class. The method sets the
 * initial values for the member variables, sets the name associated with
 * the thread and then sets the priority of the thread to the default priority
 * and then starts thread execution.
 * theThreadName is the name allocated to the this thread instance.
 */
CThreadIt::CThreadIt (const std::string& theThreadName) : CActive (theThreadName  + std::string(".") + MODULE_NAME)
{
	// Perform the standard initialisation.
	threadItInit (theThreadName + std::string(".") + MODULE_NAME);
	// Start the thread execution.
	startThread ();
} // constructor CThreadIt

/**
 * Method CThreadIt is the constructor for the class. The method sets the
 * initial values for the member variables, sets the name associated with
 * the thread and then sets the priority of the thread
 * and then starts thread execution.
 * thePriority specifies the priority of the thread of execution associated with this instance.
 * theThreadName is the name allocated to the this thread instance.
 */
CThreadIt::CThreadIt (const std::string& theThreadName, int thePriority) : CActive (theThreadName + std::string(".") + MODULE_NAME, thePriority)
{
	// Perform the standard initialisation.
	threadItInit (theThreadName + std::string(".") + MODULE_NAME);
	// Start the thread execution.
	startThread ();
} // CThreadIt

/**
 * Method CThreadItInit performs the shared initialisation code for the class.
 * theThreadName is used to determine the name of the thread with a component
 * prefix as an option. In otherwords a user defined hierarchy can be implemented
 * with .CThreadIt attached to the end. Used for selective logging and component
 * identification purposes.
 */
void CThreadIt::threadItInit (const std::string& theThreadName)
{
	int Cntr = 0;

	m_ptheLogger = &(log4cpp::Category::getInstance (theThreadName));
	if (m_ptheLogger->isDebugEnabled ()) { m_ptheLogger->debug ("starting instance"); }
	// Initialise the member variables into defined states.
	// Setup the Mutex for the global data of this instance.
	m_Access = CreateMutex (NULL, FALSE, NULL);
	// Setup a Mutex for the timing information.
	m_TimeAccess = CreateMutex (NULL, FALSE, NULL);
	// Initialise the worker methods.
	for (Cntr = 0; Cntr < MAX_WORK_METHODS; Cntr++)
	{
		m_WorkerMethod[Cntr] = NULL;
	} // if
	// Initialise the event methods.
	for (Cntr = 0; Cntr < MAX_EVENT_METHODS; Cntr++)
	{
		m_EventMethod[Cntr].theEventHandler = NULL;
		m_EventMethod[Cntr].htheEvent = NULL;
	} // if
	// Set the work packet identity counter.
	m_WorkPackID = 0;
	m_theEventMethodCount = 0;
	// The thread is executing.
	m_isExitThread = false;
	// Initialise the timing variables.
	m_TStart = 0;
	m_TStop = 0;
	m_IsTiming = FALSE;
	// Set the timing period.
	m_TimePeriod = 2000;
	m_TimeOut		 = m_TimePeriod;
	// Set the periodic method.
	m_PeriodicMethod = NULL;
	// The event information is setup.
	m_ResetEventInfo = FALSE;
} // threadItInit

/**
 * Method ~CThreadIt is the destructor for the class. The method waits until
 * the thread of execution stops and then releases the resources used by the
 * instance.
 */
CThreadIt::~CThreadIt ()
{
	// Clear all queues.
	m_WorkQ.clear ();
	m_DoneQ.clear ();
	// Close open handles.
	CloseHandle (m_Access);
	CloseHandle (m_TimeAccess);
} // ~CThreadIt

// Client Interface Methods

/**
 * Method addObserver adds an observer that can receive asynchronous notifications of
 * subject changes. The notification or update is performed via the subject in the
 * the worker, period or event method implementation.
 * ptheObserver is a reference to the instance that wants to be informed of subject
 * changes or updates.
 */
bool CThreadIt::addObserver (CObserver* ptheObserver)
{
	bool isAdded = false;

	if (ptheObserver != NULL)
	{
		m_theCallback.attach (ptheObserver);
		isAdded = true;
	} // if
	return isAdded;
} // addObserver

/**
 * Method removeObserver removes the specified observer from the list of observers.
 * ptheObserver is a reference to the instance that no longer wants to be informed
 * of subject changes or updates.
 */
bool CThreadIt::removeObserver (CObserver* ptheObserver)
{
	bool isRemoved = false;

	if (ptheObserver != NULL)
	{
		m_theCallback.detach (ptheObserver);
		isRemoved = true;
	} // if
	return isRemoved;
}	 // removeObserver

/**
 * Method clearObservers removes all observers from being notified of changes or updates
 * from the worker, period or event method implementations.
 */
void CThreadIt::clearObservers ()
{
	m_theCallback.removeObservers ();
} // clearObservers

/**
 * Method startWork provides a work package that defines the work to be
 * performed by the thread. The work pack is placed in queue to be processed
 * when a work slot is available.
 * WorkPack is The work package to be performed. Note that the memory
 * allocated to WorkPack and all its internal references (eg: pObject) should
 * be allocated off the heap. Ownership passes to the ThreadIt instance and
 * then to the worker method where the memory must be freed. It is important that
 * the caller of startWork realize that once the call to startWork has been made
 * there should be no reference made to the workpackit or any of its data.
 * WorkPackID is a unique reference to the work package. The caller can use
 * this value to track this work.
 * Method startWork returns true if the work package is placed in the work
 * queue successfully.
 */
bool CThreadIt::startWork (CWorkPackIt*& pWorkPack, ULONG& WorkPackID)
{
	bool	Success = TRUE;
	DWORD Result;

	// Get a unique work packet number for this caller.
	Result = WaitForSingleObject (m_Access, INFINITE);
	// Check if the value can be incremented.
	if (m_WorkPackID >= ULONG_MAX)
	{
		// Reset the counter.
		m_WorkPackID = 0;
	}
	else
	{
		// Now increment the value.
		m_WorkPackID++;
	} // if
	// Return this to the caller.
	WorkPackID = m_WorkPackID;
	// Setup the work package identity.
	pWorkPack->m_theWorkPackID = WorkPackID;
	// Release the mutex.
	ReleaseMutex (m_Access);
	// Now send the work package on for execution.
	m_WorkQ.insertItem (pWorkPack);
	// Return the method status.
	return Success;
} // startWork

/**
 * Method getWork waits for work processing to be completed and returns
 * a WorkDoneIt package that describes the status of the work performed.
 * WorDone is the instance that describes the processing associated with a work package.
 * TimeOut inidicates how long the caller is willing to wait for the result.
 * Method GetWork returns true if a work done instance is returned from
 * the work done queue. Note that the memory allocated to WorkPack and all its
 * internal references (eg: pObject) should be allocated off the heap within the
 * worker method or event method or periodic method. Ownership passes from the
 * ThreadIt instance and then to the receiver after the return from getWork. The
 * worker method, event method or periodic method should not keep any references
 * to the information provided open especially when this memory can be freed by
 * the recipient.
 */
CWorkPackIt* CThreadIt::getWork (UINT TimeOut)
{
	CWorkPackIt* pWork = NULL;

	pWork = m_DoneQ.waitItem (TimeOut);
	// Return the method status.
	return pWork;
} // GetWork

/**
 * Method GetWorkDoneQ returns a pointer to the work done queue. This is
 * provided to allow custom handling of the queue.
 * pWorkDoneQ is the pointer to the work done queue.
 */
void CThreadIt::getWorkDoneQ (CProtectedQueue <CWorkPackIt>* pDoneQ)
{
	pDoneQ = &m_DoneQ;
} // GetWorkDoneQ

/**
 * Method GetWorkQ returns a pointer to the work pending queue. This is
 * provided to allow custom handling of the queue.
 * pWorkQ is the pointer to the work pending queue.
 */
void CThreadIt::getWorkQ (CProtectedQueue <CWorkPackIt>* pWorkQ)
{
	pWorkQ = &m_WorkQ;
} // GetWorkQ

/**
 * Method getSelfThreadItPtr returns a shared pointer to this instance - that is to itself.
 * This is useful when telling other instances to reply to to me in response to a message.
 * sent to them.
 * \return a shared pointer to myself. This pointer is obtained using
 * shared_from_this ().
 */
 ThreadItPtr CThreadIt::getSelfThreadItPtr ()
 {
	 return shared_from_this ();
 } // getSelfThreadItPtr

/**
 * Method ThreadRoutine represents the thread of execution for the instance.
 * On start, the thread waits for work packages in the work queue. When
 * a package is found the associated worker thread is invoked to perform
 * the work. On completion, the client is notified of work completion and
 * the process repeats.
 */
void CThreadIt::threadRoutine ()
{
	bool	Success = FALSE;
	UINT	theEventCounter = 0;
	DWORD Result = 0;
	DWORD Elapsed = 0;
	ULONG WorkInstruction = 0;
	ULONG EventId = 0;;
	HANDLE WorkQSem = NULL;
	// TimedWork is the default work request for the periodic function.
	CWorkPackIt TimedWork;
	CWorkPackIt* pWorkPack = NULL;
	CWorkPackIt* pWorkDone = NULL;
	// hEventList is the list of events that we wait on.
	HANDLE hEventList[MAX_EVENT_METHODS + 1];

	// Initialise the event list.
	for (int i = 0; i < MAX_EVENT_METHODS + 1; i++)
	{
		hEventList[i] = NULL;
	} // for
	// Get the semaphore that indicates the arrival of work instructions.
	WorkQSem = m_WorkQ.getQSemaphore ();
	// Start a timing operation.
	m_Period.startTiming (m_TimePeriod);
	// Set the timeout to match.
	m_TimeOut = m_TimePeriod;
	// We must setup events for the first time round.
	m_ResetEventInfo = TRUE;
	// Perform the data processing.
	do
	{
		// Setup the wait objects.
		if (m_ResetEventInfo)
		{
			// Waiting on the work queue is always defined.
			hEventList[0] = WorkQSem;
			theEventCounter = 1;
			// We will wait on each event that is defined.
			for (int theCount = 0; theCount < MAX_EVENT_METHODS; theCount++)
			{
				if (m_EventMethod[theCount].htheEvent != NULL)
				{
					hEventList[theEventCounter] = m_EventMethod[theCount].htheEvent;
					theEventCounter++;
				} // if
			} // while
			// Event information has been setup.
			m_ResetEventInfo = FALSE;
		} // if (m_ResetEventInfo)
		// Wait for for a work instruction to arrive, an event to occur or for a time out.
		// The following change was made to support asynchronous callbacks.
		// Result = WaitForMultipleObjects	(theEventCounter, hEventList, FALSE, m_TimeOut);
		// End of change
		if (!m_isExitThread)
		{
			Result = WaitForMultipleObjectsEx	 (theEventCounter, hEventList, FALSE, m_TimeOut, TRUE);
		} // if
		// Process the outcome of the wait.
		if ((!m_isExitThread) && (Result == WAIT_OBJECT_0))
		{
			// Get the item withoug decrementing the signal count. This is already done by the
			// call to WaitForMultipleObjects. Not also that because the destructor signals the thread
			// it is possible to get two events in a row but one is because the thread needs to exit.
			pWorkPack = m_WorkQ.getItemNoDec ();
			if (pWorkPack != NULL)
			{
				// Copy the WorkPack into the WorkDone structure. This caters for the case where there
				// is no pWorkDone returned or provided due to error conditions. There may be better ways
				// to handle this condition such as write a log record rather than return a result.
				pWorkDone = pWorkPack;
				// Set the work instruction status.
				Success = FALSE;
				// Perform the work according to the work instruction given.
				WorkInstruction =	 pWorkPack->getWorkInstruction ();
				// Check that a valid work instruction has been given.
				if ((WorkInstruction >= 0) && (WorkInstruction < MAX_WORK_METHODS))
				{
					// Make sure that a method has been provided to perform the work
					// instruction.
					if (m_WorkerMethod[WorkInstruction] != NULL)
					{
						// Measure the execution time of this work.
						startTiming (pWorkPack->m_theTimeAllowed);
						// Execute the work according to the work instruction.
						Success = (this->*m_WorkerMethod[WorkInstruction])(pWorkPack, pWorkDone);
						// pWorkDone must not be null here.
						if (pWorkDone != NULL)
						{
							// Get the time to completion.
							stopTiming (pWorkDone->m_theTimeElapsed);
						} // if
					}
					else
					{
						// No method specified for this work instruction.
						pWorkDone->m_theStatus = WORKDONE_NO_METHOD;
						m_ptheLogger->error ("No method specified for work instruction");
					} // if
				}
				else
				{
					// Invalid work instruction given.
					pWorkDone->m_theStatus = WORKDONE_INVALID_INSTRUCTION;
					m_ptheLogger->error ("Invalid work instruction specified");
				} // if
				// Now that the work is done. Send a response back the issuer. Send a result to the user if requested.
				sendResponse (pWorkDone, WorkInstruction, false);
			}
			else
			{
				m_ptheLogger->error ("The work pack input is null - work cannot be performed");
			} // if (IsWorkToDo)
		} // if (Result == WAIT_OBJECT_0)
		// Check if an event has occured.
		if ((!m_isExitThread) && (Result > WAIT_OBJECT_0) && (Result <= WAIT_OBJECT_0 + theEventCounter))
		{
			// Determine the event identification.
			EventId = Result - WAIT_OBJECT_0;
			// Make sure that an evetn method has been provided handle the event.
			if ((EventId > 0) && (EventId < theEventCounter) && (m_EventMethod[EventId - 1].theEventHandler != NULL))
			{
				// Setup the work request.
				TimedWork.initialise ();
				// Measure the execution time of this work.
				startTiming (TimedWork.m_theTimeAllowed);
				// We must not provide a memory copy.
				pWorkDone = NULL;
				// Execute the method.
				Success = (this->*m_EventMethod[EventId - 1].theEventHandler) (TimedWork, pWorkDone);
				if (Success)
				{
					if (pWorkDone != NULL)
					{
						// Get the time to completion.
						stopTiming (pWorkDone->m_theTimeElapsed);
					} // if
				}
				else
				{
					// No event method specified for this event occurence.
					m_ptheLogger->error ("Event method not specified");
				} // if
			}
			else
			{
				// Invalid work instruction given.
				m_ptheLogger->error ("Invalid event method");
			} // if
			// Now that the work is done. Send a response back the issuer if necessary.
			sendResponse (pWorkDone, EventId, false);
		}	 // if ((Result > WAIT_OBJECT_0) && (Result <= WAIT_OBJECT_0 + MAX_EVENT_METHODS))
		// Check if periodic processing is required.
		if ((!m_isExitThread) && (m_PeriodicMethod != NULL)	 && (m_Period.isExpired (Elapsed)))
		{
			// There is a time out waiting for an incoming message or the threads period timer has expired.
			// Execute the periodic mehtod and measure the execution time of this work.
			startTiming (m_TimePeriod);
			// Setup the work request.
			TimedWork.initialise ();
			// Copy the WorkPack into the WorkDone structure.
			pWorkDone = NULL;
			// Execute the method.
			if ((this->*m_PeriodicMethod) (TimedWork, pWorkDone))
			{
				if (pWorkDone != NULL)
				{
					// Get the time to completion.
					stopTiming (pWorkDone->m_theTimeElapsed);
					// The period method requires a result to be returned.
					WorkInstruction = TimedWork.getWorkInstruction ();
					pWorkDone->m_theInstruction = WorkInstruction;
					// Now that the work is done. Send a response back the issuer.
					sendResponse (pWorkDone, WorkInstruction, true);
				}
				else
				{
					// If the workdone is not set then ignore for general operation.
					if (m_ptheLogger->isDebugEnabled ()) { m_ptheLogger->debug ("Error during period method execution - workdone is null"); } // if 
				} // if
			} // if
			// Start timing again.
			m_Period.startTiming (m_TimePeriod);
			m_TimeOut = m_TimePeriod;
		}
		else
		{
			// The timer has not expired. Count down the remaining time
			// on this timer. This code is provided here until the cTimeIt
			// class is fixed to provide this service.
			m_TimeOut = m_TimePeriod - Elapsed;
			if (m_TimeOut <= 0)
			{
				// The timout has occured. The timer will detect this the
				// next time round and start timing again.
				m_TimeOut = 0;
			} // if
		} // if ((m_PeriodicMethod != NULL)	 && (m_Period.IsExpired (Elapsed))
	} while (!m_isExitThread);
	// Notify any interested parties that this thread is now exiting.
} // ThreadRoutine

/**
 * Method SendResponse checks if a response to work is required and then
 * interprets the work done settings to send off the response. This method
 * is provided in the event that work processing requires the generation
 * of multiple responses per work item. This allows the work handler method
 * to place reponses in queues and to generate events associated with them.
 * WorkDone is the work done instance whose settings are used to determine
 * the type of response required.
 * WorkId us the identifier allocated to the work performed.
 * Method SendResponse returns true if the response to a work package
 * has been generated successfully.
 */
bool CThreadIt::sendResponse (CWorkPackIt*& pWorkDone, ULONG WorkId, bool isPeriodic)
{
	bool isSuccess = false;
	bool m_isNotifyWithCallback = false;

	// There is a timing issue in that if you send a response and do a callback you
	// may get the response before the callback is complete. For this reason the callback is checked
	// and done first.
	// but we want to take it out of the queue and use the result so it has to be placed in the queue first.
	// then the user informed.
	// an additional issue arises that if the user wants a callback and wants a result it is possible for the
	// user to get the result, delete it and this method then tries to set up the callback.
	// How does this manifest- case is where client code gets the work pack and deletes it and then the values
	// returned by the call back or th deletion of the workpack is problematic (unexpected values).
	// This is because the workpack is retained by the client and deleted before this method finishes. We need
	// to use the workpack before any such occurrences can take place.
	// We still have to notify with the callback after placing it in the queue in case the client wants the callback
	// to notify of the presence of the data in the queue.

	// check for null
	if (pWorkDone != NULL)
	{
		m_isNotifyWithCallback = pWorkDone->m_isNotifyWithCallback;
		if (m_isNotifyWithCallback)
		{
			// Setup for callback first (in case workpack is deleted quickly before this method completes.
			m_theCallback.setWorkInstruction (pWorkDone->m_theInstruction);
			m_theCallback.setWorkId (WorkId);
			m_theCallback.setPeriodic (isPeriodic);
			// If specified to pass data
			if (pWorkDone->m_isObjectInCallback)
			{
				m_theCallback.setDataItem (pWorkDone->m_ptheDataItem);
		} // if
	} // if
		// Send a result to the user if requested.
		if (pWorkDone->m_isSendResult)
		{
			// Return a reference to this instance of CThreadIt
			pWorkDone->m_ptheSource = this;
			// Insert it into the queue for the issuer to pick up.
			if (pWorkDone->m_isUseDefaultQ)
			{
				m_DoneQ.insertItem (pWorkDone);
			}
			else
			{
				// Check that the provided queue exists.
				if (pWorkDone->m_ptheWorkDoneQ != NULL)
				{
					pWorkDone->m_ptheWorkDoneQ->insertItem (pWorkDone);
				}
				else
				{
					pWorkDone->m_theStatus = WORKDONE_INVALID_DONE_QUEUE;
				} // if
			} // if
		}
		else
		{
			// Free the memory as it is no longer needed.
			delete pWorkDone;
			pWorkDone = NULL;
		} // if (WorkDone.SendResult)
		if (m_isNotifyWithCallback)
		{
			// Make sure that we can catch any exception that is thrown. Unfortunately
			// we are unable to identify the particular cause from the client code.
			try
			{
				m_theCallback.notifyOnChange (m_theCallback);
			} // try
			catch (...)
			{
				// We have caught the exception bug do not know what it is.
				m_ptheLogger->error ("Unexpected exception caught during callback");
			} // catch
		} // if (WordDone.NotifyWindow)
		isSuccess = true;
	} // if
	// Return the method status.
	return isSuccess;
} // SendResponse

/**
 * Method SetWorkerMethod associates member functions of a derived class with
 * work instructions. This implies that when a work instruction is received
 * in the work queue, the member function associated with the work
 * instruction will be invoked to perform the work. This member should be
 * called from the derived class before any work instruction is sent to the
 * instance.
 * Method is the method that will be required to perform work according to
 * a work instruction associated with a work package.
 * Instruction is the work instruction that the class member is to be
 * associated with. Values range from 1 to MAX_WORK_METHODS.
 * Method SetWorkerMethod returns true if the member function is setup
 * successfully.
 */
bool CThreadIt::setWorkerMethod (WorkerMethodType Method, UINT Instruction)
{
	bool Success = FALSE;

	if (Instruction < MAX_WORK_METHODS)
	{
		// Add this method.
		m_WorkerMethod[Instruction] = (WorkerMethodType)Method;
		Success = TRUE;
	} // if
	// Return the method status.
	return Success;
} // SetWorkerMethod

/**
 * Method SetPeriodicMethod associates member functions of a derived class
 * with the periodic method. This implies that when a time period
 * elapses, the given periodic method is executed. The periodic methods
 * is able to return a CWorkPackIt instance in the work done queue.
 * Method is the method that will be required to perform periodic work processing.
 * Method SetPeriodoicMethod returns true if the member function is setup
 * successfully.
 */
bool CThreadIt::setPeriodicMethod (PeriodicMethodType Method)
{
	bool Success = TRUE;

	// Add this method.
	m_PeriodicMethod = (PeriodicMethodType)Method;
	// Return the method status.
	return Success;
} // SetPeriodicMethod

/**
 * Method SetEventMethod associates member functions of a derived class with
 * events. This implies that when a waitable event occurs, the member
 * function associated with the event will be invoked to handle the event.
 * This method must only be called by the associated thread.
 * theEventId is the location of the waitable object in the array
 * of waitable events. There must not be any gaps in this array.
 * EventHandler is the method that will be invoked in response to the event.
 * hEvent is the handle to the waitable object that indicates the event.
 * The event methods cannot be cleared once they have been set.
 * Method SetEventMethod returns true if the member function is setup to
 * handle the associated event successfully.
 */
bool CThreadIt::setEventMethod (ULONG& theEventId, EventMethodType EventHandler, HANDLE hEvent)
{
	bool isSuccess = false;

	// Add this method.
	if ((m_theEventMethodCount < MAX_EVENT_METHODS) && (EventHandler != NULL) && (hEvent != NULL))
	{
		m_EventMethod[m_theEventMethodCount].theEventHandler = (EventMethodType)EventHandler;
		m_EventMethod[m_theEventMethodCount].htheEvent = hEvent;
		theEventId = m_theEventMethodCount;
		m_theEventMethodCount++;
		isSuccess = true;
		// The event information needs to be setup again.
		m_ResetEventInfo = true;
	} // if
	// Return the method status.
	return isSuccess;
} // setEventMethod

/**
 * Method GetEventMethod returns the associated member functions of a
 * derived class with events.
 * EventId is the value returned as the event id when the setEventMethod was called.
 * EventHandler is the method that will be invoked in response to the event.
 * hEvent is the handle to the waitable object that is associated with the event.
 * Method GetEventMethod returns true if the event and handler information
 * is returned correctly.
 */
bool CThreadIt::getEventMethod (ULONG theEventId, EventMethodType& EventHandler, HANDLE& hEvent)
{
	bool isSuccess = false;

	// Store the event information.
	if (theEventId < MAX_EVENT_METHODS)
	{
		// Add this method.
		EventHandler = m_EventMethod[theEventId].theEventHandler;
		hEvent = m_EventMethod[theEventId].htheEvent;
		isSuccess = true;
	} // if
	// Return the method status.
	return isSuccess;
} // getEventMethod

/**
 * Method SetPeriod sets the period time at which the periodic method is invoked.
 * Period is the timer period in milliseconds that sets the time period between
 * Periodic Method invocations is invoked. A value of zero turns of period timing.
 * This method should only be called from Worker Methods or from the Periodic method itself.
 */
void CThreadIt::setPeriod (DWORD Period)
{
	// Check if the time period has changed or not.
	if (m_TimePeriod != Period)
	{
		if (Period == 0)
			m_TimePeriod = INFINITE;
		else
			m_TimePeriod = Period;
		// Start timing again.
		m_TimeOut = m_TimePeriod;
		m_Period.startTiming (m_TimePeriod);
	} // if
} // SetPeriod

/**
 * Method IsAvailableTime is provided for use in the WorkerMethodType function
 * to determine if the time allowed for processing has elapsed or not. If
 * the time available has elpased then the worker method should stop
 * processing and return the status WORKDONE_TIME_OUT in the CWorkPackIt result.
 * Elapsed returns the time elapsed so far for work execution in milliseconds.
 * If return value of zero and the method returns TRUE indicates that timing is
 * not currently in progress.
 * Method IsAvailableTime returns true if there is still still work
 * execution time available.
 */
bool CThreadIt::isAvailableTime (DWORD& Elapsed)
{
	bool StillTime;

	// Secure access to the timing variables.
	WaitForSingleObject (m_TimeAccess, INFINITE);
	// Get the time elapsed so far.
	timeElapsed (Elapsed);
	// Check if there is still time available.
	if (m_TimeAllowed == 0)
		StillTime = TRUE;
	else
		StillTime = (m_TimeAllowed > Elapsed);
	// Release the mutex.
	ReleaseMutex (m_TimeAccess);
	// Return the method result.
	return StillTime;
} // IsAvailableTime

/**
 * Method StartTiming is called to record the start of work execution timing.
 * TimeAllowed specifies the time allocated for work to be executed.
 * A value of zero indicates that there is no timing restriction.
 */
void CThreadIt::startTiming (DWORD TimeAllowed)
{
	// Secure access to the timing variables.
	WaitForSingleObject (m_TimeAccess, INFINITE);
	// Record the time allowed for work execution.
	m_TimeAllowed = TimeAllowed;
	// Start the timer.
	m_TStart = GetTickCount ();
	// Timing is in progress.
	m_IsTiming = TRUE;
	// Release the mutex.
	ReleaseMutex (m_TimeAccess);
} // StartTiming

/**
 * Method StopTiming is called to record the end of work execution timing.
 * Elapsed returns the time elapsed so far for work execution in milliseconds.
 */
void CThreadIt::stopTiming (DWORD& Elapsed)
{
	// Secure access to the timing variables.
	WaitForSingleObject (m_TimeAccess, INFINITE);
	// Get the time Elapsed so far.
	timeElapsed (Elapsed);
	// Stop timing.
	m_IsTiming = FALSE;
	// Release the mutex.
	ReleaseMutex (m_TimeAccess);
} // StopTiming

/**
 * Method TimeElapsed calculates how much time has so far elapsed since timing
 * was started.
 * Elapsed returns the time elapsed since timing was started.
 * Method TimeTake returns true if timing is currently in progress.
 */
bool CThreadIt::timeElapsed (DWORD& Elapsed)
{
	bool IsTiming;

	// Start the timer.
	m_TStop = GetTickCount ();
	// Calculate the time elapsed so far.
	// Get the time differences from the start to the end.
	if (m_TStop >= m_TStart)
		// The timer has not wrapped.
		Elapsed = m_TStop - m_TStart;
	else
		// The timer has wrapped around.
		Elapsed = (UINT_MAX - m_TStart) + m_TStop;
	// Indicate if timing is in progress or not.
	IsTiming = m_IsTiming;
	// Return result.
	return IsTiming;
} // TimeElapsed

/**
 * Method StartThread resumes the execution of the thread of control for the instance.
 */
void CThreadIt::startThread ()
{
	CActive::startThread ();
} // StartThread

/**
 * Method WaitForThreadToStop can be called to wait for the thread to stop execution.
 */
void CThreadIt::waitForThreadToStop ()
{
	CActive::waitForThreadToStop ();
} // WaitForThreadToStop

/**
 * Method StopThread stops the execution of the thread of control for the instance.
 */
void CThreadIt::stopThread ()
{
	HANDLE WorkQSem;

	// Indicate that the thread must now terminate execution.
	m_isExitThread = true;
	// We also need to get the thread to release from any waiting it is doing.
	// Get the semaphore that indicates the arrival of work instructions.
	WorkQSem = m_WorkQ.getQSemaphore ();
	// Now release the thread from the wait for at least one iteration sufficient
	// to determine that the thread needs to exit.
	if (WorkQSem != NULL)
	{
		ReleaseSemaphore (WorkQSem, 1, NULL);
	} // if
} // StopThread

/**
 * Method isExitThread is called internally to check if the thread of
 * execution is required to stop.
 */
bool CThreadIt::isExitThread ()
{
	// Return the thread exit settting.
	return m_isExitThread;
} // isExitThread

/**
 * Method checkParams is called to assist when checking input parameters to the worker method. The aim is to simplify the task
 * of the programmer in performing repeated error checks for every method. This particular method returns the object
 * pointer in the ptheWorkPack.
 * @param[in] ptheWorkPack is check that it is not NULL and that it contains an object pointer.
 * @param[out] ptheWorkDone returns a new CWorkPackIt to return the result. If isCopy is true then this is a copy of the
 * ptheWorkPack parameter.
 * @param[out] ptheObj returns the object pointer to the object in ptheWorkPack.
 * @param[out] theStatus returns the status of the parameter check. It has a value of
 * THREADIT_STATUS_OK - parameters check out ok.
 * THREADIT_STATUS_PARAM_OBJECT_NULL - the expected object parameter is NULL.
 * THREADIT_STATUS_PARAM_WORK_PACK_NULL - the input work packet is NULL.
 * @param isCopy[in] indicates if the input ptheWorkPack should be copied to the output ptheWorkDone.
 * \return true true if the check on the parameters is successful.
 * If the check fails then the ptheWorkDone is always returned as a new CWorkPackIt. If ptheWorkPack is not NULL and
 * isCopy is specified then ptheWorkDone will be a copy of ptheWorkPack. The ptheObj is returned as NULL on failure.
 * If the check fails then the string representation of the error status will be written as an error to the log file.
 */
bool CThreadIt::checkParams (CWorkPackIt* ptheWorkPack, CWorkPackIt*& ptheWorkDone, void*& ptheObj, long& theStatus, bool isCopy) const
{
	bool isSuccess = false;

	ptheObj = NULL;
	ptheWorkDone = NULL;

	if (ptheWorkPack)
	{
		ptheObj = ptheWorkPack->m_ptheObject;
		if (ptheObj)
		{
			theStatus = CThreadIt::THREADIT_STATUS_OK;
			isSuccess = true;
		}
		else
		{
			theStatus = CThreadIt::THREADIT_STATUS_PARAM_OBJECT_NULL;
		} // if
	}
	else
	{
		theStatus = CThreadIt::THREADIT_STATUS_PARAM_WORK_PACK_NULL;
	} // if
	if ((ptheWorkPack) && (isCopy))
	{
		ptheWorkDone = new CWorkPackIt (*ptheWorkPack);
	}
	else
	{
		ptheWorkDone = new CWorkPackIt ();
	} // if
	// Return the operating status at this point.
	ptheWorkDone->m_theStatus = theStatus;
	if (!isSuccess)
	{
		logParameterError (theStatus);
	} // if
	return isSuccess;
} // checkParams

/**
 * Method checkParams is called to assist when checking input parameters to the worker method. The aim is to simplify the task
 * of the programmer in performing repeated error checks for every method. This particular method does not expect and
 * object pointer in ptheWorkPack.
 * @param[in] ptheWorkPack is check that it is not NULL and that it contains an object pointer.
 * @param[out] ptheWorkDone returns a new CWorkPackIt to return the result. If isCopy is true then this is a copy of the
 * ptheWorkPack parameter.
 * @param[out] theStatus returns the status of the parameter check. It has a value of
 * THREADIT_STATUS_OK - parameters check out ok.
 * THREADIT_STATUS_PARAM_WORK_PACK_NULL - the input work packet is NULL.
 * @param isCopy[in] indicates if the input ptheWorkPack should be copied to the output ptheWorkDone.
 * \return true true if the check on the parameters is successful.
 * If the check fails then the ptheWorkDone is always returned as a new CWorkPackIt. If ptheWorkPack is not NULL and
 * isCopy is specified then ptheWorkDone will be a copy of ptheWorkPack.
 * If the check fails then the string representation of the error status will be written as an error to the log file.
 */
bool CThreadIt::checkParams (CWorkPackIt* ptheWorkPack, CWorkPackIt*& ptheWorkDone, long& theStatus, bool isCopy) const
{
	bool isSuccess = false;

	ptheWorkDone = NULL;

	if (ptheWorkPack)
	{
		theStatus = CThreadIt::THREADIT_STATUS_OK;
		isSuccess = true;
	}
	else
	{
		theStatus = CThreadIt::THREADIT_STATUS_PARAM_WORK_PACK_NULL;
	} // if
	if ((ptheWorkPack) && (isCopy))
	{
		ptheWorkDone = new CWorkPackIt (*ptheWorkPack);
	}
	else
	{
		ptheWorkDone = new CWorkPackIt ();
	} // if
	// Return the operating status at this point.
	ptheWorkDone->m_theStatus = theStatus;
	if (!isSuccess)
	{
		logParameterError (theStatus);
	} // if
	return isSuccess;
} // checkParams

/**
 * Method logParameterError is called to log the error string in the log as an error
 * that represents theStatus. These are the enum StatusIds values.
 * @param[in] theStatus is the status value to be logged to the log file.
 */
void CThreadIt::logParameterError (long theStatus) const
{
	std::string theError = toParameterErrorStr (theStatus);
	// Log the parameter issue as an error.
	m_ptheLogger->error ("Status error: " + theError);
} // logParameterError

/**
 * Method toParameterErrorStr is called to return the error string
 * that represents theStatus value. These are the enum StatusIds values.
 * @param[in] theStatus is the status value to be returned as string.
 * \return the string representation of theStatus.
 */
std::string CThreadIt::toParameterErrorStr (long theStatus) const
{
	std::string theStr;

	switch (theStatus)
	{
		case THREADIT_STATUS_START :
			theStr = "ThreadIt: First status message";
			break;
		case WORKDONE_NO_RESULT :
			theStr = "ThreadIt: no work done yet";
			break;
    case WORKDONE_INVALID_INSTRUCTION :
			theStr = "ThreadIt: invalid work instruction";
			break;
    case WORKDONE_NO_METHOD :
			theStr = "ThreadIt: no method for work instruction";
			break;
    case WORKDONE_TIME_OUT :
			theStr = "ThreadIt: timeout waiting for work to complete";
			break;
    case WORKDONE_INVALID_DONE_QUEUE :
			theStr = "ThreadIt: queue to return result not specified";
			break;
    case WORKDONE_INVALID_EVENT :
			theStr = "ThreadIt: an event occurred that has no valid handler";
			break;
    case WORKDONE_NO_EVENT_METHOD :
			theStr = "ThreadIt: there is no handler for the workinstruction";
			break;
		case THREADIT_STATUS_OK :
			theStr = "ThreadIt: success";
			break;
    case THREADIT_STATUS_PARAM_OBJECT_NULL :
			theStr = "ThreadIt: expected input work object is null";
			break;
		case THREADIT_STATUS_PARAM_WORK_PACK_NULL :
			theStr = "ThreadIt: input work pack is null";
			break;
		case THREADIT_STATUS_LAST :
			theStr = "ThreadIt: status last";
			break;
		default :
			theStr = "ThreadIt: status invalid";
	} // switch
	return theStr;
} // toParameterErrorStr

// Class: CWorkPackIt Implementation

/**
 * Method CWorkPackIt is the constructor for the class. The method sets the
 * initial values for the member variables.
 */
CWorkPackIt::CWorkPackIt ()
{
	initialise ();
} // CWorkPackIt

/**
 * Method Initialise clears the work packet information.
 */
int CWorkPackIt::initialise ()
{
  m_theInstruction = 0;
  m_theWorkPackID = 0;
  m_isNotifyWithCallback = false;
  m_isSendResult = false;
  m_ptheObject = NULL;
  m_ptheSource = NULL;
  m_theTimeAllowed = 0;
  m_isUseDefaultQ = true;
  m_ptheWorkDoneQ = NULL;
  m_theTimeElapsed = 0;
  m_theStatus = 0;
	// We do not use the shared queue by default.
	m_isUseSharedQ = false;
	// We do not use the queue of DataItemPtrs by default.
	m_isUseSharedPtrQ = false;
	// We do not pass the data in the callback by default.
	m_isObjectInCallback = false;
//	m_ptheDataItem = DataItemPtr (new CDataItem ());
	m_ptheDataItem = DataItemPtr ();
  return 0;
} // CWorkPackIt

/**
 * Method cWorkPacktIt is the copy constructor for the class. The new
 * instance is initialised with the same values as the supplied instance.
 * theWorkpack is the object that supplies the initial values for the new instance.
 */
CWorkPackIt::CWorkPackIt (const CWorkPackIt& theWorkPack)
{
  m_theInstruction = theWorkPack.m_theInstruction;
  m_theWorkPackID  = theWorkPack.m_theWorkPackID;
  m_isSendResult   = theWorkPack.m_isSendResult;
  m_ptheObject     = theWorkPack.m_ptheObject;
  m_ptheSource     = theWorkPack.m_ptheSource;
  m_theTimeAllowed = theWorkPack.m_theTimeAllowed;
  m_isUseDefaultQ  = theWorkPack.m_isUseDefaultQ;
  m_ptheWorkDoneQ  = theWorkPack.m_ptheWorkDoneQ;
  m_theTimeElapsed = theWorkPack.m_theTimeElapsed;
  m_theStatus      = theWorkPack.m_theStatus;
	m_isUseSharedQ	 = theWorkPack.m_isUseSharedQ;
	m_pSharedQ			 = theWorkPack.m_pSharedQ;
	m_isObjectInCallback = theWorkPack.m_isObjectInCallback;
	m_ptheDataItem = theWorkPack.m_ptheDataItem;
  m_isNotifyWithCallback = theWorkPack.m_isNotifyWithCallback;
} // constructor CWorkPackIt

/**
 * Method ~CWorkPackIt is the destructor for the class. The method releases
 * resources used by the instance.
 */
CWorkPackIt::~CWorkPackIt ()
{
  m_ptheObject = NULL;
  m_ptheSource = NULL;
  m_ptheWorkDoneQ = NULL;
	m_ptheDataItem.reset ();
} // ~CWorkPackIt

/**
 * Method operator = assigns the values of a cWorkPacktIt object to
 * another instance.
 * theWorkpack is the object that supplies the values that are assigned.
 */
CWorkPackIt &CWorkPackIt::operator=(CWorkPackIt &theWorkPack)
{
	m_theInstruction = theWorkPack.m_theInstruction;
	m_theWorkPackID	 = theWorkPack.m_theWorkPackID;
	m_isSendResult	 = theWorkPack.m_isSendResult;
	m_ptheObject		 = theWorkPack.m_ptheObject;
	m_ptheSource		 = theWorkPack.m_ptheSource;
	m_theTimeAllowed = theWorkPack.m_theTimeAllowed;
	m_isUseDefaultQ	 = theWorkPack.m_isUseDefaultQ;
	m_ptheWorkDoneQ	 = theWorkPack.m_ptheWorkDoneQ;
	m_theTimeElapsed = theWorkPack.m_theTimeElapsed;
	m_theStatus			 = theWorkPack.m_theStatus;
	m_isUseSharedQ	 = theWorkPack.m_isUseSharedQ;
	m_pSharedQ			 = theWorkPack.m_pSharedQ;
	m_isObjectInCallback = theWorkPack.m_isObjectInCallback;
	m_ptheDataItem = theWorkPack.m_ptheDataItem;
  m_isNotifyWithCallback  = theWorkPack.m_isNotifyWithCallback;
  return *this;
} // CWorkPackIt

/**
 * Method WorkInstruction returns the work instruction associated with the
 * CWorkPackIt instance. A work instruction value of zero is not a valid
 * work instruction.
 */
ULONG CWorkPackIt::getWorkInstruction ()
{
	return m_theInstruction;
} // WorkInstruction

/**
 * Method getSource returns a reference to the originating CThreadIt instance
 * from which this CWorkPackIt was sent. Note: The returned value, if not NULL, may
 * not refer to an existing CThreadIt instance if the instance has been destroyed
 * since the CWorkPackIt instance has been sent.
 */
CThreadIt* CWorkPackIt::getSource ()
{
	return m_ptheSource;
} // getSource


/**
 * Method setReplyInstruction is called to set the instruction that the recipient
 * of this message can use to reply to the message. This value can be set if the
 * value of the message source is specified and available using the method
 * getSource.
 */
void CWorkPackIt::setReplyInstruction (UINT theInstruction)
{
	m_theReplyInstructionId = theInstruction;
} // setReplyInstruction

/**
 * Method getReplyInstruction is called to get the instruction that the recipient
 * of this message can use to reply to the message. This value is normally set if the
 * value of the message source is specified and available using the method
 * getSource and the sender intended the recipient to reply.
 */
UINT CWorkPackIt::getReplyInstruction ()
{
	return m_theReplyInstructionId;
} // getReplyInstruction








