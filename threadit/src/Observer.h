/**
 * Title: CObserver
 * Description: class CObserver is the virtual base class for a class that wishes to receive 
 * notifications when information of interest updates or changes. This is an 
 * implementation of the Observer pattern and allows a class to be notified of 
 * change asynchronously. Classes declare an interest by becoming observers of a subject. 
 * When data related to the subject changes the class method is invoked to notify 
 * the observers of the change or update. See class CSubject as the partner of the 
 * CObserver class in the implementation of this observer pattern.
 *
 * Copyright: Copyright (c) 2008 Ashkel Software 
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2006/12/19 02:38:06 $
 */

#ifndef _OBSERVER
#define _OBSERVER

// Forward declare the class CSubject
class CSubject;

/**
 * Class CObserver is used as the base class and implements the two virtual
 * methods in order to be notified when data/information of interest is
 * being observed. The inheriting class registers its interest by 
 * by attaching to a subject. When the subject or information relating to
 * the subject changes the onUpdate or onChange methods are invoked asynchronously.
 */
class CObserver
{
	// Protected constructor
protected :
	/** 
	 * Method CObserver is the constructor for the instance. This is an empty
	 * implementation.
	 */
	CObserver ();

	// Destructor
public :
	/**
	 * Method ~CObserver is the destructor for the instance. This is an empty 
	 * implementation.
	 */
	virtual ~CObserver ();

	// Methods
public :
	/**
	 * Method onUpdate must be implemented by the observer in order to be notified when the
	 * subject of interest is updated. Once the instance has registered interest in the subject
	 * then when the subject is updated this method is used to notify the observer of the update
	 * to the subject of interest.
	 * The method is called asynchronously and hence may be invoked from one or more threads. 
	 * ptheSubject is a pointer to the instance of the subject of interest that identfies the 
	 * update that has taken place. 
	 * The owner of ptheSubject is usually the supplier as only the supplier knows when
	 * all observers have utilised the instance. This means that the reference is only valid
	 * for the lifetime of the onUpdate method invocation. In this case ptheSubject is usally the
	 * element that has changed and is always the same object instance for this observer.
	 * NOTE: as this method is called from within a critical section no calls to attach or detach 
	 * should be made on the CSubject instance.
	 */
	virtual bool onUpdate (const CSubject& theSubject) = 0;

  /**
     A class that wraps a void pointer, but explictly makes it clear
     that ownership of the referred object is not passed to the
     underlying routine. We use this, because void& is a syntax
     error */
  class VoidRef
  {
    void *m_ref;
  public:
    VoidRef (VoidRef& x): m_ref(x.m_ref) {}
    template <class T> VoidRef(T& ref): m_ref(&ref) {}
    template <class T> T& cast() {return *reinterpret_cast<T*>(m_ref);}
  };

	/**
	 * Method onChange must be implemented by the observer in order to be notified when the
	 * information of interest is updated. Once the instance has registered interest then
	 * then when the information is updated this method is used to notify the observer of the change.
	 * The method is called asynchronously and hence may be invoked from one or more threads. 
	 * ptheSubject is a pointer to the instance that describes the change that has taken place.
	 * The owner of ptheSubject is usually the supplier as only the supplier knows when
	 * all observers have utilised the instance. This means that the reference is only valid
	 * for the lifetime of the OnChange method invocation, and should not be deleted within the implementation.
	 * NOTE: as this method is called from within a critical section no calls to attach or detach 
	 * should be made on the CSubject instance.
	 */
	virtual bool onChange (VoidRef ptheSubject) = 0;

}; // class CObserver

#endif // // #ifndef _OBSERVER
