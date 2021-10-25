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
 * Class CThreadIt is used as the base class when implementing methods that
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
 * Copyright: Copyright (c) 2008 Ashkel Software
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2006/12/19 02:38:06 $
 */

#if !defined (THREADIT_H)
#define THREADIT_H

// Includes
#include <memory>
#include <log4cpp/Category.hh>
#include "Active.h"
#include "ProtectedQueue.h"
#include "mtqueue.h"
#include "TimeIt.h"
#include "threaditcallback.h"
#include "observer.h"
#include "threadit.h"

// ThreadIt: Forward Declarations
class	 CWorkPackIt;
class	 CThreadIt;

// ThreadIt: Type Definitions
/** RequestId is a value used to match up request and response pairs. */
typedef ULONG RequestId;

// Type Definitions for use in CThreadIt and associated classes.
/** WorkPackItQ is the protocted queue by a critical section. */
typedef CProtectedQueue <CWorkPackIt> WorkPackItQ;

/** ItemQ is the a queued protected by a critical section. This queue
 * is used to transfer generic items around. */
typedef CProtectedQueue <void> ItemQ;

/** DataItemPtrQ is a queue of DataItemPtrs. This allows shared_ptr types to be
  * passed around and they can be type cast into the required shared_ptr types.
	*/
typedef CMtQueue <DataItemPtr> DataItemPtrQ;

/** ThreadItPtr is a shared pointer to a CThreadIt instance. */
typedef std::shared_ptr <CThreadIt> ThreadItPtr;

/** ThreadItWeakPtr is a weak pointer to a CThreadIt instance. */
typedef std::weak_ptr <CThreadIt> ThreadItWeakPtr;

/** WorkerMethodType is a pointer to a member function of the CThreadIt class
 * that accepts a CWorkPackIt parameter and returns a CWorkPackIt parameter.
 * The member function returns a bool value.
 * Note that the first parameter receives a pointer to that now belongs to the
 * method implementation and should be freed by the method implementation.
 * The second parameter returns a heap allocated pointer and on exit from the
 * method ownership passes to the recipient that recieves from the queue. The
 * recipient is then responsible for freeing the memory reference by the
 * parameter. */
typedef bool (CThreadIt::*WorkerMethodType)(CWorkPackIt*, CWorkPackIt*&);

/** PeriodicMethodType is a pointer to a member function of the CThreadIt class
 * that returns a CWorkPackIt and CWorkPackIt parameter. The CWorkPackIt parameter
 * simulates the work request that the PeriodicMethodType is to perform.
 * The member function returns a bool value that indicates if the CWorkPackIt results
 * should be used to simulate the result generation of CWorkPackIt and that the client
 * should be notified via the WorkDone queue. This would have to be setup in the
 * CWorkPackIt parameter in the supplied PeriodicMethodType.
 * The first parameter cannot be changed and provides the ability for timing.
 * The second parameter returns a heap allocated pointer and on exit from the
 * method ownership passes to the recipient that recieves from the queue. The
 * recipient is then responsible for freeing the memory reference by the
 * parameter.*/
typedef bool (CThreadIt::*PeriodicMethodType)(CWorkPackIt ,CWorkPackIt*&);

/** EventMethodType is a pointer to a member function of the CThreadIt class
 * that returns a CWorkPackIt and CWorkPackIt parameter. The CWorkPackIt parameter
 * simulates the work request that the EventMethodType is to perform.
 * The event method is assocaited with a wait event and when the event takes
 * place the associated method is invoked.
 * The first parameter cannot be changed and provides the ability for timing.
 * The second parameter returns a heap allocated pointer and on exit from the
 * method ownership passes to the recipient that recieves from the queue. The
 * recipient is then responsible for freeing the memory reference by the
 * parameter.*/
typedef bool (CThreadIt::*EventMethodType)(CWorkPackIt, CWorkPackIt*&);

/**
 * Class CWorkPackIt provides the ability to specify work to be done.
 */
