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

#ifndef _THREADITCALLBACK
#define _THREADITCALLBACK

#include <memory>
#include "dataitem.h"
#include "subject.h"
/**
 * class CThreadItCallback extends class CSubject. An instance of this class 
 * allows a callback to be invoked from within a threadit to invoke a method
 * in another threadit. This callback can also be invoked in the periodic method
 * of the invoking threadit. 
 */ 
class CThreadItCallback : public CSubject
{
protected:
	bool m_isPeriodic; 
  ULONG m_theWorkId;
	// m_Instruction is the instruction of work to perform. The value starts
	// from one onwards. A value of zero implies no work to be performed. 
	ULONG m_theWorkInstruction;
	DataItemPtr m_ptheDataItem;

public: 
	CThreadItCallback ()
	{
		m_theWorkInstruction = 0;
		m_isPeriodic = false;
		m_theWorkId = 0;
	} // constructor CMySubject

	CThreadItCallback (bool isPeriodic, ULONG theInstruction, ULONG theWorkId)
	{
		m_theWorkInstruction = theInstruction;
		m_isPeriodic = isPeriodic;
		m_theWorkId = theWorkId;
	} // constructor CMySubject

	~CThreadItCallback ()
	{
	} // destructor ~CMyObserver

	void setPeriodic (bool isPeriodic)
	{
		m_isPeriodic = isPeriodic;
	} // setPeriodic

	const bool isPeriodic () const
	{
		return m_isPeriodic;
	} // isPeriodic

  void setDataItem (DataItemPtr const &ptheDataItem)
	{
		m_ptheDataItem = ptheDataItem;
	} // setDataItem

  void getDataItem (DataItemPtr& ptheDataItem) const
	{
    ptheDataItem = m_ptheDataItem;
	} // getDataItem

  void setWorkInstruction (ULONG theWorkInstruction)
	{
		m_theWorkInstruction = theWorkInstruction;
	} // setWorkInstruction

	const ULONG getWorkInstruction () const
	{
		return m_theWorkInstruction;
	} // getWorkInstruction

	void setWorkId (ULONG theWorkId)
	{
		m_theWorkId = theWorkId;
	} // setWorkId

	const ULONG getWorkId () const
	{
		return m_theWorkId;
	} // m_theWorkId

}; // class CThreadItCallback

#endif // #ifndef _THREADITCALLBACK
