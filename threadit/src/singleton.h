
#ifndef SINGLETON
#define SINGLETON

#include "windows.h"
#include <log4cpp/Category.hh>

class CSingleton
{
protected:
	static CSingleton* _instance;
	static int _refCount;
	static HANDLE _mutex;
	/** m_ptheLogger is the logger used to log information and errors for each instance of this class */
	static log4cpp::Category* m_ptheLogger;

 
public:
	static CSingleton* Instance();
	static void Release();
	static void Destroy();
	~CSingleton();

	void method();
 
protected:
	CSingleton();
};


#endif // SINGLETON