class	 CWorkPackIt
{
	// Attributes
public:
	/** m_Instruction is the instruction of work to perform. The value starts
	 * from one onwards. A value of zero implies no work to be performed.
	 * The instruction is assigned to a method that performs work that
	 * corresonds to the instruction. This correspondence is implemented
	 * in the CThreadIt::SetWorkerMethod. The Instruction parameter
	 * corresponds with this attribute and thus invokes the specified method
	 * to do work. This value ranges from 1 to MAX_WORK_METHODS. */
	ULONG m_theInstruction;
	/** m_WorkPackID is a value assigned to the work packet object once it is
	 * placed in the queue of work to be done. The originator of the
	 * WorkPackIt can use this number to track the progress. */
	ULONG m_theWorkPackID;
	/** m_isNotifyWithCallback is set to true if notification of work completion
	 * is required. If m_NotifyWindow is true, then the m_pWnd value and the
	 * m_WM_USER value will be used to send a PostMessage. The PostMessage
	 * will have an wParam equal to the m_Instruction and a lParam equal to
	 * the m_WorkPackID value. */
	bool	m_isNotifyWithCallback;
	/** m_SendResult is set to true if a CWorkPackIt object is to be returned
	 * in the work done queue after the work is completed. A thread waiting
	 * on this queue will be signalled when the CWorkPackIt object is placed
	 * in the queue. */
	bool m_isSendResult;
	/** m_UseDefaultQ is set to true if the in-built work done queue is used
	 *	to return the Work Done results. This value is set to TRUE by default. */
	bool m_isUseDefaultQ;
	/** m_pWorkDoneQ receives work done packages for return to the initiator
	 * of the work request if the value of this member is not NULL and
	 * m_UseDefaultQ is false. */
	CProtectedQueue <CWorkPackIt>* m_ptheWorkDoneQ;
	// 2009-05-03 - Addition of shared_ptr for shared queue.
    /** m_UseSharedQ is set to true if the in-built work done queue is used
     *  to return the output of work methods. This value is set to false by default. */
    bool m_isUseSharedQ;
    /** m_pSharedQ receives work items for return to the initiator
     * of the work request if the value of this member is not NULL and
     * m_isUseSharedQ is true. The queue can be used to transfer any type of element
		 * or item */
		std::shared_ptr<ItemQ> m_pSharedQ;
		// 2009-05-03 - End of addition of shared_ptr for shared queue.
		// 2010-05-31 - Addition of the real shared queue.
		/** m_isUseSharedPtrQ is set to true to use the m_ptheSharedPtrQ. */
    bool m_isUseSharedPtrQ;
		/** m_ptheSharedPtrQ is a queue of DataItemPtr that can be used to
		 * transfer around DataItem types. These can then type cast to
		 * subclass types that inherit from DataItem. */
		std::shared_ptr<DataItemPtrQ> m_ptheSharedPtrQ;
		// 2010-05-31 - End of the addition of the real shared queue.
    /** m_pObject is a general pointer for passing information to the
     * method that will perform the work. It is user defined. */
    void* m_ptheObject;
		/** m_isObjectInCallback indicates if a shared_ptr reference to m_ptheObject is
		 * returned as part of the callback. If this value is true then a reference to
		 * m_ptheObject can be obtained from within the callback. The default is false. */
		bool m_isObjectInCallback;
		/** m_ptheDataItem holds a shared pointer based data item. */
		DataItemPtr m_ptheDataItem;
		/** m_ptheSource is a reference to the instance from which this CWorkPackIt has
		 * been sent. It is null for incoming CWorkPackIt instances. Note that the reference
	   * may not be valid if the originating instance has been destroyed since the CWorkPackIt
	   * has been sent. If this is a worry use the m_sptheSource version below. */
		CThreadIt* m_ptheSource;
		/** m_sptheSource is the weak pointer version reference to the instance from which this CWorkPackIt has
		 * been sent. Once there you can obtain a shared pointer from m_wptheSource and use that defensively. If you hold on to this
		 * shared reference then you can prolong the life of other CThreadIt more than you should or could be intended.
		 */
		std::weak_ptr<CThreadIt> m_wptheSource;
		/** m_theReplyInstructionId is set if m_ptheSource is set and specifies the return work
		 * instruction the receiver of this work packit can use to reply to the work request. */
		UINT m_theReplyInstructionId;
    /** m_TimeAllowed indicates the time allowed to perform the work package.
     * If the work package cannot be done in this time period, then the
     * work method must stop execution as soon as it recognises that the
     * time allowed period expired and return the status WORKDONE_TIME_OUT
     * as part of the CWorkPackIt response. A time value of zero implies
     * no time restriction on the processing. The value is in milliseconds and
     * the value selected should be based on a resolution of approx 10
     * milliseconds. */
    ULONG m_theTimeAllowed;
    /** m_TimeElapsed is the time take for the work to complete. It is monitored
     * automatically. The value returned is in milliseconds. */
    ULONG m_theTimeElapsed;
    /** m_Status returns the operation status of the work performed.  */
    ULONG m_theStatus;

