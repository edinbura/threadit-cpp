// CardioSub.h : interface of the CCardioSubject class
//
/////////////////////////////////////////////////////////////////////////////
#ifndef _DATA_QUEUE
#define _DATA_QUEUE

#include "DataBuffer.h"
#include "protectedqueue.h"


class CDataQueue
{
	// Constructors and Destructors
public :
	CDataQueue ();
	virtual ~CDataQueue();

	// Services
public :
	void addData (CDataBuffer*& theData);
	CDataBuffer* getData ();

	UINT size ();

	// Attributes
protected :
	CProtectedQueue <CDataBuffer> m_theDataQueue;

};
#endif
