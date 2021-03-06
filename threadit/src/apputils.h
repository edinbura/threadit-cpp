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
 * Title: CAppUtils
 * Description: Class CAppUtils
 * Returns the executable directory where the application was invoked.
 * Copyright: Copyright (c) 2008 Ashkel Software
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2006/12/19 02:38:06 $
 */

#if !defined (APP_UTILS_H)
#define APP_UTILS_H


#include "stdafx.h"
#include <log4cpp/Category.hh>
#include <log4cpp/BasicConfigurator.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include "protectedqueue.h"

class CAppUtils
{
	// Attributes
protected:
	log4cpp::Category* m_ptheLogger;


	// Services
public: 
	CAppUtils::CAppUtils ()
	{
		m_ptheLogger = &(log4cpp::Category::getInstance ("CAppUtils"));
	} // constructor CCallback

	CAppUtils::~CAppUtils ()
	{
	} // constructor CCallback

	static std::string getAppPath ()
	{
		TCHAR thePath[MAX_PATH];
		std::string aFilePath;
		std::string aFileName;
		int theCount = 0;
		size_t thePos = 0;

		theCount = GetModuleFileName (NULL, thePath, sizeof (thePath));
		if (theCount > 0)
		{
			aFilePath.assign(&thePath[0], &thePath[theCount]);
			thePos = aFilePath.rfind ("\\");
			if (thePos != std::string::npos)
			{
				aFilePath = aFilePath.substr (0, thePos);
				aFilePath = aFilePath + "\\";
			} // if 
		} // if 
		return aFilePath;
	} // getAppPath

}; // class CAppUtils

#endif //  !defined (APP_UTILS_H)