	// Services
public:
	/**
	 * Method CWorkPackIt is the constructor for the class. The method sets the
	 * initial values for the member variables.
	 */
	CWorkPackIt ();

	/**
	 * Method Initialise clears the work packet information.
	 */
	int initialise ();

	/**
	 * Method cWorkPacktIt is the copy constructor for the class. The new
	 * instance is initialised with the same values as the supplied instance.
	 * theWorkpack is the object that supplies the initial values for the new instance.
	 */
	CWorkPackIt (const CWorkPackIt& theWorkPack);

	/**
	 * Method ~CWorkPackIt is the destructor for the class. The method releases
	 * resources used by the instance.
	 */
	virtual ~CWorkPackIt ();

	/**
	 * Method operator = assigns the values of a cWorkPacktIt object to
	 * another instance.
	 * theWorkpack is the object that supplies the values that are assigned.
	 */
	CWorkPackIt &operator=(CWorkPackIt &theWorkPack);

	/**
	 * Method WorkInstruction returns the work instruction associated with the
	 * CWorkPackIt instance. A work instruction value of zero is not a valid
	 * work instruction.
	 */
	ULONG getWorkInstruction ();

	/**
	 * Method getSource returns a reference to the originating CThreadIt instance
	 * from which this CWorkPackIt was sent. Note: The returned value, if not NULL, may
	 * not refer to an existing CThreadIt instance if the instance has been destroyed
	 * since the CWorkPackIt instance has been sent.
	 */
	CThreadIt* getSource ();

	/**
	 * Method setReplyInstruction is called to set the instruction that the recipient
	 * of this message can use to reply to the message. This value can be set if the
	 * value of the message source is specified and available using the method
	 * getSource.
	 */
	void setReplyInstruction (UINT theInstruction);

	/**
	 * Method getReplyInstruction is called to get the instruction that the recipient
	 * of this message can use to reply to the message. This value is normally set if the
	 * value of the message source is specified and available using the method
	 * getSource and the sender intended the recipient to reply.
	 */
	UINT getReplyInstruction ();

}; // class CWorkPackIt

/**
 * Class CThreadIt implements the protocol for peforming work in a separate
 * thread when work package requests are received from clients.
 */
class CThreadIt : public CActive, public std::enable_shared_from_this<CThreadIt>
{
public:
	// ThreadIt: Constants
	/** MAX_WORK_METHODS is the maximum number of work methods that an instance
	 *	can provide. Work instructions are thus limited to the range 1 to
	 *	MAX_WORK_METHODS by implementation. */
	static const UINT MAX_WORK_METHODS = 50;
	/** MAX_EVENT_METHODS is the maximum number of event methods that an instance can provide. */
	static const UINT MAX_EVENT_METHODS = 10;
	/** THREADIT_PERIOD_TIMER is returned as the m_WorkInstruction value
	 *	in a WorkDoneIt object when the periodic method executes. */
	static const UINT THREADIT_PERIOD_TIMER = 0;

private:
	/** MODULE_NAME Name allocated to this module. This is used for logging and component
	  * identification purpose */
	static const std::string MODULE_NAME;

public:
	/** COPY_PARAMS is used as input to the checkParams methods used to assist applications
	 * in checking the input parameters to a ThreadIt worker method. This indicates that the
	 * input CWorkPackIt parameters should be copied to the output CWorkPackIt parameters.
	 * This is the default behaviour.
	 */
	static const bool COPY_PARAMS = true;

