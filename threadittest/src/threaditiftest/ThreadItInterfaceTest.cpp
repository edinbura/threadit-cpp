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
 * Title: TestThreadItInterface
 * Description: TestThreadItInterface contains test suite for testing the interface funtionality 
 * of the threadit library. 
 *
 * Copyright: Copyright (c) 2012 Ashkel Software 
 * @author Ari Edinburg
 * @version 1.0
 * $Date: 2012/08/16 10:00:00 $
 */
#include <apputils.h> 
//#include <applibrary\utils\utils.h>
#include <threadit.h>
#include <threaditmessage.h>
#include <isafethreaditinterface.h>
#include <UnitTest++.h>
#include <testresultq.h>
#include <log4cpp/Category.hh>
#include <log4cpp/BasicConfigurator.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <sstream>
#include "CIComponentA.h"
#include "CIComponentB.h"
#include "ComponentAImpl.h"
#include "ComponentBImpl.h"

using UnitTest::TestDetails;

/**
 * threaditinterfacetest is the namespace for the threadit interface test
 */
namespace threaditinterfacetest
{
	
std::string const TEST_MODULE_NAME("threadit.threaditinterfacetest");		

/**
 * Test class for testing interface funtionality of the ThreadIt library
 */
class TestThreadItInterfaceFixture
{
public:	
	std::shared_ptr<CTestResultQ> m_ptheTestResultsQFromA;
	std::shared_ptr<CTestResultQ> m_ptheTestResultsQFromB;
	std::shared_ptr<ComponentAImpl> m_ptheImplA;
	std::shared_ptr<ComponentBImpl> m_ptheImplB;
	const static long m_theWaitTime = 5000;
	
	/**
	 * TestThreadItInterfaceFixture is the constructor of the test class
	 */
	TestThreadItInterfaceFixture()
	{
		m_ptheTestResultsQFromA.reset( new CTestResultQ());
		m_ptheTestResultsQFromB.reset( new CTestResultQ());
		m_ptheImplA.reset(new ComponentAImpl("ThreadItA",m_ptheTestResultsQFromA));
		m_ptheImplB.reset(new ComponentBImpl("ThreadItB",m_ptheTestResultsQFromB));
	};

	/**
	 * ~TestThreadItInterfaceFixture is the destructor of the test class
	 */
	~TestThreadItInterfaceFixture()
	{
		m_ptheImplA.reset();
		m_ptheImplB.reset();
	}

	/**
	 * TestCallComponentA is the test case for testing the functoinality of the componentA
	 * Test for call a funtion from Component A and get a reply to confrim the correct method invoked
	 */
	void TestCallComponentA(const TestDetails& details);
	
	/**
	 * TestCallComponentB is the test case for testing the functoinality of the componentB
	 * Test for call a funtion from Component B and get a reply to confrim the correct method invoked
	 */
	void TestCallComponentB(const TestDetails& details);

