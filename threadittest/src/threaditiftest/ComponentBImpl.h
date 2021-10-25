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
 * Title: ComponentBImpl
 * Description: The declaration for the implementation class of Component B
 *  
 * Copyright: Copyright (c) 2012 Ashkel Software
 * @author Ari Edinburg
 * @version 1.0
 * $Date: 2012/08/16 10:00:00 $
 */
#pragma once
#if !defined (COMPONENT_B_IMPL_H)
#define COMPONENT_B_IMPL_H

#include <threadit.h>
#include <threaditmessage.h>
#include <isafethreaditinterface.h>
//#include <applibrary\utils\utils.h>
#include <testresultq.h>

#include "CIComponentA.h"
#include "CIComponentB.h"

/**
 * Implementation class for the ComponentB
 */
class ComponentBImpl : public CThreadIt
{
protected:
	/**
	 * The m_ptheTestResultsQ is result queue for publishing status of work instructions for testing perpose
	 */
	CTestResultQ* m_ptheTestResultsQ;
public:
	/**
	 * The ComponentBImpl is the constructor of the componentA class
	 * The theModuleName is the module name
	 * The theTestResultQ is the test result queue for testability 
	 */
	ComponentBImpl(std::string theModuleName,const std::shared_ptr<CTestResultQ> theTestResultQ);

	/**
	 * The ComponentBImpl is the constructor of the componentA class
	 * The theModuleName is the module name
	 */
	ComponentBImpl(std::string theModuleName);

	/**
	 * The ComponentBImpl is the constructor of the componentA class
	 * The theModuleName is the module name
	 * The thePriority is the thread priority
	 */
	ComponentBImpl(int thePriority,std::string theModuleName);

	/**
	 * The initialize is the shard initializer for the componentA
	 */
	void initialize();

	/**
	 * The ~ComponentBImpl is the destructor of the componentA
	 */
	~ComponentBImpl();

	/**
	 * The funtionC is a sammple worker method implementation which will publish status ok message for testing perposes
	 */
	bool funtionC(CWorkPackIt* ptheWorkPack, CWorkPackIt*& ptheWorkDone);

	/**
	 * The functionD is a sammple worker method implementation which will publish status ok message for testing perposes and send a reply to functionB in
	 * componentA using safe threadIt interface of the componentA
	 */
	bool funtionD(CWorkPackIt* ptheWorkPack, CWorkPackIt*& ptheWorkDone);
};

#endif // !defined (COMPONENT_B_IMPL_H)