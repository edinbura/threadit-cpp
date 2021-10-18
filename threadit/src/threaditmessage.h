/**
 * Title: CThreadItMessage
 * Description: Class ThreadItMessage is the base class used to define messages that
 * can be sent onto work performers using the WorkPackIt and ThreadIt work
 * model. The messages define the work to be peformed and need to be
 * extended for use use in the particular application.
 * Comment: The class needs additional work to provide a command pattern.
 *
 * Copyright: Copyright (c) 2008 Ashkel Software 
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2006/12/19 02:38:06 $
 */

#pragma once
#if !defined (THREADIT_MESSAGE_H)
#define THREADIT_MESSAGE_H

// Includes
#include "threadit.h"
#include "threaditcallback.h"

/**
 * Class ThreadItMessage is the base class of messages sent to work performers indicating 
 * the task to be performed with all the information needed to perform the task and the way 
 * in which the results are to be returned when the task is completed.
 */
class CThreadItMessage
{
protected:
	/**
	 * _theInstruction is the work instruction that this message
	 * represents.
	 */
	long _theInstruction;
	/**
	 * _theCallback is the instance used to effect the callback to the
	 * client when the work is completed.
	 */
	CThreadItCallback* _theCallback;
	/**
	 * _aNotification indicates if the client requires notification of
	 * the task completion.
	 */
	bool _aNotification;
	/**
	 * _theWorkPack is the WorkPackIt that specifies the task to be done.
	 */
	CWorkPackIt*	_theWorkPack;
	/**
	 * m_ptheSource specifies the source of the message if it is of the CThreadIt type.
	 */
	CThreadIt* m_ptheSource;
	/**
	 * m_ptheSource specifies the source of the message if it is of the CThreadIt type.
	 */
	std ::weak_ptr<CThreadIt> m_wptheSource;
	/**
	 * m_theReplyInstruction specifies the instruction to use when replying to this 
	 * m_ptheSource.
	 */
	UINT m_theReplyInstruction;

public:		
	/**
	 * Method ThreadItMessage is the constructor for the class. The method
	 * associates the work instruction with the message.<p>
	 * theInstruction : is the identity of the work to be performed when this
	 *									message is received.
	 */
	CThreadItMessage (UINT theInstruction)
	{
		initialise ();
		// Initialise the parameters to the default values.
		_theInstruction = theInstruction;
		_theCallback	 =	NULL;
		_aNotification = false;
		m_ptheSource = NULL;
		m_theReplyInstruction = 0;
	} // ThreadItMessage

		/**
		 * Method ThreadItMessage is the constructor for the class. The method
		 * associates the work instruction with the message and specifies the
		 * way in which the client is informed that the work has been completed.<p>
		 * theInstruction : is the identity of the work to be performed when this
		 *									message is received.<p>
		 * theWorkDoneCallback : is the instance to use to inform the client that the
		 *											 work has been completed.
		 */
	CThreadItMessage (UINT theInstruction, CThreadItCallback* theWorkDoneCallback)
	{
		initialise ();
		// Initialise the parameters to the default values.
		_theInstruction = theInstruction;
		_theCallback = theWorkDoneCallback;
		_aNotification = (_theCallback != NULL);
		m_ptheSource = NULL;
		m_theReplyInstruction = 0;
	} // ThreadItMessage

    CThreadItMessage (CThreadIt* ptheSource)
		{
			initialise ();
			// Initialise the parameters to the default values.
			_theInstruction = 0;
			_theCallback	 =	NULL;
			_aNotification = false;
			m_ptheSource = ptheSource;
			m_theReplyInstruction = 0;
		} // ThreadItMessage

		~CThreadItMessage ()
		{
		} // ~CThreadItMessage
		
	private:
	/**
	 * Method initialise is called to perform general initialisation for the
	 * instance and is used by the class constructors.
	 */
	void initialise ()
	{
		_theInstruction = 0;
		_theCallback =	NULL;
		_aNotification = false;
		m_ptheSource = NULL;
		m_theReplyInstruction = 0;
		_theWorkPack = new CWorkPackIt ();
	} // initialise

public:
	/**
	 * Method sendTo is called to send the message onto the work performer
	 * which is the destination of the message. The message destination
	 * must implement the startWork method.<p>
	 * theMessageDestination : is the instance that will perform the work
	 *												 in response to this message.
	 * NOTE: This message will be setup to return a reply.
	 */
	ULONG sendTo (CThreadIt* theMessageDestination)
	{
		bool aSuccess = false;
		ULONG aPackId = 0;

		// Send a message to start the work.
		_theWorkPack->m_theInstruction = _theInstruction;
		// The work package object is not changed in the event that its value
		// is set.
		//_theWorkPack.m_Object				= null;
		_theWorkPack->m_isNotifyWithCallback = _aNotification;
	  _theWorkPack->m_isSendResult = true;
		_theWorkPack->m_ptheSource = m_ptheSource;
		_theWorkPack->m_theReplyInstructionId = m_theReplyInstruction;
		_theWorkPack->m_wptheSource = m_wptheSource;
		//_theWorkPack->->m_theCallback		= _theCallback;
		// Start doing the work.
		if (theMessageDestination != NULL)
		{
			aSuccess = theMessageDestination->startWork (_theWorkPack, aPackId);
		} // if
			// Return the method status.
		return aPackId;
	} // sendTo

