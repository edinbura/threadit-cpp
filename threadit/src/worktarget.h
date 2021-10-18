/**
 * Title: CServiceManager
 * Description: Class CServiceManager provides access to the scatscache port.
 *
 * Copyright: Copyright (c) 2008 Ashkel Software
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2006/12/19 02:38:06 $
 */

#pragma once
#if !defined (WORK_TARGET_H)
#define WORK_TARGET_H

// Includes
#include "threadit.h"

/**
 * Class CServiceManagerImpl implements an interface to a serial port and
 * uses asynchronous techniques for reading and writing.
 */
class CWorkTarget
{
  // Attributes
  protected:

		CThreadIt* m_ptheWorkTarget;
		UINT m_theWorkId;

  // Methods
  public:
    /**
	   * Method CServiceManagerImpl is the constructor for the class. This method intialises the
	   * serial port and necessary handles for operation. A default thread priority is
	   * allocated when using this constructor.
	   */
    CWorkTarget ();

    /**
	   * Method CServiceManagerImpl is the constructor for the class. This method intialises the
	   * serial port and necessary handles for operation.
	   * thePriority is the priority level that can be allocated to the thread of execution
	   * that is associated with this instance.
  	 */
     CWorkTarget (CThreadIt* ptheWorkTarget, UINT theWorkId);

  	/**
	   * Method CServiceManagerImpl is the destructor for the instance. The method closes all
	   * existing handles and then initiates the thread stop sequences an waits for the
	   * thread associated with this instance to terminate.
	   */
    ~CWorkTarget ();

		/**
		 * Method setPortConfig is called in response to a request to configure the serial port
		 * for operation.
		 */
		 bool setWorkTarget (CThreadIt* ptheWorkTarget, UINT theWorkId);
		 bool getWorkTarget (CThreadIt*& ptheWorkTarget, UINT& theWorkId);
	
}; // class CWorkTarget

#endif // !defined (WORK_TARGET_H)
