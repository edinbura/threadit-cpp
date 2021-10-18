// SingletonTemplate.cpp : Defines the entry point for the console application.
//
// Copyright (C) 2002	 Paul D. Evans - All rights reserved.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	 02111-1307	 USA
//
//////////////////////////////////////////////////////////////////////////////
//
// Author:		Paul D. Evans
//
// E-Mail comments / suggestions to: paul@enjoysoftware.co.uk
//		It would be nice to hear of any positive uses of this code!
//
// Homepage:	http://www.enjoysoftware.co.uk
//
// Started:		26Nov02
//
// Purpose:		Test stub
//		
/////////////////////////////////////////////////////////////////////////////
//
// Notes:
//
//	 Test stub that hopefully(!) proves that these Singletons are of a well-
// bred Thread Safe variety!
//
/////////////////////////////////////////////////////////////////////////////
//
// History:
//		v1 - Created
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "SimpleTimer.h"
//#include "CSingleton.h"
#include "SingletonExample.h"
#include "SingletonObserver.h"

#include <windows.h>
#include <process.h>
#include <stdio.h>

using namespace PDE;
using namespace std;

void Test()
{
	CSimpleTimer timer;
	
	timer.start();
	for(int i = 0; i < 10000000; ++i)
		CSingletonExample::getInstance();

	timer.stop();

	printf("Profiling of ThreadID %d took %f seconds\n",
				 ::GetCurrentThreadId(), timer.getElapsed());
}



void Thread2(void*)
{
	printf("Thread2 ThreadID is %d\n", ::GetCurrentThreadId());

	CSingletonExample* pCSingleton = CSingletonExample::getInstance();
	

	if (pCSingleton)
	{
		pCSingleton->setTestInt(2);
		CSingletonExample::getInstance()->debugShow();
	}
	else
	{
		printf("Thread2 CSingleton::Instance() was NULL\n");
	}

	Test();

	printf("Thread2 exiting...\n");
	//_endthread();
}



int main(int argc, char* argv[])
{
	printf("Main ThreadID=%d\n", ::GetCurrentThreadId());
		
	HANDLE hThread = (HANDLE) _beginthread(Thread2, 0, NULL);

	printf("MainThread - Wait for Thread2...\n");
	// Wait for thread.
	WaitForSingleObject(hThread, /*500*/INFINITE);
	printf("... MainThread - Finished waiting for Thread2...\n");

	CSingletonExample* pCSingleton = CSingletonExample::getInstance();

	if (pCSingleton)
	{
		pCSingleton->setTestInt(1);
		CSingletonExample::getInstance()->debugShow();
	}
	else
	{
		printf("MainThread - CSingletonExample::Instance() was NULL\n");
	}

	Test(); 


	//The Observer must be called to delete the memory taken up by the Singleton
	CSingletonObserver::getInstance()->removeAllAndDie();
	return 0;
}

