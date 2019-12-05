// Author : Dr. Foster
// Purpose : desmonstration of winsock API using a simple server
//
//
//
//  Citation : Based off of sample code found at https://www.binarytides.com/winsock-socket-programming-tutorial/

#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>



#pragma comment(lib,"ws2_32.lib") //Winsock Library - don't touch this line.

// create some constants for error codes if the program dies...
#define ERROR_WINSOCK_INIT_FAILURE				1;
#define ERROR_WINSOCK_SOCKET_CREATE_FAILURE		2;
#define ERROR_WINSOCK_SOCKET_CONNECT_FAILURE	3;
#define ERROR_WINSOCK_SOCKET_SEND_FAILURE       4;
#define ERROR_WINSOCK_SOCKET_BIND_FAILURE       5;

struct sockaddr_in addrServer, addrClient;   // sockets are used to access the network
char * msgSend;
#define MSGRECVLENGTH 100
char msgRecv[MSGRECVLENGTH];
CHAR ipv4addr[INET_ADDRSTRLEN] = "127.0.0.1";  // hard-coded just to test functionality	

int main(int argc, char *argv[])
{
	// Before using Winsock calls on Windows, the Winsock library needs to be initialized...
	WSADATA wsa;
	SOCKET s, s_new;
	int c;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Winsock error Code : %d", WSAGetLastError());
		return ERROR_WINSOCK_INIT_FAILURE;
	}

	/* The socket() call takes three arguments. The first is the network protocol "Address Family", hence the AF_prefix.
	The two most common are AF_INET for IPv4 and AF_INET6 for IPv6. The next asks for the port type, which is usually a
	TCP port with SOCK_STREAM, or a UDP port with SOCK_DGRAM. The third parameter is the specific protocol, such as ICMP,
	IGMP, or for the purposes of the program, TCP, which uses the constant IPPROTO_TCP. */

	if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
		return ERROR_WINSOCK_SOCKET_CREATE_FAILURE;
	}
	// Th socket now exists...but it isn't configured with an IP address or port number yet.

	// specify the IP address of the server 
	InetPton(AF_INET, ipv4addr, &addrServer.sin_addr.s_addr);
	//or allow the system to select one
	//addrServer.sin_addr.s_addr = INADDR_ANY;					// NOTE!!! when you use INADDR_ANY, the host will print 0.0.0.0 as the IP Address, but it is listening.
	addrServer.sin_family = AF_INET;							// Must agree with the socket Address Family type
	addrServer.sin_port = htons(60480);							// htons() converts the host endianness to network endianness
																// This should always be used when transmitting integers
																// ntohs() converts the opposite way for receiving integers.

	if (bind(s, (struct sockaddr *) &addrServer, sizeof(addrServer)) == SOCKET_ERROR)
	{
		printf("Could not bind socket");
		return ERROR_WINSOCK_SOCKET_BIND_FAILURE;
	}
	// converts between the IP address as a number to a printable string in ipv4addr
	inet_ntop(AF_INET, &(addrServer.sin_addr), (PSTR)ipv4addr, INET_ADDRSTRLEN);  
	printf("Listening on IP address %s port %d...\n", ipv4addr, ntohs(addrServer.sin_port));
	
	listen(s, 5);  // tells socket to start listening for clients, 5 is the number of backlogged connections allowed
	
	c = sizeof(struct sockaddr_in);
	s_new = accept(s, (struct sockaddr *)&addrClient, &c);
	if (s_new == INVALID_SOCKET)
	{
		printf("Accept failed with error code : %d", WSAGetLastError());
	}
	inet_ntop(AF_INET, &(addrClient.sin_addr), (PSTR)ipv4addr, INET_ADDRSTRLEN);
	printf("Accepted a connection from IP address %s port %d...\n", ipv4addr, ntohs(addrClient.sin_port));

		int bytesRecv;
	bytesRecv = recv(s_new, msgRecv, MSGRECVLENGTH, 0);

	if (bytesRecv > 0)
	{
		printf("%s\n", msgRecv);  
	}



	closesocket(s);
	WSACleanup();
}