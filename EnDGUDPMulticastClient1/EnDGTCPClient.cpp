// EnDGTCPClient.cpp : Defines the entry point for the console application.
// this code is resided on PC, it acts as UDP Clint to receive data from Crouse and transfor the TCP clint 

#include "stdafx.h"

#include <ws2tcpip.h>

#define PORTMULTICAST 4321
int sd, len, sdMulticast, m_sock, m_newSock;
struct sockaddr_in localSock, servAddr, m_servAddr;
int server_port = 13400;
int port = 13401;
struct ip_mreq group;
char *sendbuffer="How are you?"; 
char recvbuffer[100];
char stIPAddress[20];
const char *IPADDRESS="C:/IPADDRESS.txt";
char buffer1[100];

int WinsockInitialize(){
    int err;
	WORD wVersionReg;
	WSADATA wsaData;

	wVersionReg = MAKEWORD(1, 1);
	err = WSAStartup(wVersionReg, &wsaData);
	if (err != 0)
       return (1);
    return (0);
}

int main(int argc, char* argv[])
{
	int ret, rc;
	printf("Hello World!\n");
  
	FILE * pFile=NULL;
    pFile = fopen (IPADDRESS , "r");
    if (pFile !=NULL) {
        fgets (stIPAddress , 19 , pFile);
        fclose (pFile);
	}
	else{
		printf("can't open the file!\n");
	} 
	strcpy(stIPAddress,"169.254.198.81");// the PDC IP address
	ret=WinsockInitialize();
	if (ret==1){
  	    printf("WinsockInitializeError\n");
        return 1;
    }
  	printf("WinsockInitializeSuccess\n");

   /* create unicast socket for control */
    sd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sd == INVALID_SOCKET){
       printf("socket failed with error: %ld\n", WSAGetLastError());
       WSACleanup();
       return 1;
	}

   /* create Multicast socket to receive the sensor data */
	sdMulticast = socket(AF_INET, SOCK_DGRAM, 0);

    if(sdMulticast == INVALID_SOCKET){
       printf("multicast socket failed with error: %ld\n", WSAGetLastError());
       WSACleanup();
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
	m_servAddr.sin_addr.s_addr =  htonl(INADDR_LOOPBACK); //!!!
	m_servAddr.sin_port = htons(port);
	rc = bind ( m_sock,(struct sockaddr* )&m_servAddr, sizeof ( m_servAddr ) );
	if (rc==SOCKET_ERROR){
		printf("bind Error\n");
		closesocket (m_sock);
		return 1;
	}    
 
	// listen
	rc=listen(m_sock, 1);
	if (rc==SOCKET_ERROR){
		printf("listen Error\n");
		closesocket (m_sock);
		return 1;
	} 
	printf(" \n-------- UDP TCP Bridge Server Started --------\n");	
 // Wait for Aptiv Tool.
  	m_newSock = accept(m_sock, NULL, NULL);
	if (m_newSock== INVALID_SOCKET){
		closesocket (m_sock); 
		return 1;
	}
	closesocket (m_sock);
	memset(buffer1,0x0,100); // init buffer 
	printf(" receive bytes ");
	ret= recv(m_newSock,buffer1,100,0); // wait for data 
	if (ret==SOCKET_ERROR){
		printf("receive Error\n");
		closesocket (m_newSock);
		return 1;
    }


     // control the crouse 
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(stIPAddress);
    servAddr.sin_port = htons(server_port);



	rc= sendto(sd, sendbuffer, (int)strlen(sendbuffer), 0, (struct sockaddr *) &servAddr,  sizeof(servAddr)); 
	if (rc == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(sd);
		WSACleanup();
	    return 1;
	}
	printf("Bytes Sent: %ld\n", rc);
		// receive message
	len =sizeof(servAddr);
    rc = recvfrom(sd, (char *)recvbuffer, 100, 0, (struct sockaddr *) &servAddr, &len); 
	
    if ( rc > 0 ){
	    printf("Bytes received: %d, %s \n", rc, recvbuffer);
	    closesocket(sd);
		/* Bind to the proper port number with the IP address */
		/* specified as INADDR_ANY. */
		memset((char *) &localSock, 0, sizeof(localSock));
		localSock.sin_family = AF_INET;
		localSock.sin_port = htons(PORTMULTICAST);
		localSock.sin_addr.s_addr = INADDR_ANY;
		rc=bind(sdMulticast, (struct sockaddr*)&localSock, sizeof(localSock));
	    if (rc == SOCKET_ERROR) {
		  printf("bind error: %d\n", WSAGetLastError());
		  closesocket(sdMulticast);
		  WSACleanup();
	      return 1;
        }
		printf("Binding datagram socket...OK.\n");
		/* Join the multicast group 226.1.1.1 on the local 203.106.93.94 */
		/* interface. Note that this IP_ADD_MEMBERSHIP option must be */
		/* called for each local interface over which the multicast */
		/* datagrams are to be received. */
		group.imr_multiaddr.s_addr = inet_addr("239.255.0.3");     // the Crouse Data multicast address
        group.imr_interface.s_addr = inet_addr("169.254.89.149");  // local IP, the OSP IP address
        rc = setsockopt(sdMulticast, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group));
	    if (rc == SOCKET_ERROR) {
		  printf("adding multicast group error: %d\n", WSAGetLastError());
		  closesocket(sdMulticast);
		  WSACleanup();
	      return 1;
        }
        printf("Setting SO_REUSEADDR...OK.\n");
		len = sizeof(localSock);
		while(1){
			rc=recvfrom(sdMulticast, recvbuffer, sizeof(recvbuffer), 0, (SOCKADDR *)&localSock, &len);
			if (rc == SOCKET_ERROR) {
			  printf("adding multicast group error: %d\n", WSAGetLastError());
			  closesocket(sdMulticast);
			  WSACleanup();
			  return 1;
			} 
			printf("The message from multicast server is: \"%s\"\n", recvbuffer);
			rc = send(m_newSock, recvbuffer, (int)strlen(recvbuffer),0); 
			if (rc ==SOCKET_ERROR){
				printf("send Error d%\n", WSAGetLastError());
				closesocket (m_newSock);
				WSACleanup();
			    return 1;
			}
			else 
				printf("successfully sent\n");

		}
	}
	else{
		printf("receive failed with error: %d\n", WSAGetLastError());
		closesocket(sd);
		WSACleanup();
    } 

	    // cleanup
	printf("end\n");
    closesocket(sd);
	closesocket(sdMulticast);
    WSACleanup();
	return 0;
}

