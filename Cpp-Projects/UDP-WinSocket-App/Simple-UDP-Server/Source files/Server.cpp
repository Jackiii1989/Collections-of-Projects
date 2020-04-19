#include "Server.h"
#include "StopWatch.h"

/*-----------------------------------------------------------------------*/
/* initialize or load the Winsock library for HTTP 1.1*/
Server::Server(int Port, long msec)
{

	/* initialize or load the Winsock library for HTTP 1.1*/
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(1, 1 ), &wsaData) != 0) {
		Print("[Error]: initializing teh WSaData struct.");
		return;
	}
	
	

	mTimeout.tv_sec = msec/1000;
	mTimeout.tv_usec = msec % 1000;

	/*Bind the socket to the endpoint information with the structure*/
	mAddr.sin_family = AF_INET;
	mAddr.sin_addr.s_addr = htonl(INADDR_ANY); // accept incoming packets from any available network
	mAddr.sin_port = htons(Port); // take specified port

	fill_n(back_inserter(mConTLV), cBufSize, cNull);
		
}
/*-----------------------------------------------------------------------*/
/* clean up, unload the library*/
Server::~Server()
{
	WSACleanup();
}





/*-----------------------------------------------------------------------*/
/*the main algorithm where the  download starts*/
void Server::start()
{


		/*Create a socket for connecting to server*/
		SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

		if (sock == INVALID_SOCKET) { // errror handling
			Print("[Error]:could not create a socket.");
			closesocket(sock);
			return;
		}


		if (bind(sock, reinterpret_cast<sockaddr*>(&mAddr), sizeof(sockaddr_in)) == SOCKET_ERROR) {
			Print("[Error]: could bind socket to the struct.");
			closesocket(sock);
			return;
		}

		// Setup timeval variable
		//mConTLV.reserve(cBufSize);
		stw::Start();
		IterativReceivedData(sock);
		Print("[Confirmation]: it took: " + to_string(stw::Stop()));
		closesocket(sock);

}

int Server::IterativReceivedData(SOCKET sock)
{

	// Setup fd_set structure
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(sock, &fds);


	TLV Data;
	char buf[cBufSize];
	
	int iResult;
	int NUmberOfBytesRecieved = 0;
	BYTE Packages;
	int iter = 0;
	ULONG NonBlock = 1;
	ioctlsocket(sock, FIONBIO, &NonBlock);

	Print("[Confirmation]: Server, Waiting for data...");

	do {
		memset(buf,0, cBufSize);
		int retval = select(sock + 1, &fds, NULL, NULL, &mTimeout);
		if (retval == -1) {
			Print("[Error]: could not connect to the socket.");
			return NUmberOfBytesRecieved / SizeOfDataPackage;;
		}
		else if (retval == 0) {
			Print("[Error]: timeout");
			//continue;
			return NUmberOfBytesRecieved / SizeOfDataPackage;
		}
		else {
			iResult = recvfrom(sock, buf, cBufSize, 0, reinterpret_cast<sockaddr*>(&mConInfo), &mSizeOfSockAddress);

			if (iResult == SOCKET_ERROR) { // error handling
				Print("[Error]: could not connect to the socket.");
				closesocket(sock);
				return -1;
			}

			if (iResult != SizeOfDataPackage) {
				stringstream ss;
				
				Data = *reinterpret_cast<TLV*>(buf);
				string result = " Package:" + to_string(Data.WhichPackegeIsIt) + "/" + to_string(Data.NumOfPackages) + " We received "
					+to_string(iResult)+" instead of 8\nBytes:";

				ss << std::hex;// << std::setfill('0');
				for (int i = 0; i < iResult; ++i)
				{
					ss << std::setw(2) << static_cast<unsigned>(buf[i]) << ", ";
				}

				result += ss.str();
				Print("[Error]: the packages has wrong size:" + result);

			/*	ss << "PackagesTotal" << std::setfill(' ')<< static_cast<unsigned>(buf[0])<< static_cast<unsigned>(buf[1])<<endl;
				ss << "Received Packages" << std::setfill(' ') << static_cast<unsigned>(buf[2]) << static_cast<unsigned>(buf[3]) << endl;
				ss << "Data" << std::setfill(' ') << static_cast<unsigned>(buf[2]) << static_cast<unsigned>(buf[3]) << endl;		
				Print("[Error Package size]:\n"+ ss.str());
			*/
				continue;
			}
		
			if (CheckForDuplicates(buf)) {
				continue;
			}
			
			Data = *reinterpret_cast<TLV*>(buf);
			mConTLV[Data.WhichPackegeIsIt] = Data;
			iter++;
			stringstream ss;


			ss << "PackagesTotal: " << std::setfill(' ') << Data.NumOfPackages << endl;
			ss << "Received Packages: " << std::setfill(' ') << Data.WhichPackegeIsIt << endl;
			ss << "Data: " << std::setfill(' ') << Data.Data << endl;

			//string result = " Packages Total: " + to_string(Data.NumOfPackages) + " sReceived Package:" 
			//				+ to_string(Data.WhichPackegeIsIt)+"\n Data:"+ to_string(Data.Data);
			Print("[Confirmation]:\n" + ss.str());


		}
	} while ((iter) != ((cBufSize/ SizeOfDataPackage))*20);

	Print("[Confirmation]All packages has been send.");
	return NUmberOfBytesRecieved / SizeOfDataPackage;
}



/* ConstructRequst:

It Constructs the the request messages and the name of the output file

--> request message type for HTTP 1.1.  :
"GET /Protocols/ HTTP/1.1\r\nHost: www.w3.org\r\n\r\n"

@ param: give the internet site

@return: request message defined by HTTP 1.1.
*/


/*-----------------------------------------------------------------------*/
/*defining a simple print function*/
void Server::Print(string ErrMSg)
{
	cout << hr;
	cout << ErrMSg<<endl;
	//cout << hr;
}

/*-----------------------------------------------------------------------*/
/*defining a simple print function*/
void Server::Print2(string MSg1, string Msg2)
{
	cout << hr;
	cout << left << setw(cWidth/2) << setfill(' ') << MSg1;
	cout << left << setw(cWidth/2) << setfill(' ') << Msg2;
	cout << hr;
}

bool Server::CheckForDuplicates(char buf[cBufSize])
{
	TLV Data = *reinterpret_cast<TLV*>(buf);
	std::stringstream ss;

	if (mConTLV[Data.WhichPackegeIsIt].WhichPackegeIsIt != Data.WhichPackegeIsIt) {
		return false;
	}


	ss << "PackagesTotal: " << std::setfill(' ') << Data.NumOfPackages << endl;
	ss << "Received Packages: " << std::setfill(' ') << Data.WhichPackegeIsIt << endl;
	ss << "Data: " << std::setfill(' ') << Data.Data<< endl;

	//string result = " Packages Total: " + to_string(Data.NumOfPackages) + " sReceived Package:" 
	//				+ to_string(Data.WhichPackegeIsIt)+"\n Data:"+ to_string(Data.Data);
	Print("[Error]: duplicate package\n" + ss.str());

}


