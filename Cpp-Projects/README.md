# Cpp Projects

## TCP-WinSocket-App

A simple tcp connection with a socket is created, where the server just echoes what the client is sending. For this project the WinSock library has been used. 

### TCP Simple-TCP-Server

The server creates a socket with a listener that "listens" at the certain IP-addres and port. Afterwards in it waits for a connection and echos the receveving information back to the client. The communcation is designed in blocking(synchronous) way.

### TCP Simple-TCP-Client

The TCP- Client connects and sends some text to the server, where it then expects to receive the same amount of data back. The user can define a timeout to define when the application should terminate in order to avoid waiting hanging the socket communication. For this project the template design pattern, where it creates a thread for every socket communication. For the communication between the main thread and worker threads a critical section has been used to avoid data incosistency. 

## UDP-WinSocket-App

UDP connection is established, where the client sends datagrams to the server. oNEThe server checks if the packages are duplicate or cut off. Only one dierction of the communication is implemented. from client to Server.  Here we are testing how many packages are lost, how is the order packages that have been sent and if there a timeout is occuring due to the latency of the network. The server has a timeout configured of 3 seconds. The Winsock framework has been used fro this project. For encoding the data, a modified TLV-protocol has been used, where the fields are tag, length and value. Instead of a tag the total number of packages have been used instead. 
The package size 8 bytes:
* 2 bytes for the number of packages
* 2 bytes for current number of the package 
* 4 for data. 

Sadly, the overhead off this approach is 50%.

### TCP Simple-TCP-Server

A class is made to encapsulated the members and the library. For the timing the application the clock() function of the library <ctime> has been used. the server class checks if the recived length is 8 bytes and if the packages are duplicate dependin on the current number of the package 


### TCP Simple-TCP-Client

The Client just connects to the server and send the packages to the server.

## gUnitTest/Unit_test-StringOpsClass

Unit test for the classs StringOps with google framework. The test checks two functions AnalyseString() and CrackUncPath() from the StringOps class. The test is shuffled and executed randomly four times.

* The method AnalysisString(): processes the given string and returns the number of strings characters, numbers and other special characters(²³{[]}|µ@€~) back.

* The method CrackUncPath(): breaks down a UNC path into its components.
Example: The path\\Roadrunner\shared\Datasheets\Specs\2017\uart.pdf --> provides "Roadrunner" as hostname, "shared" as sharefolder, "Datasheets", "Specs", "2017"
as elements for Path, "uart" as file name and "pdf" as extension. 