	// ThreadIt: WorkDoneIt Status Constants
	enum StatusIds
	{
		/** THREADIT_STATUS_START indicates values that apply to the status parameter
		 *	of the CWorkPackIt class. Values below this are user defined. */
		THREADIT_STATUS_START = 150,
		// The following constants define status outcomes that are CThreadIt
		// specific and indicate a problem with work execution.
		// No work has been done yet.
		WORKDONE_NO_RESULT = 151,
		/** The WorkPackage instruction was not valid. */
		WORKDONE_INVALID_INSTRUCTION = 152,
		/** There is no mehtod allocated to the given work instruction.	 */
		WORKDONE_NO_METHOD = 153,
		/** There was a time-out waiting for the work to complete.	*/
		WORKDONE_TIME_OUT = 154,
		/** A result needs to be returned using a local queue specified in the
		 * CWorkPackIt class, but none was specified correctly. */
		WORKDONE_INVALID_DONE_QUEUE = 155,
		/** An event has occurred for which the event is out of range.	*/
		WORKDONE_INVALID_EVENT = 156,
		/** An event has occurred for which there is no handler.	*/
		WORKDONE_NO_EVENT_METHOD = 157,
		/** A general successful status report. */
		THREADIT_STATUS_OK,
		THREADIT_STATUS_PARAM_OBJECT_NULL,
	  THREADIT_STATUS_PARAM_WORK_PACK_NULL,
		THREADIT_STATUS_LAST // Last kid off the block - used for looping.
	}; // enum StatusIds

	// types
protected:

	// The following type definition defines the event method and the associcated waitable object.
	typedef struct EventInfoTag
	{
		// hEvent is the waitable object.
		HANDLE htheEvent;
		// EventHandler is the method that is invoked when the event associated with
		// the waitable object occurs.
		EventMethodType theEventHandler;
	} EventInfo;

	// attributes
protected:
	/** m_Access is a critical section for the global data of the instance. */
	HANDLE m_Access;
	/** m_TimeAccess is a critical section for the global timing data of the instance.*/
	HANDLE m_TimeAccess;
	/** m_WorkPackID is a running number used to uniquely identify work
	 * packages in the system. It should just reset itself when incremented
	 * beyond its limit. This allows the system to handle work packages of
	 * the same work instruction. */
	ULONG m_WorkPackID;
	/** m_isExitThread is set to true when the thread of execution must exit. */
	volatile bool m_isExitThread;
	/** m_WorkQueue represents the queue that receives work packages to execute
	 *	that is then processed by the thread. */
	CProtectedQueue <CWorkPackIt> m_WorkQ;
	/** m_DoneQ receives work done packages for return to the initiators of work. */
	CProtectedQueue <CWorkPackIt> m_DoneQ;
	/** m_WorkerMethod is the array of member functions of the WorkerMethodType
	 * signature that are called to process work packages. These member
	 * functions are declared in a derived class. */
	WorkerMethodType	m_WorkerMethod[MAX_WORK_METHODS + 1];
	/** m_PeriodMethod is the function that is called to peform processing
	 * when a time period expires. */
	PeriodicMethodType m_PeriodicMethod;
	/** m_EventMethod is the array of member functions and associated waitable objects. */
	EventInfo m_EventMethod[MAX_EVENT_METHODS];
	/** m_theEventMethodCount is the running count of the number of event method handlers
	 * installed. */
	volatile UINT m_theEventMethodCount;
	/** m_ResetEventInfo is set to true when the event associations has been
	 * modified. This implies that it must be setup on the next wait
	 * function invocation. */
	volatile bool m_ResetEventInfo;
	/** m_theCallback is the instance used for managing callbacks to interested clients */
	CThreadItCallback m_theCallback;
	// Exectution Timing variables.
	/** m_TStart measures the start of a timing operation. */
	DWORD m_TStart;
	/** m_TStop measures the end of a timing operation. */
	DWORD m_TStop;
	/** m_IsTiming indicates if execution timing is in progress. */
	bool m_IsTiming;
	/** m_TimeAllowed is the time allowed for work execution for the current operation. */
	DWORD m_TimeAllowed;
	/** m_TimePeriod is the time period at which periodic work is expected to occur.*/
	DWORD m_TimePeriod;
	/** m_TimeOut is used while delaying for an event. */
	DWORD m_TimeOut;
	/** m_Period is the timer for determining when the periodic method should execute.*/
	CTimeIt m_Period;
	/** m_ptheLogger is the logger used to log information and errors for each instance of
	 * this class */
	log4cpp::Category* m_ptheLogger;

