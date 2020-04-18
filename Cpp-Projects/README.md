# Cpp Projects

## TCP-WinSocket-App

A simple tcp connection with a socket is created, where the server just echoes what the client is sending. For this project the WinSock library has been used. 

### TCP Simple-TCP-Server

The server creates a socket with a listener that "listens" at the certain IP-addres and port. Afterwards in it waits for a connection and echos the receveving information back to the client. The communcation is designed in blocking(synchronous) way.

### TCP Simple-TCP-Client

The TCP- Client connects and sends some text to the server, where it then expects to receive the same amount of data back. The user can define a timeout to define when the application should terminate in order to avoid waiting hanging the socket communication. For this project the template design pattern, where it creates a thread for every socket communication. For the communication between the main thread and worker threads a critical section has been used to avoid data incosistency. 

## UDP-WinSocket-App



### TCP Simple-TCP-Server


### TCP Simple-TCP-Client

