#include "Client.h"
#include "Hlp.h"


/*-----------------------------------------------------------------------*/
/* initialize or load the Winsock library for HTTP 1.1 and initialize the
	endpotin paramters*/
Client::Client(int  Port, string IP)
{
	if (IP.empty()) {

		throw string{ "IP adress is empty." };
	}

	/* initialize or load the Winsock library for HTTP 1.1*/
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		Print("[Error]", "initializing teh WSaData struct");
		return;
	}

	


	/*initialize the strucuture*/
	mAddr.sin_family = AF_INET;
	inet_pton(AF_INET, IP.c_str(), &mAddr.sin_addr); // destiona ip address
	mAddr.sin_port = htons(Port); // destination port


	// the vector mInterSites has not been filled yet.
	mInitilize = false;
}
/*-----------------------------------------------------------------------*/
/* clean up, unload the library*/
Client::~Client()
{
	WSACleanup();
}



void Client::initialize()
{
	int i = 1;
		for (; i < (HowManyTimeToSend); ++i) {
			mConSend.emplace_back(i*10);
		}

		mInitilize = true;
}

int Client::sendIterative(SOCKET  const& sock, int NUmberOfIterations, int sizeOFPackage )
{
	/* define the temporary variable*/
	TLV Data;
	char buf[SizeOfDataPackage];
	int iResult;
	int iter = 0;
	int NUmberOfBytesSended = 0;
	size_t size = 0;




	/*  iterativ send to the server*/
	do {
		++iter;
		size = mConSend.size();
		Data.NumOfPackages = static_cast<WORD>(size);
		Data.WhichPackegeIsIt = static_cast<WORD>(iter);
		Data.Data = mConSend[iter-1];

		memcpy(buf, &Data, SizeOfDataPackage);

		iResult = sendto(sock, buf, sizeOFPackage, 0, reinterpret_cast<sockaddr*>(&mAddr), mSizeOfSockAddress);

		if (iResult == SOCKET_ERROR) { // error handling
			cout << "Error: " << Hlp::ErrMsg(GetLastError()) << endl;
			Print("[Error]", "could not connect to the socket.");
			closesocket(sock);
			return -1;
		}
		Print("[Confirmation]: package " + to_string(iter) +" received.", " Sending package " + to_string(iter) + " of " + to_string(size) + " send");
		NUmberOfBytesSended += iResult;
		
	} while (iter!= (NUmberOfIterations));

	return (NUmberOfBytesSended/ SizeOfDataPackage);
}

/*-----------------------------------------------------------------------*/
//the main algorithm where the  download starts*/
void Client::start()
{
	if (!mInitilize) {
		Print("[Error]", "Call the DefineSource() before download().");
		return;
	}

	/*Create a socket for connecting to server*/
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

	if (sock == INVALID_SOCKET) { // errror handling
		Print("[Error]", "could not create a socket.");
		closesocket(sock);
		return;
	}

	sendIterative(sock, mConSend.size() - 1, SizeOfDataPackage);
	sendIterative(sock, mConSend.size(), SizeOfDataPackage-1);
	sendIterative(sock, mConSend.size(), SizeOfDataPackage);

	Print("\n\n\n[Confirmation]", " All Data has been send");

		closesocket(sock);

}



/*-----------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/*defining a simple print function*/
void Client::Print(string ErrMSg, string Msg)
{
	cout << hr;
	cout << left << setw(15) << setfill(' ') << ErrMSg;
	cout << right << setw(45) << setfill(' ') << Msg << endl;

}