	/**
	 * TestCallOnComponentBWithReplyToFunctionB is the test case for testing the reply functionality using safe threadit interface
	 */
	void TestCallOnComponentBWithReplyToFunctionB(const TestDetails& details);
};


/**
 * TestCallComponentA is the test case for testing the functoinality of the componentA
 * Test for call a funtion from Component A and get a reply to confrim the correct method invoked
 */
void TestThreadItInterfaceFixture::TestCallComponentA(const TestDetails& details)
{
	TestResultPtr theResult;
	UINT theId=0;
	RequestId theRequestId=0;
	log4cpp::Category *ptheLogger =&(log4cpp::Category::getInstance (TEST_MODULE_NAME)); 
	ptheLogger->info("Testing %s started",details.testName);

	CIComponentA* theCompA = new CIComponentA(m_ptheImplA);
	theCompA->callFunctionA();
	theRequestId = m_ptheTestResultsQFromA->getResult(theId,theResult,m_theWaitTime);
	CHECK_EQUAL(true,theResult.get()->isSuccess());
	CHECK_EQUAL("funtionA",theResult.get()->getOperationName());
	CHECK_EQUAL(theRequestId,theResult->getRequestId());
	CHECK_EQUAL(CIComponentA::FUNCTION_A_REPLY,theResult->getResultId());
	
	delete theCompA;

	ptheLogger->info("Testing %s completed",details.testName);
}//TestCallComponentA

/**
 * TestCallComponentB is the test case for testing the functoinality of the componentB
 * Test for call a funtion from Component B and get a reply to confrim the correct method invoked
 */
void TestThreadItInterfaceFixture::TestCallComponentB(const TestDetails& details)
{
	TestResultPtr theResult;
	UINT theId=0;
	RequestId theRequestId=0;
	log4cpp::Category *ptheLogger =&(log4cpp::Category::getInstance (TEST_MODULE_NAME)); 
	ptheLogger->info ("Testing %s started",details.testName);	

	CIComponentB* theCompB = new CIComponentB(m_ptheImplB);
	theCompB->callFunctionC();	
	theRequestId = m_ptheTestResultsQFromB->getResult(theId,theResult,m_theWaitTime);
	CHECK_EQUAL(true,theResult.get()->isSuccess());
	CHECK_EQUAL("funtionC",theResult.get()->getOperationName());
	CHECK_EQUAL(theRequestId,theResult->getRequestId());
	CHECK_EQUAL(CIComponentB::FUNCTION_C_REPLY,theResult->getResultId());

	delete theCompB;

	ptheLogger->info ("Testing %s completed",details.testName);
}//TestCallComponentB

/**
 * TestCallOnComponentBWithReplyToFunctionB is the test case for testing the reply functionality using safe threadit interface
 */
void TestThreadItInterfaceFixture::TestCallOnComponentBWithReplyToFunctionB(const TestDetails& details)
{
	TestResultPtr theResult;
	UINT theId=0;
	RequestId theRequestId=0;
	log4cpp::Category *ptheLogger =&(log4cpp::Category::getInstance (TEST_MODULE_NAME)); 
	ptheLogger->info ("Testing %s started",details.testName);	

	CIComponentB* theCompB = new CIComponentB(m_ptheImplB);
	theRequestId = theCompB->callFunctionD(m_ptheImplA,CIComponentA::FUNCTION_B);
	
	m_ptheTestResultsQFromB->getResult(theId,theResult,m_theWaitTime);
	CHECK_EQUAL(true,theResult->isSuccess());
	CHECK_EQUAL("funtionD",theResult->getOperationName());
	CHECK_EQUAL(theRequestId,theResult->getRequestId());
	CHECK_EQUAL(CIComponentB::FUNCTION_D_REPLY,theResult->getResultId());

	theRequestId = m_ptheTestResultsQFromA->getResult(theId=0,theResult=NULL,m_theWaitTime);
	CHECK_EQUAL(true,theResult.get()->isSuccess());
	CHECK_EQUAL("funtionB",theResult.get()->getOperationName());
	CHECK_EQUAL(theRequestId,theResult->getRequestId());
	CHECK_EQUAL(CIComponentA::FUNCTION_B_REPLY,theResult->getResultId());

	delete theCompB;	

	ptheLogger->info ("Testing %s completed",details.testName);
}//TestCallOnComponentBWithReplyToFunctionB

};

using namespace threaditinterfacetest;

/**
 * TestThreadItInterface is the test suite
 */
SUITE(TestThreadItInterface)
{
	/**
	 * test case for tets component A
	 */
	TEST_FIXTURE(TestThreadItInterfaceFixture,TestCallComponentA)
	{
		TestCallComponentA(m_details);
	}

	/**
	 * test case for tets component B
	 */
	TEST_FIXTURE(TestThreadItInterfaceFixture,TestCallComponentB)
	{
		TestCallComponentB(m_details);
	}

	/**
	 * test case for tets function with safe threadit reply
	 */
	TEST_FIXTURE(TestThreadItInterfaceFixture,TestCallOnComponentBWithReplyToFunctionB)
	{
		TestCallOnComponentBWithReplyToFunctionB(m_details);
	}
}