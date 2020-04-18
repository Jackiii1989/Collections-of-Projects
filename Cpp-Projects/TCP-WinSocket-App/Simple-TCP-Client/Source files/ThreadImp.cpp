#include"ThreadImp.h"

 extern CritSec  gCsAcces;		// Critical section for Snyc output


ThreadImpl::ThreadImpl() {
	assert(ThreadBase::IsThreadCreated());
	msInitialized = false; 
	// fetch the initial priority of the thread and check if it id valid
	mThreadPriority = GetThreadPriority(GetThreadHdl());
	assert(mThreadPriority != THREAD_PRIORITY_ERROR_RETURN);

	/* initialize or load the Winsock library for HTTP 1.1*/
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		std::string a = "[Error]: initializing teh WSaData struct.";
		Printsync1(a, std::cout);
		return;
	}

}

ThreadImpl::~ThreadImpl() {
	WSACleanup();
}

int ThreadImpl::Run() {
	// take care that this thread has been fully initialized previously
	assert(msInitialized);
	if (!msInitialized)
	{
		return 0;
	}

	/*Create a socket for connecting to server*/
	mSock = socket(AF_INET, SOCK_STREAM, 0);

	if (mSock == INVALID_SOCKET) { // errror handling
		std::string a = "[Error]:could not create a socket.";
		Printsync1(a, std::cout);
		closesocket(mSock);
		return -1;
	}

 	int ret = connect(mSock, reinterpret_cast<sockaddr*>(&mAddr), sizeof(sockaddr_in));

	if (ret == INVALID_SOCKET) { // errror handling
		std::string a = "[Error]:Invalied socket.";
		Printsync1(a, std::cout);
		closesocket(mSock);
		return -2;
	}

	int NUmberOfBytesRecieved = 0;
	int iter = 0;
	int receiveMessage = 2;

	// while the state of mtRequest is not signaled?
	while (WaitForSingleObject(mtRequest, 0) != WAIT_OBJECT_0) {
		
		if (receiveMessage == 2) {
			mBuf = "Hei whats up";
			int sentBytes = send(mSock, mBuf.c_str(), mBuf.size(), 0);

			std::string sendBuffer{ mBuf };
			Printsync1("\n[Confirmation]Client sent: " + to_string(mBuf.size()) + "bytes: " + sendBuffer + "\n", std::cout);
		}
		receiveMessage = ReceivedData();
		if (receiveMessage < 0) {
			closesocket(mSock);
			return -3;
		}
	
		Sleep(10);
	}
	
	// ---- end of the person section
	cout << "." << endl;
	std::string a = "\nshuting down the application...\n";
	Printsync1(a, std::cout);
	delete this; // remove yourself ->deleting this pointer, data will be deleted

	return 0;
}

int ThreadImpl::ReceivedData()
{	
		// Setup fd_set structure
		FD_ZERO(&mFds);
		FD_SET(mSock, &mFds);
		ULONG NonBlock = 1;
		ioctlsocket(mSock, FIONBIO, &NonBlock);
		
		int iResult;

			int retval = select(mSock + 1, &mFds, NULL, NULL, &mTimeout);
			if (retval == -1) {
				std::string a = "[Error]: could not connect to the socket.";
				Printsync1(a, std::cout);
			}
			else if (retval == 0) {

				std::string a = "[Error]: timeout";
				Printsync1(a, std::cout);
				//continue
			}
			else {
				char Buf[cBufSize];
				memset(Buf,0, cBufSize),
				iResult = recv(mSock, Buf, cBufSize, 0);

				if (iResult == SOCKET_ERROR) { // error handling
					std::string a = "[Error]: could not connect to the socket.";
					Printsync1(a, std::cout);
					closesocket(mSock);
					return -1;
				}


				if (iResult == 0) { // error handling
					std::string a = "[Confirmation]: Server, Waiting for data...\n";
					Printsync1(a, std::cout);
					//closesocket(mSock);
					return 1;
				}

				std::string sendBuffer{ Buf };
				Printsync1("Recieved bytes from server: " + to_string(iResult) + " \nMessage: " + sendBuffer, std::cout);

				return 2;
			}
}

// general initialization function
void ThreadImpl::Init(HANDLE tRequest, int Port, long msec, int const &threadprio) {

	mThreadPriority = threadprio;
	SetThreadPriority(GetThreadHdl(), mThreadPriority);

	mtRequest = tRequest;
	mTimeout.tv_sec = msec / 1000;
	mTimeout.tv_usec = msec % 1000;

	/*Bind the socket to the endpoint information with the structure*/
	mAddr.sin_family = AF_INET;
	//mAddr.sin_addr.s_addr = htonl(INADDR_ANY); // accept incoming packets from any available network
	mAddr.sin_port = htons(Port); // take specified port

	inet_pton(AF_INET, "127.0.0.1", &mAddr.sin_addr);
	//fill_n(back_inserter(mConTLV), cBufSize, cNull);
	msInitialized = true; //  data is initialized
}

//defining function outside of the class

void Printsync(std::stringstream  & str, std::ostream &os) {

	CSLock Lock{ gCsAcces };
	os << str.str();
	// avoid invoking the Ctor that takes const char* (Compilers may identify that)
	str.str(std::string());			
}

void Printsync1(std::string  const& str, std::ostream &os) {

	CSLock Lock{ gCsAcces };
	os << str;
}


void ThreadImpl::Print(std::string ErrMSg)
{
	std::cout << hr;
	std::cout << ErrMSg << std::endl;
	//cout << hr;
}