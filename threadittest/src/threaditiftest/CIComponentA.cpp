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
 * Title: CIComponentA definition
 * Description: The definition for the interface class of Component A
 *  
 * Copyright: Copyright (c) 2012 Ashkel Software 
 * @author Ari Edinburg
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