	// 	NOTE: This message will be setup not to return a reply.
	ULONG sendWithNoReplyTo (CThreadIt* theMessageDestination)
	{
		bool aSuccess = false;
		ULONG aPackId = 0;

		// Send a message to start the work.
		_theWorkPack->m_theInstruction	= _theInstruction;
		_theWorkPack->m_isSendResult	 = false;
		// The work package object is not changed in the event that its value is set.
		//_theWorkPack.m_Object				= null;
		_theWorkPack->m_isNotifyWithCallback = false;
		_theWorkPack->m_ptheSource = m_ptheSource;
		_theWorkPack->m_wptheSource = m_wptheSource;
		_theWorkPack->m_theReplyInstructionId = m_theReplyInstruction;
		//_theWorkPack.m_Callback = null;
		// Start doing the work.
		if (theMessageDestination != NULL)
		{
			aSuccess = theMessageDestination->startWork (_theWorkPack, aPackId);
		} // if
			// Return the method status.
		return aPackId;
	} // sendTo



		/**
		 * Method sendTo is called to send the message onto the work performer
		 * which is the destination of the message. The message destination
		 * must implement the startWork method. This method allows the
		 * work instruction to be specified when sending the message.<p>
		 * theMessageDestination : is the instance that will perform the work
		 *												 in response to this message.<p>
		 * theInstruction : is the work instruction to be assocated with
		 *									this message.
		 */
	ULONG sendTo (CThreadIt* theMessageDestination, int theInstruction)
	{
		bool aSuccess = false;
		ULONG aPackId = 0;
		
		// Send a message to start the work.
		_theInstruction = theInstruction;
		_theWorkPack->m_theInstruction	= theInstruction;
		_theWorkPack->m_isSendResult	 = true;
		// The work package object is not changed in the event that its value is set.
		//_theWorkPack.m_Object				= null;
		_theWorkPack->m_isNotifyWithCallback = _aNotification;
		_theWorkPack->m_ptheSource = m_ptheSource;
		_theWorkPack->m_wptheSource = m_wptheSource;
		_theWorkPack->m_theReplyInstructionId = m_theReplyInstruction;
		//_theWorkPack.m_Callback = _theCallback;
		// Start doing the work.
		if (theMessageDestination != NULL)
		{
			aSuccess = theMessageDestination->startWork (_theWorkPack, aPackId);
		} // if
			// Return the method status.
		return aPackId;
	} // sendTo
		
		/**
		 * Method sendTo is called to send the message onto the work performer
		 * which is the destination of the message. The message destination
		 * must implement the startWork method. This method allows the
		 * work instruction to be specified when sending the message.<p>
		 * theMessageDestination : is the instance that will perform the work
		 *												 in response to this message.<p>
		 * theInstruction : is the work instruction to be assocated with
		 *									this message.
		 */
	ULONG sendWithNoReplyTo (CThreadIt* theMessageDestination, int theInstruction)
	{
		bool aSuccess = false;
		ULONG aPackId = 0;

		// Send a message to start the work.
		_theInstruction = theInstruction;
		_theWorkPack->m_theInstruction	= theInstruction;
		_theWorkPack->m_isSendResult	 = false;
		// The work package object is not changed in the event that its value is set.
		//_theWorkPack.m_Object				= null;
		_theWorkPack->m_isNotifyWithCallback = false;
		_theWorkPack->m_ptheSource = m_ptheSource;
		_theWorkPack->m_wptheSource = m_wptheSource;
		_theWorkPack->m_theReplyInstructionId = m_theReplyInstruction;
		//_theWorkPack.m_Callback = null;
		// Start doing the work.
		if (theMessageDestination != NULL)
		{
			aSuccess = theMessageDestination->startWork (_theWorkPack, aPackId);
		} // if
			// Return the method status.
		return aPackId;
	} // sendTo

		/**
		 * Method getWork returns the WorkPackIt associated with this message.
		 * It is not a copy.
		 */
	CWorkPackIt* getWork ()
	{
		return _theWorkPack;
	} // getWork
		
		/**
		 * Method setTheInstruction is called to set or to modify the work
		 * instruction associated with this message. The invocation will only
		 * affect the next call to sendTo.<p>
		 * theInstruction: is the work instruction to be associated with this message.
		 */
	void setTheInstruction (long theInstruction)
	{
		_theInstruction = theInstruction;
	} // setTheInstruction

		/**
		 * Method setSourceInfo is called to set the source information 
		 * in terms of the originating CThreadIt instance and also the work instruction
		 * that can be used to reply to this message. 
		 * theInstruction: is the work instruction to be associated with this message.
		 */
	void setSourceInfo (CThreadIt* ptheSource, long theReplyInstruction)
	{
		m_ptheSource = ptheSource;
		m_theReplyInstruction = theReplyInstruction;
	} // setTheInstruction

			/**
		 * Method setSourceInfo is called to set the source information 
		 * in terms of the originating CThreadIt instance and also the work instruction
		 * that can be used to reply to this message. 
		 * theInstruction: is the work instruction to be associated with this message.
		 */
	void setSourceInfo (std ::weak_ptr<CThreadIt> ptheSource, long theReplyInstruction)
	{
		m_wptheSource = ptheSource;
		m_theReplyInstruction = theReplyInstruction;
	} // setTheInstruction

}; // class CThreadItMessage

#endif // !defined (THREADIT_MESSAGE_H)
