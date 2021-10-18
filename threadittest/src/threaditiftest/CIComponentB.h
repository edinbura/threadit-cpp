/**
 * Title: ComponentB
 * Description: The declaration for the interface class of Component B
 *  
 * Copyright: Copyright (c) 2012 Road and Maritime Services
 * @author Nirosh Gunaratne
 * @version 1.0
 * $Date: 2012/08/16 10:00:00 $
 */

#pragma once
#if !defined (I_COMPONENT_B_H)
#define I_COMPONENT_B_H

#include <threadit.h>
#include <threaditmessage.h>
#include <isafethreaditinterface.h>

/**
 * Interface class to Component B
 */
class CIComponentB : public CISafeThreadItInterface
{
public:
	/** 
	 * Work Instructions: The following constants define the work instructions
     * that the CIComponentB instance will perform on request.  
	 */
	enum ServiceIds
	{
		// request ids
		FUNCTION_C =0,
		FUNCTION_D,
		// reply ids
		FUNCTION_C_REPLY,
		FUNCTION_D_REPLY,
	}; 
	
	/**
	 * The CIComponentB is the constructor of the interface class of the ComponentB
	 */
	CIComponentB(const std::shared_ptr<CThreadIt>& ptheWorker): CISafeThreadItInterface (ptheWorker)	{	};

	/**
	 * The callFunctionC invokes the functionC from Component C implementation
	 * The functionC is a sample worker method implementation which will publish status ok message for testing perposes
	 */
	RequestId callFunctionC();

	/**
	 * The callFunctionD invokes the functionD from Component D implementation
	 * The functionD is a sammple worker method implementation which will publish status ok message for testing perposes and send a reply to functionB in
	 * componentA using safe threadIt interface of the componentA
	 */
	RequestId callFunctionD(const std::shared_ptr<CThreadIt>&  ptheSender, UINT theReplyFunction);

};

#endif // !defined (I_COMPONENT_B_H)
