////////////////////////////////////////////////////////////////////////////
// main.cpp 
// Windows Socket Server
// This function parses the command line, starts up Winsock, 
// and calls an external function 'DoWinsock()' to do the actual work.
////////////////////////////////////////////////////////////////////////////

// Note: add ws2_32.lib to your Project-Settings!


#include <iostream>
#include <winsock2.h>
#include "basic-server.h"


using namespace std;


//// Constants /////////////////////////////////////////////////////////

// Default port to connect to on the server
// const int kDefaultServerPort = 4242;


//// main //////////////////////////////////////////////////////////////

int main(int argc, char * argv[])
{


	// set IP-Address and port
	string IpAddr = "127.0.0.1";
	size_t Port   = 1234;

	// Start Winsock up
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        return 255;
    }

	// Do the actual connection stuff
    int Ret = DoWinsock(IpAddr, Port);

	// Shut Winsock back down and take off
    WSACleanup();

	cout << "Server has been shutdown.\n" << endl;
    return Ret;
}

