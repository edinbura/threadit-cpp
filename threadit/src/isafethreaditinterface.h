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
 * Title: CISafeThreadItInterface
 * Description: Class CISafeThreadItInterface is a base class that provides
 * support for defining an interface to a CThreadIt derived instance.
 * The CISafeThreadItInterface provides the minimum services to send
 * messages to a CThreadIt derived instance and also to manage the
 * partner CThreadIt instances that form part of the message transfer.
 * This utilises the ability of CThreadIt to support a reply response
 * to the sender of the work request.
 * The concept embodied in this class is to provide an interface
 * specification between communicating CThreadIt type instances.
 * The interface is lightweight in that it only has a depdendency
 * on CThreadIt and it can be easily used within CThreadIt derived
 * classes without requiring additional depedencies. The use of such
 * an interface supports using various CThreadIt implementations
 * even at at run-time. 
 *
 * The CISafeThreadItInterface is provided to help ensure lifetime
 * consistency between CThreadIt instances. Two cases are addressed:
 * 1) Having supplied a reference to the worker CThreadIt to an instance of 
 * the CISafeThreadItInterface, the worker could be released. By using a 
 * shared pointer pointer approach and providing a weak reference this 
 * instance can determine if the reference to the work is valid before 
 * using it to send the worker a message.
 * 2) Having supplied the sender reference, this must be a shared pointer
 * until it gets to the reciever CTreadIt that will use it as part of 
 * the reply process. The sender then uses an instance of CISafeThreadItInterface
 * and as such then uses a weak reference to the original sender. 
 * The lifecycles of the various CThreadIts are not compromised as long as the
 * messages containing shared_ptrs are processed timeously.
 *
 * CThreadIt has been modified to support this approach as follows:
 * CWorkPackIt : now suppports the m_spSender which is a shared pointer to a
 * CThreadIt as required for (2) above.
 * CThreadIt can return a shared pointer to itself using  getSelfThreadItPtr. 
 * This allows us to get a shared pointer reference to use for the sender
 * field in this interface. 
 * 
 * CISafeThreadItInterface will be useful in a fully shared pointer based 
 * environment. 
 *
 * Copyright: Copyright (c) 2010 Ashkel Software
 * @author Ari Edinburg
 * @version 1.0
 * @Revision: 1.2 
 * @Date: 2010/06/02
 */

#pragma once
#if !defined (I_SAFE_THREADIT_INTERFACE_H)
#define I_SAFE_THREADIT_INTERFACE_H

// Includes
#include "threadit.h"

/**
 * Class CISafeThreadItInterface provides support for specifying the interface
 * used to communicate with a CThreadIt based instance and addresses the lifecycle
 * issues with CThreadIt instances. 
 */
class CISafeThreadItInterface
{
  // Attributes
  private:
		/** m_ptheWorker is the CThreadIt instance that is requested to do work on
		 * behalf of the client. */
		std::weak_ptr<CThreadIt> m_ptheWorker;
		/** m_ptheSender is the CThreadIt instance that is sending the request
		 * to do work. This is the client. */
    std::weak_ptr<CThreadIt> m_ptheSender;

  // Methods
  public:
		/**
		 * Constructor CISafeThreadItInterface ensures the member variables are initialised.
		 */
    CISafeThreadItInterface ();

		/**
		 * Constructor CISafeThreadItInterface ensures the member variables are initialised.
		 * @param[in] ptheWorker is a pointer to the CThreadIt instance that will be asked to
		 * do work. This is held as a weak pointer internally.
		 */
    CISafeThreadItInterface (const std::weak_ptr<CThreadIt>& ptheWorker);

		/**
		 * Constructor CISafeThreadItInterface ensures the member variables are initialised.
		 * @param[in] ptheWorker is a pointer to the CThreadIt instance that will be asked to
		 * do work.  This is held as a weak pointer internally.
		 * @param[in] ptheSender is a pointer to the CThreadIt instance that is asking for work
		 * to be done on it's behalf. If this reference is available it can be used
		 * for the worker to send a reply work request. It must be a weak_ptr until the
		 * reciever obtains the reference whereupon it can be used in an instance of
		 * CISafeThreadItInterface based class as a weak reference. 
		 */
    CISafeThreadItInterface (const std::weak_ptr<CThreadIt>& ptheWorker, const std::weak_ptr<CThreadIt>& ptheSender);

  	/**
	   * Method ~CISafeThreadItInterface is the destructor for the instance.
	   */
    virtual ~CISafeThreadItInterface ();

