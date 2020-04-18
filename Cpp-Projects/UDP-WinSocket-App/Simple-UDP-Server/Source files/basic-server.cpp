/////////////////////////////////////////////////////////////////////////
// basic-server.cpp - Implements a fairly basic single-threaded Winsock 
// 	server program that waits for a connection, accepts it, echoes back
//	any data it receives, and then goes back to listening when the
//	client drops the connection.
/////////////////////////////////////////////////////////////////////////	

#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>  
#include <winbase.h>
#include <cassert>
#include "ws-util.h"
#include "basic-server.h"

using namespace std;


//// private constants /////////////////////////////////////////////////

static size_t const BufSize = 1024;
		

//// DoWinsock /////////////////////////////////////////////////////////
// The module's driver function -- we just call other functions and
// interpret their results.

int DoWinsock(string const & IpAddr, size_t Port)
{
	cout << "Establishing the listener..." << endl;

	// create a socket to listen for incoming connections
	SOCKET ListeningSocket = SetUpListener(IpAddr, htons(Port));

	if (ListeningSocket == INVALID_SOCKET) {
		cout << endl << ErrMsg("establish listener") << endl;
		cout << "error occurred" << endl;
		return 3;
	}

		cout << "Waiting for a connection..." << flush;

		sockaddr_in sinRemote;

		// accepting an incoming connection request from a client
		SOCKET Sock = AcceptConnection(ListeningSocket, sinRemote);

		if (Sock != INVALID_SOCKET) {
			int const n = 50;
			char AdrBuf[n];
			inet_ntop(AF_INET, &sinRemote.sin_addr, AdrBuf, n);
			
			cout << "Accepted connection from "
				 << AdrBuf << "." << endl;			
		}
		else {
			cout << endl << ErrMsg("accept connection") << endl;
			return 3;
		}

		// send the received data back to the client - unmodified
		bool Success = EchoIncomingPackets(Sock);

		// calls to recv and send 
		if (Success) {
			// Successfully bounced all data back to client,
			// so close the connection gracefully.
			cout << "Shutting connection down..." << flush;
			if (ShutdownConnection(Sock)) {
				cout << "Connection is down." << endl;
			}
			else {
				cout << endl << ErrMsg("shutdown connection") << endl;
				return 3;
			}
		}
		else {
			cout << endl << ErrMsg("echo incoming packets") << endl;
			return 3;
		}
	//}

	return 0;
}


//// SetUpListener /////////////////////////////////////////////////////
// Sets up a listener on the given interface and port, returning the
// listening socket if successful; if not, returns INVALID_SOCKET.

SOCKET SetUpListener(string const & IpAddr, size_t Port)
{
	// convert dotted IP-Address into unsigned long value
	u_long InterfaceAddr = 0;
	int ret = inet_pton(AF_INET, IpAddr.c_str(), &InterfaceAddr);
	assert(ret != -1);  // error

	if (InterfaceAddr != INADDR_NONE) {

		// create a stream socket
		SOCKET Sock = socket(AF_INET, SOCK_STREAM, 0);

		if (Sock != INVALID_SOCKET) {
			sockaddr_in Interface;		// structure to specify local connection endpoint

			Interface.sin_family      = AF_INET;		// address family for internet
			Interface.sin_addr.s_addr = InterfaceAddr;	// IP-Address
			Interface.sin_port        = Port;			// port

			// bind the interface to the socket; listen for incoming connections
			if (bind(Sock, reinterpret_cast<sockaddr *>(&Interface), sizeof(sockaddr_in)) != SOCKET_ERROR) {
				listen(Sock, SOMAXCONN);
				return Sock;
			}
		}
	}

	return INVALID_SOCKET;
}



//// AcceptConnection //////////////////////////////////////////////////
// Waits for a connection on the given socket.  When one comes in, we
// return a socket for it.  If an error occurs, we return 
// INVALID_SOCKET.

SOCKET AcceptConnection(SOCKET ListeningSocket, sockaddr_in & Remote)
{
	int AddrSize = sizeof(Remote);
	cout << " accepting connection..." << endl ;
	return accept(ListeningSocket, reinterpret_cast<sockaddr*> (&Remote), &AddrSize);
}



//// EchoIncomingPackets ///////////////////////////////////////////////
// Bounces any incoming packets back to the client.  We return false
// on errors, or true if the client closed the socket normally.

bool EchoIncomingPackets(SOCKET Sock)
{
	// Read data from client
	char   ReadBuffer[BufSize];
	size_t ReadBytes;

	//do {
		// receive data: recv blocks....
		ReadBytes = recv(Sock, ReadBuffer, BufSize, 0);
		if (ReadBytes > 0) {
			ReadBuffer[ReadBytes] = 0;
			cout << "Received " << ReadBytes << " bytes from client: " << endl;
			cout << "  --> Msg: " << ReadBuffer << endl;
					
			size_t SentBytes = 0;

			// we must send iteratively
			while (SentBytes < ReadBytes) {

				// send data back
				int Cnt = send(Sock, ReadBuffer + SentBytes, ReadBytes - SentBytes, 0);
				if (Cnt > 0) {
					cout << "Sent " << Cnt << " bytes back to client." << endl;
					SentBytes += Cnt;
				}
				else if (Cnt == SOCKET_ERROR) {
					return false;
				}
				else {
					// Client closed connection before we could reply to
					// all the data it sent, so bomb out early.
					cout << "Peer unexpectedly dropped connection!" << endl;
					return true;
				}
			}
		}
		else if (ReadBytes == SOCKET_ERROR) {
			return false;
		}
	//} while (1);
	
	cout << "Connection closed by peer." << endl;
	return true;
}


//// ShutdownConnection ////////////////////////////////////////////////
// Gracefully shuts the connection sd down.  Returns true if we're
// successful, false otherwise.

bool ShutdownConnection(SOCKET Sock)
{
	// Disallow any further data sends.  This will tell the other side
	// that we want to go away now.  If we skip this step, we don't
	// shut the connection down nicely.

	if (shutdown(Sock, SD_SEND) == SOCKET_ERROR) {
		return false;
	}

	// Receive any extra data still sitting on the socket.  After all
	// data is received, this call will block until the remote host
	// acknowledges the TCP control packet sent by the shutdown above.
	// Then we'll get a 0 back from recv, signalling that the remote
	// host has closed its side of the connection.

	char ReadBuf[BufSize];
	//while (true) {
		int Bytes = recv(Sock, ReadBuf, BufSize, 0);
		if (Bytes == SOCKET_ERROR) {
			return false;
		}
		else if (Bytes != 0) {
			cerr << endl << "Received " << Bytes
			     << " unexpected bytes during shutdown." << endl;
		}
		//else {
		//	// Okay, we're done!
		//	break;
		//}


		Sleep(30000);
	// Close the socket.
	if (closesocket(Sock) == SOCKET_ERROR) {
		return false;
	}



	return true;
}

