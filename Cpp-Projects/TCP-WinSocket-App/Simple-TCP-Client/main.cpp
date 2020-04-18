//#include <Windows.h>
#include "ThreadImp.h"

const std::string Finish = "Terminate";
 CritSec  gCsAcces;
int main(){


	HANDLE			tRequest;
	ThreadImpl*		pThreads;
	HANDLE			hThreads;

	tRequest = CreateEvent(
		0,								// security attributes(NULL-> child can not inherited by child)
		true,							// true: create an manual-reset event object to nonsignaled after a thread has ben created
		false,							// false: the object is not signaled, otherwise it is
		Finish.c_str()		           // name of the object -> no name
	);

	string data;

	pThreads = new ThreadImpl();
	hThreads = pThreads->GetDuplicateHdl();
	pThreads->Init(tRequest,1234,3000);
	pThreads->Start();

	do {	
		{
			CSLock Lock{ gCsAcces };
			//Printsync1(, cout);
			cout << "To continue press '.' and enter\n to terminate press 't.' and enter: ";
			getline(cin, data, '.');
		}
			cin.ignore(1); // removes '.' from stream cin
		
		Sleep(100);

	} while (data.compare("t") != 0); 

	SetEvent(tRequest);

	// wait for all threads and after close them
	WaitForMultipleObjects(1,&hThreads,true, INFINITE);
	CloseHandle(hThreads);
	CloseHandle(tRequest);

	return 0;
}