	// Methods
public:
	/**
	 * Method CThreadIt is the constructor for the class. The method sets the
	 * initial values for the member variables, sets the thread to be executed
	 * at default priority and starts thread execution.
	 */
	CThreadIt ();

	/**
	 * Method CThreadIt is the constructor for the class. The method sets the
	 * initial values for the member variables, sets the priority of the thread
	 * and then starts thread execution.
	 * Priority specifies the priority of the thread of execution associated with this instance.
	 */
	CThreadIt (int Priority);

	/**
	 * Method CThreadIt is the constructor for the class. The method sets the
	 * initial values for the member variables, sets the name associated with
	 * the thread and then sets the priority of the thread to the default priority
	 * and then starts thread execution.
	 * theThreadName is the name allocated to the this thread instance.
	 */
	CThreadIt (const std::string& theThreadName);

	/**
	 * Method CThreadIt is the constructor for the class. The method sets the
	 * initial values for the member variables, sets the name associated with
	 * the thread and then sets the priority of the thread
	 * and then starts thread execution.
	 * thePriority specifies the priority of the thread of execution associated with this instance.
	 * theThreadName is the name allocated to the this thread instance.
	 */
	CThreadIt (const std::string& theThreadName, int thePriority);

	/**
	 * Method ~CThreadIt is the destructor for the class. The method waits until
	 * the thread of execution stops and then releases the resources used by the
	 * instance.
	 */
	virtual ~CThreadIt ();

	// Client Interface Methods

	/**
	 * Method addObserver adds an observer that can receive asynchronous notifications of
	 * subject changes. The notification or update is performed via the subject in the
	 * the worker, period or event method implementation.
	 * ptheObserver is a reference to the instance that wants to be informed of subject
	 * changes or updates.
	 */
	bool addObserver (CObserver* ptheObserver);

	/**
	 * Method removeObserver removes the specified observer from the list of observers.
	 * ptheObserver is a reference to the instance that no longer wants to be informed
	 * of subject changes or updates.
	 */
	bool removeObserver (CObserver* ptheObserver);

	/**
	 * Method clearObservers removes all observers from being notified of changes or updates
	 * from the worker, period or event method implementations.
	 */
	void clearObservers ();

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
	 * Method StartWork returns true if the work package is placed in the work
	 * queue successfully.
	 */
	bool startWork (CWorkPackIt*& pWorkPack, ULONG& WorkPackID);

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
	CWorkPackIt* getWork (UINT TimeOut);

	/**
	 * Method getWorkDoneQ returns a pointer to the work done queue. This is
	 * provided to allow custom handling of the queue.
	 * pWorkDoneQ is the pointer to the work done queue.
	 */
	void getWorkDoneQ (CProtectedQueue <CWorkPackIt>* pDoneQ);

	/**
	 * Method GetWorkQ returns a pointer to the work pending queue. This is
	 * provided to allow custom handling of the queue.
	 * pWorkQ is the pointer to the work pending queue.
	 */
	void getWorkQ (CProtectedQueue <CWorkPackIt>* pWorkQ);

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
	bool setWorkerMethod (WorkerMethodType Method, UINT Instruction);

	/**
	 * Method SetPeriodicMethod associates member functions of a derived class
	 * with the periodic method. This implies that when a time period
	 * elapses, the given periodic method is executed. The periodic methods
	 * is able to return a CWorkPackIt instance in the work done queue.
	 * Method is the method that will be required to perform periodic work processing.
	 * Method SetPeriodoicMethod returns true if the member function is setup
	 * successfully.
	 */
	bool setPeriodicMethod (PeriodicMethodType Method);

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
	bool setEventMethod (ULONG& theEventId, EventMethodType EventHandler, HANDLE hEvent);

