Christophoros Prodromou-14598
Vangelis Photiou-14585


What have we done:

We created a program for authenticating and transfering files from a client to a server
using a socket connection.The first part is a program that uses UDP sockets that includes a UDP server
and a UDP client.The UDP client must be authenticated by the server with a username-password login combination.
The UDP server must verify whether the credentials are correct or wrong. The file named "login"
stores the credentials of the users used for verification.
In the second part after the authentication we have a connection that uses TCP socket for transfering
files from the TCP client to the TCP server.The client sends the filename to the server and waits for
a response about whether the file of that name exists, which in this case a message will be sent
to him asking if the file has to be overwritten, appended or the file transfer aborted.
If there is no such file, the TCP server will ask the client to send the file.

~udpserver.c:Creates a UDP socket,binds on a port number and receives the login-password
combination from the UDP client for authentication through the "login" file.

~udpclient.c:Creates a UDP socket,binds on a port number and sends its login and password to the 
the UDP server for authentication.Controls how many times a user should try to connect and what 
to do in case of a success/failure at authentication.

~tcpserver.c:Creates,initializes bind and listens on a TCP socket for transfering and managing 
files from the TCP clients to the TCP sevrer.

~tcpclient.c:Creates and binds a TCP socket for transfering files from the TCP client to the
TCP sevrer.

~Makefile:Compiles all the files.c and creates the executables for the program.It has a method for 
deleting the executables created before and can start the UDP and TCP servers at the same time
in predefined ports.

How to run the program: 
-Open a terminal in a linux environment.
-Run the Makefile using the command "make" in the directory with all of the source files.
-Run the command "make start" to start the socket servers.
-Run the command "./udpclient 127.0.0.1 2400" to start the udp client.
-Login with username=admin and password=1234.
-After successful authentication,the TCP client will automatically start.
-Give the name of the file you want to transfer to the TCP sevrer.
-Follow the instructions.
-Press CTRL-C to stop the program
