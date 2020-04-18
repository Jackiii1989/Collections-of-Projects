#ifndef ThreadImpl_INCLUDED
#define ThreadImpl_INCLUDED


#include <cassert>
#include<iostream>
#include <sstream>
#include <iomanip>
#include <ostream>
#include <string>

#include <string>
#include <conio.h>

#include <winsock2.h>
#include <Ws2tcpip.h> 

//#include <sys/stat.h>

#include "lock.h"

#include "ThreadBase.h"

using namespace std;

int const cWidth{ 30 };
int const cBufSize{ 512 };

class ThreadImpl : public ThreadBase {
public:
	ThreadImpl();
	virtual ~ThreadImpl();
	virtual void Init(HANDLE tRequest, int Port, long msec, int const &threadPrio = THREAD_PRIORITY_NORMAL);

	// for printing sync on streams-> cout, files, cerr,...
	// he is a friend, a nice friend
	void friend Printsync(std::stringstream  & str, std::ostream &os);
	void Print(std::string ErrMSg);
	void friend Printsync1(std::string  const& str, std::ostream &os);

private:
	int mThreadNum;					// personal thread number
	int mThreadPriority;			// priority of the thread
	int mPort;
	sockaddr_in mAddr;
	sockaddr mConInfo;
	timeval mTimeout;
	bool msInitialized;				// indicates if thread os initialized before the start is called 			
	HANDLE		   mtRequest;		// handle for terminating the the threads
	SOCKET		   mSock;
	fd_set		   mFds;
	std::string mBuf;

	// from experience the run function should be PRIVATE!!!
	virtual int Run() override;
	int ReceivedData();

};


inline std::ostream& hr(std::ostream& out) {

	out << std::left << std::setw(cWidth) << std::setfill('-') << "-";
	out << std::endl;
	return out;
}





#endif // !ThreadImpl_INCLUDED
