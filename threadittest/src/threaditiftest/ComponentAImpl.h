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
 * Title: ComponentAImpl
 * Description: The declaration for the implementation class of Component A
 *  
 * Copyright: Copyright (c) 2012 Ashkel Software
 * @author Ari Edinburg
 * @version 1.0
 * $Date: 2012/08/16 10:00:00 $
 */

#pragma once
#if !defined (COMPONENT_A_IMPL_H)
#define COMPONENT_A_IMPL_H

#include <threadit.h>
#include <threaditmessage.h>
#include <isafethreaditinterface.h>
//#include <applibrary\utils\utils.h>
#include <testresultq.h>

#include "CIComponentA.h"

/**
 * Implementation class for the ComponentA
 */
class ComponentAImpl : public CThreadIt
{
protected:
	/**
	 * The m_ptheTestResultsQ is result queue for publishing status of work instructions for testing perpose
	 */
	CTestResultQ* m_ptheTestResultsQ;
public:
	/**
	 * The ComponentAImpl is the constructor of the componentA class
	 * The theModuleName is the module name
	 * The theTestResultQ is the test result queue for testability 
	 */
	ComponentAImpl(std::string theModuleName,const std::shared_ptr<CTestResultQ> theTestResultQ);
	
	/**
	 * The ComponentAImpl is the constructor of the componentA class
	 * The theModuleName is the module name
	 */
	ComponentAImpl(std::string theModuleName);
	
	/**
	 * The ComponentAImpl is the constructor of the componentA class
	 * The theModuleName is the module name
	 * The thePriority is the thread priority
	 */
	ComponentAImpl(int thePriority,std::string theModuleName);

	/**
	 * The initialize is the shard initializer for the componentA
	 */
	void initialize();

	/**
	 * The ~ComponentAImpl is the destructor of the componentA
	 */
	~ComponentAImpl();

	/**
	 * The functionA is a sammple worker method implementation which will publish status ok message for testing perposes
	 */
	bool funtionA(CWorkPackIt* ptheWorkPack, CWorkPackIt*& ptheWorkDone);

	/**
	 * The funtionB is a sammple worker method implementation which will publish status ok message for testing perposes
	 */
	bool funtionB(CWorkPackIt* ptheWorkPack, CWorkPackIt*& ptheWorkDone);
};

#endif // !defined (COMPONENT_A_IMPL_H)