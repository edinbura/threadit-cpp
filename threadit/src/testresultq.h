/**
 * Title: CTestResultQ
 * Description: CTestResultQ is the test operation result status queue.
 *
 * Copyright: Copyright (c) 2010 Roads and Traffic Authority of New South Wales
 * @author Ari Edinburg
 * @version 1.0
 */
#pragma once
#if !defined (TEST_RESULT_Q_H)
#define TEST_RESULT_Q_H

// Includes
#include <string>
#include "threadit.h"
#include "mtqueue.h"
#include "testresult.h"

class CTestResultQ
{
	// Attributes
	protected:
		CMtQueue<TestResultPtr> m_theResQ;
	// Methods
	public:
		CTestResultQ () {};
		~CTestResultQ () {};

		void publish (TestResultPtr& ptheResult)
		{
			m_theResQ.insertItem (ptheResult);
		} // publish

		void publish (unsigned int theResultId, std::string theOpName, bool isOk, std::string theMsg)
		{
			TestResultPtr ptheResult (new CTestResult ());
			ptheResult->setResultId (theResultId);
			ptheResult->setOperationName (theOpName);
			ptheResult->setSuccess (isOk);
			ptheResult->setReason (theMsg);
			m_theResQ.insertItem (ptheResult);
		} // publish

		void publish (unsigned int theResultId, RequestId theRequestId, std::string theOpName, bool isOk, std::string theMsg)
		{
			TestResultPtr ptheResult (new CTestResult ());
			ptheResult->setResultId (theResultId);
			ptheResult->setRequestId (theRequestId);
			ptheResult->setOperationName (theOpName);
			ptheResult->setSuccess (isOk);
			ptheResult->setReason (theMsg);
			m_theResQ.insertItem (ptheResult);
		} // publish


		// if the id is zero return anything that comes in. Waits for that result type
		// until time out.
		bool getResult (unsigned int theId, TestResultPtr& theResult, long theWaitTime)
		{
			bool isWaiting = true;
			bool isResult = false;
			long theTimeLeft = 0;
			DWORD aPeriod = 0;
			CTimeIt theTimer;

			theResult.reset ();
			theTimer.startTiming (theWaitTime);
			do
			{
				if (theTimer.timeElapsed (aPeriod)) // still timing
				{ 
					theTimeLeft = theWaitTime - aPeriod; // how long left
					if (theTimeLeft > 20) // we should wait.
					{
						if (m_theResQ.waitItem (theResult, theTimeLeft))
						{
							if (theId == 0)
							{
								isResult = true;
								isWaiting = false;
							}
							else
							{
								if (theResult->getResultId () == theId)
								{
									isResult = true;
									isWaiting = false;
								}
								else
								{
									// This is not the expected result.
									theResult.reset ();
								} // if 
							} // if 
						}
						else
						{
							// We have expired. 
							isWaiting = false;
						} // 
					}
					else
					{
						// We have expired. 
						isWaiting = false;
					} // if 
				}
				else
				{
					// We have expired. 
					isWaiting = false;
				} // if 
			} while (isWaiting);
			return isResult;
		} // getResult

		// if the id is zero return anything that comes in.
		bool getSuccessResult (unsigned int theId, TestResultPtr& theResult, long theLoops, long theLoopWaitTime)
		{
			long theCount = 0;
			bool isResult = false;
			theResult.reset ();
			do
			{
				getResult (theId, theResult, theLoopWaitTime);
				if ((theResult) && (theResult->isSuccess ()))
				{
					isResult = true;
				} // if 
			} while ((theCount < theLoops) && (!isResult));
			return isResult;
		} // getSuccessResult


}; // class CResultStatus

#endif // !defined (TEST_RESULT_Q_H)
