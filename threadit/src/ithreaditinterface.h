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
 * Title: CIThreadItInterface
 * Description: Class CIThreadItInterface is a base class that provides
 * support for defining an interface to a CThreadIt derived instance.
 * The CIThreadItInterface provides the minimum services to send
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
 * Copyright: Copyright (c) 2009 Ashkel Software
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2009/04/30 21:38:06 $
 */

#pragma once
#if !defined (I_THREADIT_INTERFACE_H)
#define I_THREADIT_INTERFACE_H

// Includes
#include "threadit.h"

/**
 * Class CIThreadItInterface provides support for specifying the interface
 * used to communicate with a CThreadIt based instance.
 */
class CIThreadItInterface
{
  // Attributes
  protected:
		/** m_ptheWorker is the CThreadIt instance that is requested to do work on
		 * behalf of the client. */
    CThreadIt* m_ptheWorker;
		/** m_ptheSender is the CThreadIt instance that is sending the request
		 * to do work. This is the client. */
    CThreadIt* m_ptheSender;

  // Methods
  public:
		/**
		 * Constructor CIThreadItInterface ensures the member variables are initialised.
		 */
    CIThreadItInterface ();

		/**
		 * Constructor CIThreadItInterface ensures the member variables are initialised.
		 * ptheWorker is a pointer to the CThreadIt instance that will be asked to
		 * do work.
		 */
    CIThreadItInterface (CThreadIt* ptheWorker);

		/**
		 * Constructor CIThreadItInterface ensures the member variables are initialised.
		 * ptheWorker is a pointer to the CThreadIt instance that will be asked to
		 * do work.
		 * ptheSender is a pointer to the CThreadIt instance that is asking for work
		 * to be done on it's behalf. If this reference is available it can be used
		 * for the worker to send a reply work request.
		 */
    CIThreadItInterface (CThreadIt* ptheWorker, CThreadIt* ptheSender);

  	/**
	   * Method ~CIThreadItInterface is the destructor for the instance.
	   */
    virtual ~CIThreadItInterface ();

		/**
		 * Method setWorker is called to set the work CThreadIt instance that is asked
		 * to perform work.
		 * ptheWorker is a pointer to the CThreadIt instance that will be asked to
		 * do work.
		 */
    void setWorker (CThreadIt* ptheWorker);

		/**
		 * Method getWorker is called to return the work CThreadIt instance that is asked
		 * to perform work on behalf of this instance.
		 * ptheWorker is a pointer to the CThreadIt instance that will be asked to
		 * do work.
		 * method getWorker returns true if ptheWorker is not NULL.
		 */
    bool getWorker (CThreadIt*& ptheWorker);


		/**
		 * Method setSender is called to set the work CThreadIt instance that is asking
		 * the worker to perform work on the sender's behalf.
		 * ptheSender is a pointer to the CThreadIt instance that is asking for work
		 * to be done on it's behalf. If this reference is available it can be used
		 * for the worker to send a reply work request.
		 */
    void setSender (CThreadIt* ptheSender);

		/**
		 * Method setPartners is called to set the communicating partners in a
		 * work request and response interaction.
		 * ptheWorker is a pointer to the CThreadIt instance that will be asked to
		 * do work.
		 * ptheSender is a pointer to the CThreadIt instance that is asking for work
		 * to be done on it's behalf. If this reference is available it can be used
		 * for the worker to send a reply work request.
		 */
    void setPartners (CThreadIt* ptheWorker, CThreadIt* ptheSender);

		/**
		 * Method sendMessage is called to send a work request with work payload
		 * to the worker instance.
		 * theServiceId is the identity of the work to be done. This is used to
		 * idnetify the method at the receiving worker of which method will be used
		 * to perform the work request.
		 * ptheMessage is the payload or the container of information that
		 * defines the work to be done. The work request is sent with no reply
		 * or notification options set.
		 */
    bool sendMessage (UINT theServiceId, void* ptheMessage);

  private:
		/**
		 * Method init is a private method that ensure that the member variables
		 * of this class are initialised in each constructor form.
		 */
    void init ();

}; // class CIThreadItInterface

#endif // !defined (I_THREADIT_INTERFACE_H)
