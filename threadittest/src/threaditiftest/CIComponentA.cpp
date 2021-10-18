/**
 * Title: CIComponentA definition
 * Description: The definition for the interface class of Component A
 *  
 * Copyright: Copyright (c) 2012 Road and Maritime Services
 * @author Nirosh Gunaratne
 * @version 1.0
 * $Date: 2012/08/16 10:00:00 $
 */

#include "CIComponentA.h"

/**
 * The callFunctionA invokes the functionA from Component A implementation
 * The functionA is a sammple worker method implementation which will publish status ok message for testing perposes
 */
RequestId CIComponentA::callFunctionA()
{
	long theWorkID = 0;
	ThreadItPtr theWorker = getSafeWorker();
		
	// send message to worker thread (ThreadItA)
	if(theWorker != NULL) 
	{
		CThreadItMessage aMsg(FUNCTION_A);			
		theWorkID = aMsg.sendTo(theWorker.get());
	}

	return theWorkID;
}//callFunctionA

/**
 * The callFunctionB invokes the functionB from Component B implementation
 * The functionB is a sammple worker method implementation which will publish status ok message for testing perposes
 */
RequestId CIComponentA::callFunctionB()
{
	long theWorkID = 0;
	ThreadItPtr theWorker = getSafeWorker();
		
	// send message to worker thread (ThreadItA)
	if(theWorker != NULL) 
	{
		CThreadItMessage aMsg(FUNCTION_B);			
		theWorkID = aMsg.sendTo(theWorker.get());
	}
	return theWorkID;
}//callFunctionB