#ifndef WEB_D
#define WEB_D

#include <iostream>
#include <iomanip>
#include <ostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <winsock2.h>
#include <Ws2tcpip.h> 
#include <string>
#include <sys/stat.h>

const int cBufSize{ 512 };

typedef uint8_t BYTE;
typedef uint16_t WORD;

struct TLV {
	WORD NumOfPackages;
	WORD WhichPackegeIsIt;
	int Data;
};

typedef struct CharPack { char x[cBufSize]; } CharPack;


const size_t  SizeOfDataPackage{ sizeof(TLV) };
const WORD HowManyTimeToSend{ (cBufSize / SizeOfDataPackage)*20 };
using namespace std;

typedef std::string::size_type StringSizeType;

class Client
{
public:
	Client(int Port = 1234, string IP = "127.0.0.1");
	~Client();

	void start();
	void initialize();
	int sendIterative(SOCKET  const& sock, int NUmberOfIterations, int sizeOFPackage);

private:
	bool mInitilize;
	vector<int> mConSend;
	char mBuf[cBufSize]; 
	string mIP;			 // IP address
	u_short mPort;		 // local port 
	sockaddr_in mAddr;  // local server socket


	int mSizeOfSockAddress = sizeof(sockaddr_in);


	/*Private print functions*/
	void Print(string ErrMSg, string Msg);

};


/*-----------------------------------------------------------------------*/
/*defining a simple manipulator for output*/
inline ostream& hr(ostream& out) {

	out << "------------------------------------------------------------";
	out << endl;
	return out;
}

#endif // !WEB_D

