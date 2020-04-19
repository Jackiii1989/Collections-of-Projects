#ifndef WEB_D
#define WEB_D

#include <iostream>
#include<sstream>
#include <iomanip>
#include <ostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <winsock2.h>
#include <Ws2tcpip.h> 
#include <string>
#include <sys/stat.h>
#include <algorithm>


using namespace std;

static const int cBufSize{ 51200 };
const int cWidth{ 79 };
typedef uint8_t BYTE;
typedef uint16_t WORD;



struct TLV {
	WORD NumOfPackages;
	WORD WhichPackegeIsIt;
	int Data;
};


const size_t  SizeOfDataPackage{ sizeof(TLV) };
typedef std::string::size_type StringSizeType;
const TLV cNull = { WORD{0},WORD{ 0 },int{ 0 } };

class Server
{
public:
	Server(int Port, long msec= 3000);
	~Server();

	void start();
	int IterativReceivedData(SOCKET sock);

private:

	vector<TLV> mConTLV;
	char mBuf[cBufSize];
	int mPort;
	sockaddr_in mConInfo;
	timeval mTimeout;
	sockaddr_in mAddr; // local server socket

	int mSizeOfSockAddress = sizeof(sockaddr_in);


	/*Private print functions*/
	void Print( string Msg);
	void Print2(string ErrMSg, string Msg);
	bool CheckForDuplicates(char buf[cBufSize]);
	
};


/*-----------------------------------------------------------------------*/
/*defining a simple manipulator for output*/
inline ostream& hr(ostream& out) {

	out << left<<setw(cWidth) << setfill('-')<<"-";
	out << endl;
	return out;
}

#endif // !WEB_D

