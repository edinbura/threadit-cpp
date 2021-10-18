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

#ifndef THREADIT_OBSERVER
#define THREADIT_OBSERVER

#include <windows.h>
#include <memory>

// forward declarations
class CThreadIt;
class CICloneable;

/**
 * Class CThreadItObserver is used as the base class and implements the two virtual
 * methods in order to be notified when data/information of interest is
 * being observed. The inheriting class registers its interest by 
 * by attaching to a subject. When the subject or information relating to
 * the subject changes the onUpdate or onChange methods are invoked asynchronously.
 */
class CThreadItObserver
{
	// Protected member variables
	protected :
		/** m_theWorkInstruction is the work instruction at the target. */
		UINT m_theWorkInstruction;
		/** m_wptheThreadIt is the target for the notification. */ 
		std::weak_ptr<CThreadIt> m_wptheThreadIt;
		/** m_wptheSource is the source - the initiator of the notification. */ 
		std::weak_ptr<CThreadIt> m_wptheSource;

	public:
		/** 
  	 * Method CThreadItObserver is the constructor for the instance. This is an empty
	   * implementation.
		 */
		CThreadItObserver ();
		/** 
  	 * Method CThreadItObserver is the constructor for the instance. 
		 * ptheSource specifies the source of the notification. If this is set then
		 * the work request to the target will have this information attached. 
		 */
		CThreadItObserver (const std::shared_ptr<CThreadIt>& ptheSource);
		/** 
		 * Method CThreadItObserver is the constructor for the instance. 
		 * ptheThreadIt is the target CThreadIt 
		 * theWorkInstruction is the work instruction at the target CThreadIt
		 */
		CThreadItObserver (const std::shared_ptr<CThreadIt>& ptheThreadIt, UINT theWorkInstruction);
		/** 
		 * Method CThreadItObserver is the constructor for the instance. 
		 * ptheSource specifies the source of the notification. If this is set then
		 * the work request to the target will have this information attached. 
		 * ptheThreadIt is the target CThreadIt 
		 * theWorkInstruction is the work instruction at the target CThreadIt
		 */
		CThreadItObserver (const std::shared_ptr<CThreadIt>& ptheSource, const std::shared_ptr<CThreadIt>& ptheThreadIt, UINT theWorkInstruction);


		// Destructor
	public :
		/**
		* Method ~CThreadItObserver is the destructor for the instance. This is an empty 
		* implementation.
		*/
		virtual ~CThreadItObserver ();

	// Methods
	public :
		/**
			// Do something about loosely held memory. This should be a shared pointer or weakref.
		 */
		bool getTarget (std::shared_ptr<CThreadIt>& ptheThreadIt, UINT& theWorkInstruction) const;
		std::shared_ptr<CThreadIt> getTarget () const;
		bool notify ();
		bool notify (CICloneable& ptheObject);
		bool notify (UINT theWorkInstruction);
		bool notify (CICloneable& ptheObject, UINT theWorkInstruction);
	  bool operator== (const CThreadItObserver& theOther) const;
		bool isValid (); //  returns if the reference held is good.

}; // class CThreadItObserver

#endif // // #ifndef THREADIT_OBSERVER
