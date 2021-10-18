/**
 * Title: CTimeIt
 * Description: Class CTimeIt is used to measure elapsed time based on the 
 * system tickcount. The methods in this class support an application to
 * peform measurements on processing time. 
 *
 * Copyright: Copyright (c) 2008 Ashkel Software 
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2006/12/19 02:38:06 $
 */
#if !defined(TIMEIT_H)
#define TIMEIT_H

#include <windows.h>

/*
 * Description: Class CTimeIt is used to measure elapsed time based on the 
 * system tickcount. The methods in this class support an application to
 * peform measurements on processing time. 
 */
class	 CTimeIt	
{
	// Attributes
protected:
	DWORD	 m_TStart;
	// m_TStart measures the start of a timing operation.
	DWORD	 m_TStop;
	// m_TStop measures the end of a timing operation.
	bool	 m_IsTiming;
	// m_IsTiming indicates if execution timing is in progress.
	DWORD	 m_TimeAllowed;
	// m_TimeAllowed is the time allowed before the timer expires.
	HANDLE m_TimeAccess;
	// m_TimeAccess is a critical section for the global timing data of the instance. 

public:
	/**
	 * Method CTimeIt is the constructor for the class. This method initialises the
	 * member variables for use.
	 */
	CTimeIt ();

	/**
	 * Method ~CTimeIt is	 the destructor for the class. This method frees any resources
	 * allocated for use.
	 */
	virtual ~CTimeIt ();

	/**
	 * Method StartTiming is called to record the start of work execution timing. 
	 * TimeAllowed specifies the time allocated for work to be executed. 
	 * A value of zero indicates that there is no timing restriction. 
	 */
	void startTiming (DWORD TimeAllowed);

	/**
	 * Method StopTiming is called to record the end of work execution timing. 
	 * Elapsed returns the time elapsed so far for work execution in milliseconds. 
	 */
	void stopTiming (DWORD& Elapsed);

	/**
	 * Method TimeElapsed calculates how much time has so far elapsed since timing
	 * was started. 
	 * Elapsed returns the time elapsed since timing was started.
	 * Method TimeElapsed returns true if timing is currently in progress.
	 */
	bool timeElapsed (DWORD& Elapsed) const;

	/**
	 * Method IsExpired is provided for use in the WorkerMethodType function
	 * to determine if the time allowed for processing has elapsed or not. If
	 * the time available has elpased then the worker method should stop 
	 * processing and return the status WORKDONE_TIME_OUT in the cWorkDoneIt
	 * result. 
	 * Elapsed returns the time elapsed so far for work execution in milliseconds. 
	 * If return value of zero and the method returns TRUE indicates that timing is
	 * not currently in progress.
	 * Method Expired returns true if the timer has expired. 
	 */
	bool isExpired (DWORD& Elapsed) const;

	/**
	 * Method reset resets the timing instance so that timing can be restarted without 
	 * having to create a new instance of this class.
	 */
	void reset ();

	/** 
	 * Method getRemaining returns the number of milliseconds remaining to time. 
	 */
	unsigned long getRemaining () const;

private:
  // CTimeIts are not copiable due to the HANDLE member
  CTimeIt(const CTimeIt&);
  const CTimeIt& operator=(const CTimeIt&);
}; // class CTimeIt

#endif // !defined(TIMEIT_H)
