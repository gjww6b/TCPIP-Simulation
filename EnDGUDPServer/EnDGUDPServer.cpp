// EnDGUDPServer.cpp : Cruise emulator, broadcasting Cruise Data to 192.168.1.255 subnet and 1534 port  
//

#include "stdafx.h"
#include "stdlib.h"
#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFLEN 512	//Max length of buffer
#define PORT 1534	//The port on which to listen for incoming data
char stIPAddress[20];
int main()
{
	SOCKET s;
	struct sockaddr_in si_other;
	const char *buf="Cruise Data....";
	WSADATA wsa;
	int rc;

	strcpy(stIPAddress,"192.168.1.255");
	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		return (0);
	}
	printf("Initialised.\n");
	
	//Create a socket
	if((s = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
		return (0);
	}
	printf("Socket created for broadcast on subnet UDP.\n");
	
	//Prepare the sockaddr_in structure
	si_other.sin_family = AF_INET;
	si_other.sin_addr.s_addr = inet_addr(stIPAddress);
	si_other.sin_port = htons( PORT );
	

	while(1)
	{
		printf("send out the Cruise UDP datagram...\n");
		
		rc=sendto(s, buf, (int)strlen(buf), 0, (struct sockaddr*) &si_other, sizeof(si_other));
		if (rc < 0){
			printf("sendto() failed with error code : %d" , WSAGetLastError());
			break;
		}
		_sleep(1);
	}
    printf("end");
	closesocket(s);
	WSACleanup();
	
	return 0;
}


