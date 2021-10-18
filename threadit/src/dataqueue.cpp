#include "stdafx.h"
#include "DataQueue.h"


CDataQueue::CDataQueue ()
{
} //
CDataQueue::~CDataQueue()
{
	CDataBuffer* ptheDataBuffer = NULL;

	while (m_theDataQueue.size () > 0)
	{
		ptheDataBuffer = m_theDataQueue.getItem ();
		if (ptheDataBuffer != NULL)
		{
			delete ptheDataBuffer;
		} // if 
	} // while
} // destructor ~CDataQueue

void CDataQueue::addData (CDataBuffer*& theData)
{
	m_theDataQueue.insertItem (theData);
//	NotifyOnChange();
} // AddData

CDataBuffer* CDataQueue::getData ()
{
	CDataBuffer* ptheDataBuffer;

	ptheDataBuffer = m_theDataQueue.getItem (); 
	return ptheDataBuffer;
} // GetData

UINT CDataQueue::size ()
{
	return m_theDataQueue.size ();
} // size
