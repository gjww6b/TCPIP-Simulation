// EnDGTCPClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int sd, len;
struct sockaddr_in localAddr, servAddr;
int server_port = 13400;
char *sendbuffer="How are you?"; 
char recvbuffer[100];
char stIPAddress[20];
const char *IPADDRESS="C:/IPADDRESS.txt";

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
      //  strcpy(stIPAddress,"169.254.28.229");
	} 
	strcpy(stIPAddress,"169.254.144.82");
	ret=WinsockInitialize();
	if (ret==1){
  	    printf("WinsockInitializeError\n");
        return 1;
    }
  	printf("WinsockInitializeSuccess\n");

   /* create socket */
    sd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sd == INVALID_SOCKET){
       printf("socket failed with error: %ld\n", WSAGetLastError());
       WSACleanup();
       return 1;
	}

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(stIPAddress);
    servAddr.sin_port = htons(server_port);
		// send how are u
//	rc = send(sd, sendbuffer, (int)strlen(sendbuffer), 0);
	rc= sendto(sd, sendbuffer, (int)strlen(sendbuffer), 0, (struct sockaddr *) &servAddr,  sizeof(servAddr)); 
	if (rc == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(sd);
		WSACleanup();
	    return 1;
	}
	printf("Bytes Sent: %ld\n", rc);
		// receive message
	//rc = recv(sd, recvbuffer, 100, 0);
	while (1){
	  len = sizeof(servAddr);
      rc = recvfrom(sd, (char *)recvbuffer, 100, 0, (struct sockaddr *) &servAddr, &len); 
	  if ( rc > 0 )
		  printf("Bytes received: %d, %s \n", rc, recvbuffer); 
	  else{
		printf("receive failed with error: %d\n", WSAGetLastError());
		closesocket(sd);
		WSACleanup();
		break;
      } 
	}
	    // cleanup
    closesocket(sd);
    WSACleanup();
	return 0;
}

