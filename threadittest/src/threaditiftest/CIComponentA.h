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
 * Title: ComponentA
 * Description: The declaration for the interface class of Component A
 *  
 * Copyright: Copyright (c) 2012 Ashkel Software 
 * @author Ari Edinburg
 * @version 1.0
 * $Date: 2012/08/16 10:00:00 $
 */

#pragma once
#if !defined (I_COMPONENT_A_H)
#define I_COMPONENT_A_H

#include <threadit.h>
#include <threaditmessage.h>
#include <isafethreaditinterface.h>


/**
 * Interface class to Component A
 */
class CIComponentA : public CISafeThreadItInterface
{
public:
	/** 
	 * Work Instructions: The following constants define the work instructions
     * that the CIComponentA instance will perform on request.  
	 */
	enum ServiceIds
	{
		// request ids
		FUNCTION_A =0,
		FUNCTION_B,
		// reply ids
		FUNCTION_A_REPLY,
		FUNCTION_B_REPLY,
	}; 

	/**
	 * The CIComponentA is the constructor of the interface class of the ComponentA
	 */
	CIComponentA(const std::shared_ptr<CThreadIt>& ptheWorker): CISafeThreadItInterface (ptheWorker)	{	};

	/**
	 * The callFunctionA invokes the functionA from Component A implementation
 	 * The functionA is a sammple worker method implementation which will publish status ok message for testing perposes
	 */
	RequestId callFunctionA();

	/**
	 * The callFunctionB invokes the functionB from Component B implementation
	 * The functionB is a sammple worker method implementation which will publish status ok message for testing perposes
	 */
	RequestId callFunctionB();

};

#endif // !defined (I_COMPONENT_A_H)
