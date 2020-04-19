# Cpp Projects

## TCP-WinSocket-App

A simple TCP connection with a socket is created, where the server just echoes what the client is sending. For this project, the WinSock library has been used. 

### TCP Simple-TCP-Server

The server creates a socket with a listener that "listens" at certain IP-address and port. Afterward in it waits for a connection and echos the receiving information back to the client. The communication is designed in a blocking (synchronous) way.

### TCP Simple-TCP-Client

The TCP- Client connects and sends some text to the server, where it then expects to receive the same amount of data back. The user can define a timeout to define when the application should terminate in order to avoid waiting hanging the socket communication. For this project the template design pattern, where it creates a thread for every socket communication. For the communication between the main thread and worker threads, a critical section has been used to avoid data inconsistency. 

## UDP-WinSocket-App

UDP connection is established, where the client sends datagrams to the server. The server checks if the packages are duplicate or cut off and notify the user accordingly. Only one direction of communication is implemented. from Client to Server.  Here we are testing how many packages are lost, how is the order packages that have been sent and if there a timeout is occurring due to the latency of the network. The server has a timeout configured of 3 seconds. The Winsock framework has been used for this project. For encoding the data, a modified TLV-protocol has been used, where the fields are tag, length and value. Instead of a tag, the total number of packages have been used instead. 
The package size 8 bytes:
* 2 bytes for the number of packages
* 2 bytes for the current number of the package 
* 4 for data. 

Sadly, the overhead off this approach is 50%.

### TCP Simple-TCP-Server

A class is made to encapsulate the members and the library. For the timing of the application the clock() function of the library <ctime> has been used. the server class checks if the received length is 8 bytes and if the packages are duplicate depending on the current number of the package. 


### TCP Simple-TCP-Client

The Client just connects to the server and send the packages to the server.

## gUnitTest/Unit_test-StringOpsClass

Unit test for the class StringOps with google framework. The test checks two functions AnalyseString() and CrackUncPath() from the StringOps class. The test is shuffled and executed randomly four times.

* The method AnalysisString(): processes the given string and returns the number of strings characters, numbers and other special characters(²³{[]}|µ@€~) back.

* The method CrackUncPath(): breaks down a UNC path into its components.
Example: The path\\Roadrunner\shared\Datasheets\Specs\2017\uart.pdf --> provides "Roadrunner" as hostname, "shared" as sharefolder, "Datasheets", "Specs", "2017"
as elements for Path, "uart" as file name and "pdf" as extension. 


## Sorting-Algoritms-Comparison

Compares sorting algorithms between STL sort library, quick sort, and implementation of the linear sort. Linear sorting is interesting as it has time complexity of O(n), although the trade-off is it consumes more memory and therefore needs to be limited for certain range numbers. Results:

<img src="./Sorting-Algoritms-Comparison/Bar%20Chart.PNG" width="650" >

## JSONWritter

The JSon Writer gets an object and it converts it to a JSON format. The Implementation of the JSONWritter is realized with a __variadic template__. Meaning that template functions can accept different numbers of parameters. It can either accept one or many Persons objects and par Combination of key/value pair.
