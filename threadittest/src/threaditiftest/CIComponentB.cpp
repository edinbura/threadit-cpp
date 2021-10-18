/**
 * Title: CIComponentB definition
 * Description: The definition for the interface class of Component B
 *  
 * Copyright: Copyright (c) 2012 Road and Maritime Services
 * @author Nirosh Gunaratne
 * @version 1.0
 * $Date: 2012/08/16 10:00:00 $
 */

#include "CIComponentB.h"

/**
 * The callFunctionC invokes the functionC from Component C implementation
 * The functionC is a sample worker method implementation which will publish status ok message for testing perposes
 */
RequestId CIComponentB::callFunctionC()
{
	long theWorkID = 0;
	ThreadItPtr theWorker = getSafeWorker();
		
	// send message to worker thread (ThreadItB)
	if(theWorker != NULL) 
	{
		CThreadItMessage aMsg(FUNCTION_C);			
		theWorkID = aMsg.sendTo(theWorker.get());
	}
	return theWorkID;
}//callFunctionC()

/**
 * The callFunctionD invokes the functionD from Component D implementation
 * The functionD is a sammple worker method implementation which will publish status ok message for testing perposes and send a reply to functionB in
 * componentA using safe threadIt interface of the componentA
 */
RequestId CIComponentB::callFunctionD(const std::shared_ptr<CThreadIt>&  ptheSender, UINT theReplyFunction)
{
	long theWorkID = 0;
	ThreadItPtr theWorker = getSafeWorker();
		
	// send message to worker thread (ThreadItB)
	if(theWorker != NULL) 
	{
		CThreadItMessage aMsg(FUNCTION_D);		
		aMsg.setSourceInfo(ptheSender,theReplyFunction);
		theWorkID = aMsg.sendTo(theWorker.get());
	}
	return theWorkID;
}//callFunctionD()