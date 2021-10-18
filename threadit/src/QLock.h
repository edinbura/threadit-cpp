/**
 * Title: CQLock
 * Description: class CDataBuffer is provided to supported reading in a sequence
 * of bytes from input/output (I/O) devices. As each sequence of bytes of 
 * however long are read in they can be stored in a CDataBuffer instance. The class
 * supports concatention of buffers so as a number of buffers are read in the can
 * be concatenated into a single buffer. 
 *
 * Copyright: Copyright (c) 2008 Ashkel Software 
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2006/12/19 02:38:06 $
 */
#ifndef _QLOCK
#define _QLOCK

// Include files
#include <windows.h>

/**
 * Class CQLock 
 */
class CQLock 
{
	// Attributes
public:
	typedef CQLock *Mutex;

private:
	// flag that is set after lock is released by predecessor
	LONG m_theReadyFlag; 
	// flag that is set after d_next is set by successor
	LONG m_theNextFlag;	 
	// pointer to tail of queue
	Mutex *m_ptheMutex;			
	// pointer to successor in queue
	CQLock *m_ptheNext;			
												
	// Constructors and Destructors
public:
	/** 
	 * CQLock is the	constructor for the class. 
	 */
	CQLock (Mutex *ptheMutex);

	/** 
	 * ~CQLock is the destructor for the class. The method 
	 */
	~CQLock ();

	// Services
private: 
	/**
	 *
	 */
	static void setFlag (LONG *ptheFFlag);

	/**
	 *
	 */
	static void waitOnFlag (LONG *pTheFlag);

}; // class CQLock

#endif	// #ifndef _QLOCK
