////////////////////////////////////////////////////////////
// Prototypes for socket server
////////////////////////////////////////////////////////////

#ifndef BasicServer_INCLUDED
#define BasicServer_INCLUDED

#include <string>
#include "hlp.h"

int    DoWinsock          (std::string const & Address, size_t Port);
SOCKET SetUpListener      (std::string const & Address, size_t Port);
SOCKET AcceptConnection   (SOCKET ListeningSocket, sockaddr_in & Remote);
bool   EchoIncomingPackets(SOCKET Sock);
bool   ShutdownConnection (SOCKET Sock);

#endif