// EnDGUDPReceiver.cpp : UDPtoTCPconverter to bridge the cruise data to acquisition tool

#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#include "stdafx.h"
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

int main()
{

    int iResult = 0;

    WSADATA wsaData;

    SOCKET RecvSocket, m_sock, m_newSock;
    struct sockaddr_in RecvAddr, m_servAddr;

    unsigned short Port = 1534;
	 unsigned short port = 13400;

    char RecvBuf[1024];
    int BufLen = 1024;

    struct sockaddr_in SenderAddr;
    int SenderAddrSize = sizeof (SenderAddr);

    //-----------------------------------------------
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"WSAStartup failed with error %d\n", iResult);
        return 1;
    }
   
	/* create TCP socket for Aptiv Tool */
    m_sock = socket ( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if(m_sock== INVALID_SOCKET){
	  printf("create TCP socket Error\n");
	  WSACleanup();
	  return 1;
    } 
	//bind
	memset ( &m_servAddr, 0, sizeof ( m_servAddr ) );
	m_servAddr.sin_family = AF_INET;
	m_servAddr.sin_addr.s_addr =  htonl(INADDR_LOOPBACK); // htonl(INADDR_ANY);
	m_servAddr.sin_port = htons(port);
	
	iResult=bind ( m_sock,(struct sockaddr* )&m_servAddr, sizeof(m_servAddr) );
	if (iResult==SOCKET_ERROR){
		printf("bind Error\n");
		closesocket (m_sock);
		return 1;
	}    
	// listen
	iResult=listen(m_sock, 1);
	if (iResult==SOCKET_ERROR){
		printf("listen Error\n");
		closesocket (m_sock);
		return 1;
	} 

	printf(" \n--------  UDP TCP Bridge Server Started --------\n");
  // Wait Aptiv Tool
  	m_newSock = accept(m_sock, NULL, NULL);
	if (m_newSock== INVALID_SOCKET){
		closesocket (m_sock); 
		return 1;
	}
	closesocket (m_sock);
	memset ( &RecvBuf, 0, sizeof ( RecvBuf ) );
	printf(" receive bytes ");
	iResult= recv(m_newSock,RecvBuf,BufLen,0); // wait for data 
	if (iResult==SOCKET_ERROR){
		printf("receive Error\n");
		closesocket (m_newSock);
		return 1;
    }

	//-----------------------------------------------
    // Create a receiver socket to receive datagrams
    RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (RecvSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error %d\n", WSAGetLastError());
		closesocket (m_newSock);
        return 1;
    }
    //-----------------------------------------------
    // Bind the socket to any address and the specified port.
    RecvAddr.sin_family = AF_INET;
    RecvAddr.sin_port = htons(Port);
    RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    iResult = bind(RecvSocket, (SOCKADDR *) & RecvAddr, sizeof (RecvAddr));
    if (iResult != 0) {
        wprintf(L"bind failed with error %d\n", WSAGetLastError());
		closesocket (m_newSock);
		closesocket (RecvSocket);
        return 1;
    }
    //-----------------------------------------------
    // Call the recvfrom function to receive datagrams
    // on the bound socket.
    wprintf(L"Receiving datagrams...\n");
	memset ( &RecvBuf, 0, sizeof ( RecvBuf ) );
	printf("RecvBuf: %s\n", RecvBuf);


    while(1){
		iResult = recvfrom(RecvSocket,RecvBuf, BufLen, 0, (SOCKADDR *) & SenderAddr, &SenderAddrSize);
		if (iResult == SOCKET_ERROR) {
			wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
			closesocket (m_newSock);
		    closesocket (RecvSocket);
			return 1;
		}
		printf("received: %s\n", RecvBuf);
		iResult = send(m_newSock, RecvBuf, (int)strlen(RecvBuf),0); 
		if (iResult ==SOCKET_ERROR){
			printf("send Error d%\n", WSAGetLastError());
			closesocket (m_newSock);
			closesocket (RecvSocket);
			WSACleanup();
		    return 1;
		}
		printf("successfully sent\n");

    } 
    //-----------------------------------------------
    // Close the socket when finished receiving datagrams
    wprintf(L"Finished receiving. Closing socket.\n");
	closesocket (m_newSock);
    iResult = closesocket(RecvSocket);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"closesocket failed with error %d\n", WSAGetLastError());
        return 1;
    }

    //-----------------------------------------------
    // Clean up and exit.
    wprintf(L"Exiting.\n");
    WSACleanup();
    return 0;
}
