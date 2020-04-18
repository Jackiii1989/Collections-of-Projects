///////////////////////////////////////////////////////////////////////
// Socket error helper (implementation)
// SEN4, Hagenberg
///////////////////////////////////////////////////////////////////////


#include <strstream>	
#include <winsock2.h>
#include <iostream>

using namespace std;
		

//// Statics ///////////////////////////////////////////////////////////

static struct ErrorEntry {
	int nID;
	char const * pcMessage;
} gaErrorList[] = {
	{ 0,                     "No error" },
	{ WSAEINTR,              "Interrupted system call" },
	{ WSAEBADF,              "Bad file number" },
	{ WSAEACCES,             "Permission denied" },
	{ WSAEFAULT,             "Bad address" },
	{ WSAEINVAL,             "Invalid argument" },
	{ WSAEMFILE,             "Too many open sockets" },
	{ WSAEWOULDBLOCK,        "Operation would block" },
	{ WSAEINPROGRESS,        "Operation now in progress" },
	{ WSAEALREADY,           "Operation already in progress" },
	{ WSAENOTSOCK,           "Socket operation on non-socket" },
	{ WSAEDESTADDRREQ,       "Destination address required" },
	{ WSAEMSGSIZE,           "Message too long" },
	{ WSAEPROTOTYPE,         "Protocol wrong type for socket" },
	{ WSAENOPROTOOPT,        "Bad protocol option" },
	{ WSAEPROTONOSUPPORT,    "Protocol not supported" },
	{ WSAESOCKTNOSUPPORT,    "Socket type not supported" },
	{ WSAEOPNOTSUPP,         "Operation not supported on socket" },
	{ WSAEPFNOSUPPORT,       "Protocol family not supported" },
	{ WSAEAFNOSUPPORT,       "Address family not supported" },
	{ WSAEADDRINUSE,         "Address already in use" },
	{ WSAEADDRNOTAVAIL,      "Can't assign requested address" },
	{ WSAENETDOWN,           "Network is down" },
	{ WSAENETUNREACH,        "Network is unreachable" },
	{ WSAENETRESET,          "Net connection reset" },
	{ WSAECONNABORTED,       "Software caused connection abort" },
	{ WSAECONNRESET,         "Connection reset by peer" },
	{ WSAENOBUFS,            "No buffer space available" },
	{ WSAEISCONN,            "Socket is already connected" },
	{ WSAENOTCONN,           "Socket is not connected" },
	{ WSAESHUTDOWN,          "Can't send after socket shutdown" },
	{ WSAETOOMANYREFS,       "Too many references, can't splice" },
	{ WSAETIMEDOUT,          "Connection timed out" },
	{ WSAECONNREFUSED,       "Connection refused" },
	{ WSAELOOP,              "Too many levels of symbolic links" },
	{ WSAENAMETOOLONG,       "File name too long" },
	{ WSAEHOSTDOWN,          "Host is down" },
	{ WSAEHOSTUNREACH,       "No route to host" },
	{ WSAENOTEMPTY,          "Directory not empty" },
	{ WSAEPROCLIM,           "Too many processes" },
	{ WSAEUSERS,             "Too many users" },
	{ WSAEDQUOT,             "Disc quota exceeded" },
	{ WSAESTALE,             "Stale NFS file handle" },
	{ WSAEREMOTE,            "Too many levels of remote in path" },
	{ WSASYSNOTREADY,        "Network subsystem is unavailable" },
	{ WSAVERNOTSUPPORTED,    "Winsock version not supported" },
	{ WSANOTINITIALISED,     "Winsock not yet initialized" },
	{ WSAHOST_NOT_FOUND,     "Host not found" },
	{ WSATRY_AGAIN,          "Non-authoritative host not found" },
	{ WSANO_RECOVERY,        "Non-recoverable errors" },
	{ WSANO_DATA,            "Valid name, no data record of requested type" },
	{ WSAEDISCON,            "Graceful disconnect in progress" },
	{ WSASYSCALLFAILURE,     "System call failure" },
	{ WSA_NOT_ENOUGH_MEMORY, "Insufficient memory available" },
	{ WSA_OPERATION_ABORTED, "Overlapped operation aborted" },
	{ WSA_IO_INCOMPLETE,  	 "Overlapped I/O object not signalled" },
	{ WSA_IO_PENDING,        "Overlapped I/O will complete later" },
	//{ WSAINVALIDPROCTABLE,   "Invalid proc. table from service provider" },
	//{ WSAINVALIDPROVIDER,    "Invalid service provider version number" },
	//{ WSAPROVIDERFAILEDINIT, "Unable to init service provider" },
	{ WSA_INVALID_PARAMETER, "One or more parameters are invalid" },
	{ WSA_INVALID_HANDLE,    "Event object handle not valid" }
};
const int kNumMessages = sizeof(gaErrorList) / sizeof(ErrorEntry);


//// ErrMsg ////////////////////////////////////////////
// A function similar in spirit to Unix's perror() that tacks a canned 
// interpretation of the value of WSAGetLastError() onto the end of a
// passed string, separated by a ": ".  Generally, you should implement
// smarter error handling than this, but for default cases and simple
// programs, this function is sufficient.
//
// This function returns a pointer to an internal static buffer, so you
// must copy the data from this function before you call it again.  It
// follows that this function is also not thread-safe.

char const * ErrMsg(char const * pcMessagePrefix)
{
	// Build basic error string
	static char acErrorBuffer[256];
	ostrstream outs(acErrorBuffer, sizeof(acErrorBuffer));
	outs << pcMessagePrefix << ": ";

	// Tack appropriate canned message onto end of supplied message 
	// prefix -- if you want to make this faster, sort the table above
	// and do a binary search here instead.
	int nLastError = WSAGetLastError();
	int i;
	for (i = 0; i < kNumMessages; ++i) {
		if (gaErrorList[i].nID == nLastError) {
			outs << gaErrorList[i].pcMessage;
			break;
		}
	}
	if (i == kNumMessages) {
		// Didn't find error in list, so make up a generic one
		outs << "unknown error";
	}
	outs << " (" << nLastError << ")";

	// Finish error message off and return it.
	outs << ends;
	acErrorBuffer[sizeof(acErrorBuffer) - 1] = '\0';
	return acErrorBuffer;
}



