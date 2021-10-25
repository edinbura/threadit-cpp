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
 * Title: ComponentBImpl definition
 * Description: The definition for the implementation class of Component B
 *  
 * Copyright: Copyright (c) 2012 Ashkel Software
 * @author Ari Edinburg
 * @version 1.0
 * $Date: 2012/08/16 10:00:00 $
 */

#include "CIComponentA.h"
#include "CIComponentB.h"
#include "ComponentBImpl.h"

/**
 * The ComponentBImpl is the constructor of the componentB class
 * The theModuleName is the module name
 * The theTestResultQ is the test result queue for testability 
 */
ComponentBImpl::ComponentBImpl(std::string theModuleName,const std::shared_ptr<CTestResultQ> theTestResultQ):CThreadIt(theModuleName)
{ 
	// initialize test queue from input parameters
	m_ptheTestResultsQ = theTestResultQ.get();
	
	// initialize worker methods
	initialize();
};

/**
 * The ComponentBImpl is the constructor of the componentB class
 * The theModuleName is the module name
 */
ComponentBImpl::ComponentBImpl(std::string theModuleName):CThreadIt(theModuleName)
{ 	
	// initialize worker methods
	initialize();
};

/**
 * The ComponentBImpl is the constructor of the componentB class
 * The theModuleName is the module name
 * The thePriority is the thread priority
 */
ComponentBImpl::ComponentBImpl(int thePriority,std::string theModuleName):CThreadIt(theModuleName,thePriority)
{ 
	// initialize worker methods
	initialize();
};

/**
 * The initialize is the shard initializer for the componentA
 */
void ComponentBImpl::initialize()
{
	// initialize worker methods
	setWorkerMethod((WorkerMethodType)&ComponentBImpl::funtionC,CIComponentB::FUNCTION_C);
	setWorkerMethod((WorkerMethodType)&ComponentBImpl::funtionD,CIComponentB::FUNCTION_D);
}

/**
 * The ~ComponentBImpl is the destructor of the componentA
 */
ComponentBImpl::~ComponentBImpl()
{
	// request to stop the worker thread
	stopThread ();	

	// wait until worker thread stop is completed
	waitForThreadToStop ();
}

/**
 * The functionC is a sample worker method implementation which will publish status ok message for testing perposes
 */
bool ComponentBImpl::funtionC(CWorkPackIt* ptheWorkPack, CWorkPackIt*& ptheWorkDone)
{
	bool isChecked = false;
	long theStatus = CThreadIt::THREADIT_STATUS_OK;
	
	// Check that the parameters received are valid.
	isChecked = checkParams(ptheWorkPack,ptheWorkDone,theStatus,CThreadIt::COPY_PARAMS); 
	
	// Publish status to test results Q for testing perposes
	m_ptheTestResultsQ->publish(CIComponentB::FUNCTION_C_REPLY,ptheWorkPack->m_theWorkPackID,"funtionC",isChecked,"funtionC is ok");
	
	// Free allocated memory for the received work pack
	delete ptheWorkPack;
	
	// Return the method status.
	return isChecked;

}//funtionC

/**
 * The functionD is a sammple worker method implementation which will publish status ok message for testing perposes and send a reply to functionB in
 * componentA using safe threadIt interface of the componentA
 */
bool ComponentBImpl::funtionD(CWorkPackIt* ptheWorkPack, CWorkPackIt*& ptheWorkDone)
{
	bool isChecked = false;
	long theStatus = CThreadIt::THREADIT_STATUS_OK;

	// Check that the parameters received are valid.
	isChecked = checkParams(ptheWorkPack,ptheWorkDone,theStatus,CThreadIt::COPY_PARAMS); 
	
	// Publish status to test results Q for testing perposes
	m_ptheTestResultsQ->publish(CIComponentB::FUNCTION_D_REPLY,ptheWorkPack->m_theWorkPackID,"funtionD",isChecked,"funtionD is ok");

	// Now send a reply to funtion B in Component A
	std::shared_ptr<CThreadIt> theImplA( ptheWorkPack->m_wptheSource);
	
	//retriew the work instruction to be replied from the work pack
	ULONG theInstruction= ptheWorkPack->getWorkInstruction();
	if(theImplA != NULL)
	{
		// create interface for compoentA using threadIt reference from the work pack
		CIComponentA* theCompA = new CIComponentA(theImplA);

		if(theInstruction == CIComponentA::FUNCTION_B)
		{
			// invoke the functionB in the componentA as a reply
			theCompA->callFunctionB();
		}
		// release memory allocated to the componentA interface
		delete theCompA;
	}

	// Free allocated memory for the received work pack
	delete ptheWorkPack;
	
	// Return the method status.
	return isChecked;
}//funtionD