	/**
	 * Method GetEventMethod returns the associated member functions of a
	 * derived class with events.
	 * EventId is the value returned as the event id when the setEventMethod was called.
	 * EventHandler is the method that will be invoked in response to the event.
	 * hEvent is the handle to the waitable object that is associated with the event.
	 * Method GetEventMethod returns true if the event and handler information
	 * is returned correctly.
	 */
	bool getEventMethod (ULONG EventId, EventMethodType& EventHandler, HANDLE& hEvent);

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
	bool isAvailableTime (DWORD& Elapsed);

	/**
	 * Method StopThread stops the execution of the thread of control for the instance.
	 */
	void stopThread ();

	/**
	 * Method SetPeriod sets the period time at which the periodic method is invoked.
	 * Period is the timer period in milliseconds that sets the time period between
	 * Periodic Method invocations is invoked. A value of zero turns off period timing.
	 * This method should only be called from Worker Methods or from the Periodic method itself.
	 */
	void setPeriod (DWORD Period);

  /** return the period of the periodic method */
  DWORD getPeriod() const {return m_TimePeriod;}

	/**
	 * Method WaitForThreadToStop can be called to wait for the thread to stop execution.
	 */
	virtual void waitForThreadToStop ();

	/**
	 * Method getSelfThreadItPtr returns a shared pointer to this instance - that is to itself.
	 * This is useful when telling other instances to reply to to me in response to a message.
	 * sent to them.
	 * Return a shared pointer to myself. This pointer is obtained using shared_from_this ().
	 */
	 ThreadItPtr getSelfThreadItPtr ();

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
	bool checkParams (CWorkPackIt* ptheWorkPack, CWorkPackIt*& ptheWorkDone, void*& ptheObj, long& theStatus, bool isCopy = !COPY_PARAMS) const;

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
	bool checkParams (CWorkPackIt* ptheWorkPack, CWorkPackIt*& ptheWorkDone, long& theStatus, bool isCopy = !COPY_PARAMS) const;

	/**
	 * Method logParameterError is called to log the error string in the log as an error
	 * that represents theStatus. These are the enum StatusIds values.
	 * @param[in] theStatus is the status value to be logged to the log file.
	 */
	void logParameterError (long theStatus) const;

	/**
	 * Method toParameterErrorStr is called to return the error string
	 * that represents theStatus value. These are the enum StatusIds values.
	 * @param[in] theStatus is the status value to be returned as string.
	 * \return the string representation of theStatus.
	 */
	std::string toParameterErrorStr (long theStatus) const;

// end of client interface methods
protected:
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
	bool sendResponse (CWorkPackIt*& WorkDone, ULONG WorkId, bool isPeriodic);

	/**
	 * Method StartThread resumes the execution of the thread of control for the instance.
	 */
	virtual void startThread ();

	/**
	 * Method CThreadItInit performs the shared initialisation code for the class.
	 * theThreadName is used to determine the name of the thread with a component
	 * prefix as an option. In otherwords a user defined hierarchy can be implemented
	 * with .CThreadIt attached to the end. Used for selective logging and component
	 * identification purposes.
	 */
	void threadItInit (const std::string& theThreadName);

	/**
	 * Method ThreadRoutine represents the thread of execution for the instance.
	 * On start, the thread waits for work packages in the work queue. When
	 * a package is found the associated worker thread is invoked to perform
	 * the work. On completion, the client is notified of work completion and
	 * the process repeats.
	 */
	virtual void threadRoutine ();

	/**
	 * Method isExitThread is called internally to check if the thread of
	 * execution is required to stop.
	 */
	bool isExitThread ();

	/**
	 * Method StartTiming is called to record the start of work execution timing.
	 * TimeAllowed specifies the time allocated for work to be executed.
	 * A value of zero indicates that there is no timing restriction.
	 */
	void startTiming (DWORD TimeAllowed);

	/**
	 * Method StopTiming is called to record the end of work execution timing.
	 * Elapsed returns the time elapsed so far for work execution in milliseconds.
	 */
	void stopTiming (DWORD& Elapsed);

	/**
	 * Method TimeElapsed calculates how much time has so far elapsed since timing
	 * was started.
	 * Elapsed returns the time elapsed since timing was started.
	 * Method TimeTake returns true if timing is currently in progress.
	 */
	bool timeElapsed (DWORD& Elapsed);

}; // class ThreadIt

#endif // !defined (THREADIT_H)