		/**
		 * Method setWorker is called to set the work CThreadIt instance that is asked
		 * to perform work.
		 * @param[in] ptheWorker is a pointer to the CThreadIt instance that will be asked to
		 * do work.  This is held as a weak pointer internally.
		 */
    void setWorker (const std::weak_ptr<CThreadIt>& ptheWorker);

		/**
		 * Method getWorker is called to return the work CThreadIt instance that is asked
		 * to perform work on behalf of this instance.
		 * @param[out] ptheWorker is a pointer to the CThreadIt instance that will be asked to
		 * do work. It is derived from a weak pointer and such should be used and 
		 * released.
		 */
    void getWorker (std::weak_ptr<CThreadIt>& ptheWorker) const;

		/**
		 * Method setSender is called to set the work CThreadIt instance that is asking
		 * the worker to perform work on the sender's behalf.
		 * @param[in] ptheSender is a pointer to the CThreadIt instance that is asking for work
		 * to be done on it's behalf. If this reference is available it can be used
		 * for the worker to send a reply work request. It must be a weak_ptr until the
		 * reciever obtains the reference whereupon it can be used in an instance of
		 * CISafeThreadItInterface based class as a weak reference. 
		 */
    void setSender (const std::weak_ptr<CThreadIt>& ptheSender);

		/**
		 * Method clearSender is called to free the referenece to work CThreadIt instance that is asking
		 * the worker to perform work on the sender's behalf. It allows us to minimise the impact on
		 * the lifetime of the sender CThreadIt instance. 
		 */
		void clearSender ();

		/**
		 * Method setPartners is called to set the communicating partners in a
		 * work request and response interaction.
		 * @param[in] ptheWorker is a pointer to the CThreadIt instance that will be asked to
		 * do work. It is held a weak pointer internally.
		 * @param[in] ptheSender is a pointer to the CThreadIt instance that is asking for work
		 * to be done on it's behalf. If this reference is available it can be used
		 * for the worker to send a reply work request. It must be a weak_ptr until the
		 * reciever obtains the reference whereupon it can be used in an instance of
		 * CISafeThreadItInterface based class as a weak reference. 
		 */
    void setPartners (const std::weak_ptr<CThreadIt>& ptheWorker, const std::weak_ptr<CThreadIt>& ptheSender);

		/**
		 * Method sendMessage is called to send a work request with work payload
		 * to the worker instance.
		 * @param[in] theServiceId is the identity of the work to be done. This is used to
		 * identify the method at the receiving worker of which method will be used
		 * to perform the work request.
		 * @param[in] ptheMessage is the payload or the container of information that
		 * defines the work to be done. The work request is sent with no reply
		 * or notification options set.
		 * Note: the sender is set as part of the work message if it has previously
		 * specified.
		 */
    bool sendMessage (UINT theServiceId, void* ptheMessage);

		/**
		 * Method sendMessage is called to send a work request with work payload
		 * to the worker instance.
		 * @param[in] theServiceId is the identity of the work to be done. This is used to
		 * identify the method at the receiving worker of which method will be used
		 * to perform the work request.
		 * @param[in] theDataItem is the weak_ptr based payload or the container of information that
		 * defines the work to be done. The work request is sent with no reply
		 * or notification options set. 
		 * Note: the sender is set as part of the work message if it has previously
		 * specified.
		 */
    bool sendMessage (UINT theServiceId, const DataItemPtr& theDataItem);

	protected:
		/**
		 * Method getSafeWorker returns a shared pointer to the CThreadIt worker associated with this 
		 * instance. You should use this pointer if it is valid and release it as soon as 
		 * possible. It should not be held onto. 
		 * \return shared pointer to the CThreadIt worker associated with this instance.
		 */ 
		std::shared_ptr<CThreadIt> getSafeWorker ();

		/**
		 * Method getSafeSender returns a shared pointer to the CThreadIt sender associated with this 
		 * instance. You should use this pointer if it is valid and release it as soon as 
		 * possible. It should not be held onto. 
		 * \return shared pointer to the CThreadIt sender associated with this instance.
		 */ 
		std::shared_ptr<CThreadIt> getSafeSender ();

		/**
		 * Method getSender returns a weak pointer to the CThreadIt sender associated with this 
		 * instance. You should use this pointer if it is valid and release it as soon as 
		 * possible. It should not be held onto. 
		 * \return weak pointer to the CThreadIt sender associated with this instance.
		 */ 
		std::weak_ptr<CThreadIt> getSender ();

}; // class CISafeThreadItInterface

#endif // !defined (I_SAFE_THREADIT_INTERFACE_H)
