/**
 * Title: ComponentAImpl definition
 * Description: The definition for the implementation class of Component A
 *  
 * Copyright: Copyright (c) 2012 Road and Maritime Services
 * @author Nirosh Gunaratne
 * @version 1.0
 * $Date: 2012/08/16 10:00:00 $
 */

#include "CIComponentA.h"
#include "ComponentAImpl.h"

/**
 * The ComponentAImpl is the constructor of the componentA class
 * The theModuleName is the module name
 * The theTestResultQ is the test result queue for testability 
 */
ComponentAImpl::ComponentAImpl(std::string theModuleName,const std::shared_ptr<CTestResultQ> theTestResultQ):CThreadIt(theModuleName)
{
	// initialize test queue from input parameters
	m_ptheTestResultsQ = theTestResultQ.get();

	// initialize worker methods
	initialize();
};

/**
 * The ComponentAImpl is the constructor of the componentA class
 * The theModuleName is the module name
 */
ComponentAImpl::ComponentAImpl(std::string theModuleName):CThreadIt(theModuleName)
{ 	
	// initialize worker methods
	initialize();
};

/**
 * The ComponentAImpl is the constructor of the componentA class
 * The theModuleName is the module name
 * The thePriority is the thread priority
 */
ComponentAImpl::ComponentAImpl(int thePriority,std::string theModuleName):CThreadIt(theModuleName,thePriority)
{ 
	// initialize worker methods
	initialize();
};

/**
 * The initialize is the shard initializer for the componentA
 */
void ComponentAImpl::initialize()
{
	// initialize worker methods
	setWorkerMethod((WorkerMethodType)&ComponentAImpl::funtionA,CIComponentA::FUNCTION_A);
	setWorkerMethod((WorkerMethodType)&ComponentAImpl::funtionB,CIComponentA::FUNCTION_B);
}

/**
 * The ~ComponentAImpl is the destructor of the componentA
 */
ComponentAImpl::~ComponentAImpl()
{
	// request to stop the worker thread
	stopThread ();	 

	// wait until worker thread stop is completed
	waitForThreadToStop (); 
}
/**
 * The functionA is a sammple worker method implementation which will publish status ok message for testing perposes
 */
bool ComponentAImpl::funtionA(CWorkPackIt* ptheWorkPack, CWorkPackIt*& ptheWorkDone)
{
	bool isChecked = false;
	long theStatus = CThreadIt::THREADIT_STATUS_OK;

	// Check that the parameters received are valid.
	isChecked = checkParams(ptheWorkPack,ptheWorkDone,theStatus,CThreadIt::COPY_PARAMS); 		

	// Publish status to test results Q for testing perposes
	m_ptheTestResultsQ->publish(CIComponentA::FUNCTION_A_REPLY,ptheWorkPack->m_theWorkPackID,"funtionA",isChecked,"funtionA is ok");
	
	// Free allocated memory for the received work pack
	delete ptheWorkPack;		

	// Return the method status.
	return isChecked;
}

/**
 * The funtionB is a sammple worker method implementation which will publish status ok message for testing perposes
 */
bool ComponentAImpl::funtionB(CWorkPackIt* ptheWorkPack, CWorkPackIt*& ptheWorkDone)
{
	bool isChecked = false;
	long theStatus = CThreadIt::THREADIT_STATUS_OK;

	// Check that the parameters received are valid.
	isChecked = checkParams(ptheWorkPack,ptheWorkDone,theStatus,CThreadIt::COPY_PARAMS); 	
	
	// Publish status to test results Q for testing perposes
	m_ptheTestResultsQ->publish(CIComponentA::FUNCTION_B_REPLY,ptheWorkPack->m_theWorkPackID,"funtionB",isChecked,"funtionB is ok");

	//free allocated memory for the received work pack
	delete ptheWorkPack;

	// Return the method status.
	return isChecked;
}
