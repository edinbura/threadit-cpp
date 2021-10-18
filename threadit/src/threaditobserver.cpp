/**
 * Title: CThreadItObserver
 * Description: class CThreadItObserver is the virtual base class for a class that wishes to receive 
 * notifications when information of interest updates or changes. This is an 
 * implementation of the Observer pattern and allows a class to be notified of 
 * change asynchronously. Classes declare an interest by becoming observers of a subject. 
 * When data related to the subject changes the class method is invoked to notify 
 * the observers of the change or update. See class CSubject as the partner of the 
 * CThreadItObserver class in the implementation of this observer pattern.
 *
 * Copyright: Copyright (c) 2008 Ashkel Software 
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2006/12/19 02:38:06 $
 */

// Includes
#include "stdafx.h"
#include "threadit.h"
#include "threaditmessage.h"
#include "icloneable.h"
#include "threaditobserver.h"

/** 
 * Method CThreadItObserver is the constructor for the instance. This is an empty
 * implementation.
 */
CThreadItObserver::CThreadItObserver ()
{
	m_theWorkInstruction = 0;
} // constructor CThreadItObserver

/** 
 * Method CThreadItObserver is the constructor for the instance. 
 * ptheSource specifies the source of the notification. If this is set then
 * the work request to the target will have this information attached. 
 */
CThreadItObserver::CThreadItObserver (const std::shared_ptr<CThreadIt>& ptheSource)
{
	m_wptheSource = ptheSource;
} // constructor CThreadItObserver

/** 
 * Method CThreadItObserver is the constructor for the instance. 
 * ptheThreadIt is the target CThreadIt 
 * theWorkInstruction is the work instruction at the target CThreadIt
 */
CThreadItObserver::CThreadItObserver (const std::shared_ptr<CThreadIt>& ptheThreadIt, UINT theWorkInstruction)
{
	m_theWorkInstruction = theWorkInstruction;
	m_wptheThreadIt = ptheThreadIt;
} // constructor CThreadItObserver
 
/** 
 * Method CThreadItObserver is the constructor for the instance. 
 * ptheSource specifies the source of the notification. If this is set then
 * the work request to the target will have this information attached. 
 * ptheThreadIt is the target CThreadIt 
 * theWorkInstruction is the work instruction at the target CThreadIt
 */
CThreadItObserver::CThreadItObserver (const std::shared_ptr<CThreadIt>& ptheSource, const std::shared_ptr<CThreadIt>& ptheThreadIt, UINT theWorkInstruction)
{
	m_theWorkInstruction = theWorkInstruction;
	m_wptheThreadIt = ptheThreadIt;
	m_wptheSource = ptheSource;
} // constructor CThreadItObserver

/**
 * Method ~CThreadItObserver is the destructor for the instance. This is an empty 
 * implementation.
 */
CThreadItObserver::~CThreadItObserver()
{
} // destructor ~CThreadItObserver

// Does not use the work id
bool CThreadItObserver::operator ==(const CThreadItObserver& theOther) const
{
	bool isEqual = false;

	std::shared_ptr<CThreadIt> sptheThreadIt = m_wptheThreadIt.lock ();
	std::shared_ptr<CThreadIt> sptheThreadItOther = theOther.m_wptheThreadIt.lock ();
	if ((sptheThreadIt) && (sptheThreadItOther))
	{
		if (sptheThreadIt.get () == sptheThreadItOther.get ())
		{
			isEqual = true;
		} // if 
	} // if 
	return isEqual;
} // operator == 

bool CThreadItObserver::getTarget (std::shared_ptr<CThreadIt>& ptheThreadIt, UINT& theWorkInstruction) const
{
	bool isGood = false;

	theWorkInstruction = m_theWorkInstruction;
	ptheThreadIt = m_wptheThreadIt.lock ();
	isGood = true;
	return isGood;
} // getTarget

std::shared_ptr<CThreadIt> CThreadItObserver::getTarget () const
{
	return m_wptheThreadIt.lock ();
} // getTarget

//  returns if the reference held is good.
bool CThreadItObserver::isValid ()
{
	bool isGood = false;
	std::shared_ptr<CThreadIt> thePtr;

	thePtr = m_wptheThreadIt.lock (); 
	if (thePtr)
	{
		isGood = true;
	} // if 
	return isGood;
} // isValid

bool CThreadItObserver::notify ()
{
	bool isSuccess = false;

	std::shared_ptr<CThreadIt> sptheThreadIt = m_wptheThreadIt.lock ();
	if (sptheThreadIt)
	{
		CThreadItMessage aWorkMessage (m_theWorkInstruction);
		aWorkMessage.setSourceInfo(m_wptheSource, 0);
		aWorkMessage.sendWithNoReplyTo (sptheThreadIt.get ());
		isSuccess = true;
	} // if 
	return isSuccess;
} // notify

bool CThreadItObserver::notify (CICloneable& theObject) 
{
	bool isSuccess = false;

	std::shared_ptr<CThreadIt> sptheThreadIt = m_wptheThreadIt.lock ();
	if (sptheThreadIt)
	{
		CThreadItMessage aWorkMessage (m_theWorkInstruction);
		aWorkMessage.getWork ()->m_ptheObject = theObject.getClone ();
		aWorkMessage.setSourceInfo(m_wptheSource, 0);
		aWorkMessage.sendWithNoReplyTo (sptheThreadIt.get ());
		isSuccess = true;
	} // if 
	return isSuccess;
} // notify

bool CThreadItObserver::notify (UINT theWorkInstruction)
{
	bool notify = false;

	std::shared_ptr<CThreadIt> sptheThreadIt = m_wptheThreadIt.lock ();
	if (sptheThreadIt)
	{
		CThreadItMessage aWorkMessage (theWorkInstruction);
		aWorkMessage.setSourceInfo(m_wptheSource, 0);
		aWorkMessage.sendWithNoReplyTo (sptheThreadIt.get ());
		notify = true;
	} // if 
	return notify;
} // notify

bool CThreadItObserver::notify (CICloneable& ptheObject, UINT theWorkInstruction) 
{
	bool notify = false;

	std::shared_ptr<CThreadIt> sptheThreadIt = m_wptheThreadIt.lock ();
	if (sptheThreadIt)
	{
		CThreadItMessage aWorkMessage (theWorkInstruction);
		aWorkMessage.getWork ()->m_ptheObject = ptheObject.getClone ();
		aWorkMessage.setSourceInfo(m_wptheSource, 0);
		aWorkMessage.sendWithNoReplyTo (sptheThreadIt.get ());
		notify = true;
	} // if 
	return notify;
} // notify

