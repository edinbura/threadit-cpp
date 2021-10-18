#include "stdafx.h"
#include "worktarget.h"

/**
 * Method CServiceManagerImpl is the constructor for the class. This method intialises the
 * serial port and necessary handles for operation. A default thread priority is
 * allocated when using this constructor.
 */
CWorkTarget::CWorkTarget ()
{
	m_ptheWorkTarget = NULL;
	m_theWorkId = 0;
} // CWorkTarget

/**
 * Method CServiceManagerImpl is the constructor for the class. This method intialises the
 * serial port and necessary handles for operation.
 * thePriority is the priority level that can be allocated to the thread of execution
 * that is associated with this instance.
 */
CWorkTarget::CWorkTarget (CThreadIt* ptheWorkTarget, UINT theWorkId)
{
	m_ptheWorkTarget = ptheWorkTarget;
	m_theWorkId = theWorkId;
} // CWorkTarget


/**
 * Method CServiceManagerImpl is the destructor for the instance. The method closes all
 * existing handles and then initiates the thread stop sequences an waits for the
 * thread associated with this instance to terminate.
 */
CWorkTarget::~CWorkTarget ()
{
	m_ptheWorkTarget = NULL;
}

/**
 * Method setPortConfig is called in response to a request to configure the serial port
 * for operation.
 */
bool CWorkTarget::setWorkTarget (CThreadIt* ptheWorkTarget, UINT theWorkId)
{
	bool isSuccess = false;

	if ((ptheWorkTarget != NULL) && (theWorkId > 0) && (CThreadIt::MAX_WORK_METHODS > theWorkId))
	{
		m_ptheWorkTarget = ptheWorkTarget;
		m_theWorkId = theWorkId;
		isSuccess = true;
	} // if 
	return isSuccess;
} // setWorkTarget 

bool CWorkTarget::getWorkTarget (CThreadIt*& ptheWorkTarget, UINT& theWorkId)
{
	bool isSuccess = false;

  ptheWorkTarget = NULL;
	theWorkId = 0;
	if ((m_ptheWorkTarget != NULL) && (m_theWorkId > 0) && (CThreadIt::MAX_WORK_METHODS > m_theWorkId))
	{
		ptheWorkTarget = m_ptheWorkTarget;
		theWorkId = m_theWorkId;
		isSuccess = true;
	} // if 
	return isSuccess;
} // getWorkTarget
