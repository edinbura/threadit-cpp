
#ifndef THREADITWORKTARGETS_H
#define THREADITWORKTARGETS_H

#include <iostream>
#include <map>
#include <string>
#include "threadit.h"
using namespace std;

/**
 * class CThreadItWorkTargets extends class CSubject. The class allows a value to be incremented
 * and the total number of increments to be counted. In this way the behaviour of the
 * observer can be evaluated.
 */
class CThreadItWorkTargets
{
public:
	struct WorkTargetType
	{
		ULONG m_theSelectWorkInstruction;
		ULONG m_theDestWorkInstruction;
		CThreadIt* m_ptheDestination;
	} WorkTarget;

protected:	
	typedef map<ULONG, WorkTargetType*> WorkTargetMap;
	WorkTargetMap m_theWorkTargets;

public: 
	CThreadItWorkTargets ()
	{
	} // constructor CThreadItWorkTargets

	CThreadItWorkTargets (ULONG theSelectorInstruction, ULONG theDestinationInstruction, CThreadIt* ptheDestination)
	{
		WorkTargetType*	 pWorkTarget = new WorkTargetType ();
		pWorkTarget->m_theSelectWorkInstruction = theSelectorInstruction;
		pWorkTarget->m_theDestWorkInstruction = theDestinationInstruction;
		pWorkTarget->m_ptheDestination = ptheDestination;
		m_theWorkTargets[theSelectorInstruction] = pWorkTarget;
	} // constructor CThreadItWorkTargets

	~CThreadItWorkTargets ()
	{
		// Free all used resources before the instance can release.
		reset ();
	} // destructor ~CMyObserver

	void setWorkTarget (ULONG theSelectorInstruction, ULONG theDestinationInstruction, CThreadIt* ptheDestination)
	{
		WorkTargetType*	 pWorkTarget = NULL;
		
		pWorkTarget =	 m_theWorkTargets[theSelectorInstruction];
		if (pWorkTarget != NULL)
		{
			delete pWorkTarget;
		} // if 
		pWorkTarget = new WorkTargetType ();
		pWorkTarget->m_theSelectWorkInstruction = theSelectorInstruction;
		pWorkTarget->m_theDestWorkInstruction = theDestinationInstruction;
		pWorkTarget->m_ptheDestination = ptheDestination;
		m_theWorkTargets[theSelectorInstruction] = pWorkTarget;
	} // setWorkTarget

	void setWorkTarget (WorkTargetType theWorkTarget)
	{
		WorkTargetType*	 ptheWorkTarget = NULL;
		
		ptheWorkTarget =	m_theWorkTargets[theWorkTarget.m_theSelectWorkInstruction];
		if (ptheWorkTarget != NULL)
		{
			delete ptheWorkTarget;
		} // if 
		ptheWorkTarget = new WorkTargetType ();
		*ptheWorkTarget = theWorkTarget;
		m_theWorkTargets[theWorkTarget.m_theSelectWorkInstruction] = ptheWorkTarget;
	} // setWorkTarget

	void getWorkTarget (ULONG theSelectorInstruction, ULONG& theDestinationInstruction, CThreadIt*& ptheDestination)
	{
		WorkTargetType*	 pWorkTarget = NULL;
		
		pWorkTarget =	 m_theWorkTargets[theSelectorInstruction];
		if (pWorkTarget != NULL)
		{
			theDestinationInstruction = pWorkTarget->m_theDestWorkInstruction;
			ptheDestination = pWorkTarget->m_ptheDestination;
		} // if 
	} // getWorkTarget

	WorkTargetType* getWorkTarget (ULONG theSelectorInstruction)
	{
		WorkTargetType* pWorkTarget = NULL;
		WorkTargetType* pWorkTargetResult = NULL;
		
		pWorkTarget =	 m_theWorkTargets[theSelectorInstruction]; 
		if (pWorkTarget != NULL)
		{
			pWorkTargetResult = new WorkTargetType ();
			*pWorkTargetResult = *pWorkTarget;
		} // if 
		return pWorkTargetResult;
	} // getWorkTarget

