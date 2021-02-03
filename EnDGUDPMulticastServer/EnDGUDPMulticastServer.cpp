// EnDGUDPMulticastServer.cpp : Defines the entry point for the console application.
// this code simulates Cruise. it acts as Crouse to broadcast UDP package 

#include "stdafx.h"
#include <stdlib.h>
#include <ws2tcpip.h>


int sd, len, sdMulticast;
struct sockaddr_in  servAddr, multicast_addr;
struct in_addr localInterface;
int PORTMULTICAST = 13400;
struct ip_mreq group;
char *sendbuffer="How are you?"; 
char recvbuffer[100];
char LocalIPAddress[20];
char UDPMulticastIPAddress[20];

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
	char *SensorData ="Radar, Camera, LiDar, GPS, IMU, ultrasonic raw data.....";

	printf("Cruise is here\n");
  
    strcpy(UDPMulticastIPAddress,argv[1]);
    PORTMULTICAST=atoi(argv[2]);
    strcpy(LocalIPAddress,argv[3]);
    printf("UDP Multicast IP address =%s, port = %d, Local IP=%s\n", UDPMulticastIPAddress,PORTMULTICAST,LocalIPAddress);
	ret=WinsockInitialize();
	if (ret==1){
  	    printf("WinsockInitializeError\n");
        return 1;
    }
  	printf("WinsockInitializeSuccess\n");


   // create multicast socket for transport the sensor data*
	sdMulticast = socket(AF_INET, SOCK_DGRAM, 0);

    if(sdMulticast == INVALID_SOCKET){
       printf("multicast socket failed with error: %ld\n", WSAGetLastError());
       WSACleanup();
       return 1;
	}
  // fill the Multicast' info
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_addr.s_addr = inet_addr(UDPMulticastIPAddress);
    multicast_addr.sin_port = htons(PORTMULTICAST); 

//	inet_ntop(AF_INET, &(multicast_addr.sin_addr), str, INET_ADDRSTRLEN);
//    printf(" Multicase address is %s\n", str );

    localInterface.s_addr=inet_addr(LocalIPAddress);
    rc = setsockopt(sdMulticast, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface));

/*		group.imr_multiaddr.s_addr = inet_addr("239.255.0.3");     // the Sensor Data multicast address
        group.imr_interface.s_addr = inet_addr("169.254.89.149");  // local IP, the OSP IP address
        rc = setsockopt(sdMulticast, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group));
*/
    if (rc == SOCKET_ERROR) {
		  printf("adding multicast group error: %d\n", WSAGetLastError());
		  closesocket(sdMulticast);
		  WSACleanup();
	      return 1;
     }
     printf("Local Interface is setting for MultiCast UDP\n"); 
//		len = sizeof(localSock);
	 while(1){
//		rc = sendto(sdMulticast, (const char *)SensorData, strlen(SensorData), 0, (const struct sockaddr *) &localSock, len); 
        rc = sendto(sdMulticast, (const char *)SensorData, strlen(SensorData), 0, (const struct sockaddr *) &multicast_addr, sizeof(multicast_addr)); 

		if (rc == SOCKET_ERROR) {
			  printf("adding multicast group error: %d\n", WSAGetLastError());
			  closesocket(sdMulticast);
			  WSACleanup();
			  return 1;
		} 
		printf("sent %s...\n", SensorData);
		_sleep(1);
	 }

	printf("end\n");
	closesocket(sdMulticast);
    WSACleanup();
	return 0;
}

