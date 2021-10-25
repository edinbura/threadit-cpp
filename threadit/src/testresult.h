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
 * Title: CTestResult
 * Description: CTestResult is the test operation result status.
 *
 * Copyright: Copyright (c) 2010 Roads and Traffic Authority of New South Wales
 * @author Ari Edinburg
 * @version 1.0
 */
#pragma once
#if !defined (TEST_RESULT_H)
#define TEST_RESULT_H

// Includes
#include <string>
#include "utils.h"
class CTestResult;

typedef std::shared_ptr<CTestResult> TestResultPtr;

class CTestResult
{
	// Attributes
	private:
		bool m_isSuccess;  // Is this a successful test result. m_isReady;	
		std::string m_theReason; // the reason for the success or failure. 
		std::string m_theOperationName;	// The operation for which this result applies. 
		RequestId m_theRequestId;
		unsigned int m_theResultId; // the result of what. 


	// Methods
	public:
		CTestResult () {};
		virtual ~CTestResult () {};

		/**
		 * retrieves the name of the requested operation to which this status object applies
		 */
		std::string getOperationName () const
		{
			return m_theOperationName;
		} // getOperationName

		/**
		 * sets the name of the operation that is being requested
		 */
		void setOperationName (const std::string& theOpName)
		{
			m_theOperationName = theOpName;
		} // setOperationName

		void setSuccess (bool isSuccess)
		{
			m_isSuccess = isSuccess;
		} 
		bool isSuccess ()
		{
			return m_isSuccess;
		} // 

		void setResultId (unsigned int theResultId)
		{
			m_theResultId = theResultId;
		} // setResultId
		
		unsigned int getResultId ()
		{
			return m_theResultId;
		} // 

		void setRequestId (RequestId theRequestId)
		{
			m_theRequestId = theRequestId;
		} // setRequestId
		
		RequestId getRequestId ()
		{
			return m_theRequestId;
		} // getRequestId

		void setReason (std::string theReason)
		{
			m_theReason = theReason;
		} // setReason

		std::string getReason ()
		{
			return m_theReason;
		} // 

		std::string toString ()
		{
			std::string theMsg;

			theMsg = "Test Result = \n";
			theMsg += "Result Id : " + to_str (m_theResultId) + " \n";	
			theMsg += "Status    : " + to_str (m_isSuccess) + "\n";	
			theMsg += "Operation : " + m_theOperationName + "\n";
			theMsg += "Status    : " + m_theReason;
			return theMsg;
		} // toString

}; // class CResultStatus

#endif // !defined (TEST_RESULT_H)