// Author : Dr. Foster
// Purpose : desmonstration of winsock API using a simple server
//
//
//
//  Citation : Based off of sample code found at https://www.binarytides.com/winsock-socket-programming-tutorial/

#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library - don't touch this.

CHAR IPADDR[INET_ADDRSTRLEN] = "192.168.97.53";

// create some constants for error codes if the program dies...
#define ERROR_WINSOCK_INIT_FAILURE				1;
#define ERROR_WINSOCK_SOCKET_CREATE_FAILURE		2;
#define ERROR_WINSOCK_SOCKET_CONNECT_FAILURE	3;
#define ERROR_WINSOCK_SOCKET_SEND_FAILURE       4;
#define ERROR_WINSOCK_SOCKET_BIND_FAILURE       5;

struct sockaddr_in addrServer;    // IN_ADDR holds an IPv4 address
char * message;
CHAR * ipv4addr;

int main(int argc, char *argv[])
{
	// Before using Winsock calls on Windows, the Winsock library needs to be initialized...
	WSADATA wsa;
	SOCKET s;

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

	ipv4addr = IPADDR;
	InetPton(AF_INET, ipv4addr , &addrServer.sin_addr.s_addr);  // There are useful functions to convert between formats,
	addrServer.sin_family = AF_INET;							// Must agree with the socket Address Family type
	addrServer.sin_port = htons(60480);							// htons() converts the host endianness to network endianness
																// This should always be used when transmitting integers
																// ntohs() converts the opposite way for receiving integers.

	inet_ntop(AF_INET, &(addrServer.sin_addr), (PSTR)ipv4addr, INET_ADDRSTRLEN);
	printf("Attempting to connect to %s on port %d...\n", ipv4addr, ntohs(addrServer.sin_port));
	
	//Connect to remote server
	if (connect(s, (struct sockaddr *) &addrServer, sizeof(addrServer)) < 0)
	{
		printf("Could not connect socket");
		return ERROR_WINSOCK_SOCKET_CONNECT_FAILURE;
	}
	   
	message = "Hello World!";
	if (send(s, message, (int)strlen(message), 0) < 0)
	{
		printf("Error sending message");
		return ERROR_WINSOCK_SOCKET_SEND_FAILURE;
	}

	closesocket(s);
	WSACleanup();
	return 0;
}