		/**
		 * Method operator+ supports the concatenation of two data buffers.
		 * The data sequence of the second buffer is added to the data
		 * sequence of the first buffer.
		 * theBuffer is the data buffer whose bytes are to be added.
		 * The method returns the combination of the byte sequence in the
		 * two buffers.
		 */
	CThreadItWorkTargets& operator+ (CThreadItWorkTargets &theWorkTargets)
	{
		WorkTargetType* aWorkTarget = NULL;
		CThreadItWorkTargets* ptheTargets = new CThreadItWorkTargets ();

		// Copy the bytes in the current buffer to the new buffer.
		if (!m_theWorkTargets.empty ())
		{
			for (WorkTargetMap::iterator theIter = m_theWorkTargets.begin(); theIter != m_theWorkTargets.end(); theIter++)
			{
				aWorkTarget = m_theWorkTargets[theIter->first];
				if (aWorkTarget != NULL)
				{
					ptheTargets->setWorkTarget (*aWorkTarget);
				} // if 
			} // for
		} // if 
			// Add the bytes from the supplied buffer into the new buffer.
		if (!theWorkTargets.m_theWorkTargets.empty ())
		{
			for (WorkTargetMap::iterator theIter = theWorkTargets.m_theWorkTargets.begin(); theIter != theWorkTargets.m_theWorkTargets.end(); theIter++)
			{
				aWorkTarget = theWorkTargets.m_theWorkTargets[theIter->first];
				if (aWorkTarget != NULL)
				{
					ptheTargets->setWorkTarget (*aWorkTarget);
				} // if 
			} // for
		} // if 
			// Return the concatenated work targets.
		return *ptheTargets;
	} // operator+

		/**
		 * Method operator= supports the assignment of the specified data
		 * buffer to the target data buffer. All data bytes associated with the
		 * target data buffer will be lost.
		 * theBuffer is the data buffer whose bytes are to be assigned to the
		 * target buffer.
		 */
	CThreadItWorkTargets& operator= (CThreadItWorkTargets &theWorkTargets)
		{
			WorkTargetType* aWorkTarget = NULL;

			// Clear all the data from the target buffer.
			this->m_theWorkTargets.clear ();
			// Copy all the data from the supplied buffer into the target buffer.
			if (!theWorkTargets.m_theWorkTargets.empty ())
			{
				for (WorkTargetMap::iterator theIter = theWorkTargets.m_theWorkTargets.begin(); theIter != theWorkTargets.m_theWorkTargets.end(); theIter++)
				{
					aWorkTarget = theWorkTargets.m_theWorkTargets[theIter->first];
					if (aWorkTarget != NULL)
					{
						this->setWorkTarget (*aWorkTarget);
					} // if 
				} // for
			} // if 
			return *this;
		} // operator=

	/**
	 * Method size is called to return the number of bytes currently
	 * contained in the data buffer.
	 */

	UINT size ()
	{
		return (UINT)m_theWorkTargets.size ();
	} // size

	/**
	 * Method reset is called to clear the data buffer of all elements. This allows
	 * the data buffer to be re-used without having to be re-created.
	 */
	void reset ()
	{
		WorkTargetType* pWorkTarget = NULL;

		if (!m_theWorkTargets.empty ())
		{
			for (WorkTargetMap::iterator theIter = m_theWorkTargets.begin(); theIter != m_theWorkTargets.end(); theIter++)
			{
				pWorkTarget = m_theWorkTargets[theIter->first];
				if (pWorkTarget != NULL)
				{
					delete pWorkTarget;
				} // if 
			} // for
			m_theWorkTargets.clear ();
		} // if 
	} // reset;

}; // class CThreadItWorkTargets

#endif // #ifndef THREADITWORKTARGETS_H

