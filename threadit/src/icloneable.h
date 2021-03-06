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

/**
 * Title: CICloneable
 * Description: class CICloneable is the virtual base class for a class that wishes to receive 
 * notifications when information of interest updates or changes. This is an 
 * implementation of the Observer pattern and allows a class to be notified of 
 * change asynchronously. Classes declare an interest by becoming observers of a subject. 
 * When data related to the subject changes the class method is invoked to notify 
 * the observers of the change or update. See class CSubject as the partner of the 
 * CICloneable class in the implementation of this observer pattern.
 *
 * Copyright: Copyright (c) 2008 Ashkel Software 
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2006/12/19 02:38:06 $
 */

#ifndef I_CLONEABLE
#define I_CLONEABLE

/**
 * Class CICloneable is used as the base class and implements the two virtual
 * methods in order to be notified when data/information of interest is
 * being observed. The inheriting class registers its interest by 
 * by attaching to a subject. When the subject or information relating to
 * the subject changes the onUpdate or onChange methods are invoked asynchronously.
 */
class CICloneable
{
	// Methods
	public :
		/**
			// Do something about loosely held memory. This should be a shared pointer or weakref.
		 */
		virtual CICloneable* getClone () = 0;

}; // class CICloneable

#endif // // #ifndef I_CLONEABLE
