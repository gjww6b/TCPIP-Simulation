// EnDGTCPServerM.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#define MAXCONNECTIONS 2
#define BUFFER_SIZE 0xFFFF
struct sockaddr_in m_cliAddr, m_servAddr;
int port = 13400;
int m_sock;
int m_newSock[2];

HANDLE  hThreadArray[MAXCONNECTIONS];  

DWORD dwThreadIdArray[MAXCONNECTIONS];
DWORD WINAPI MyThreadFunction1( void *);
DWORD WINAPI MyThreadFunction2( void *);

DWORD (WINAPI *func_ptr[2])(void *) = {&MyThreadFunction1, &MyThreadFunction2} ;

//DWORD (WINAPI *func_ptr)(void *) = &MyThreadFunction1 ;


void sleep( clock_t wait )
{
   clock_t goal;
   goal = wait + clock();
   while( goal > clock() )
      ;
}

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

int main(int argc, char* argv[]){
    int ret, retsent;
    int i=0;
	printf("Hello World!\n");



	ret= WinsockInitialize();
    if (ret !=0) { 
	       printf("WinSockIntError\n");
	       return 0;
	}
	else
	       printf("WinSockIntSuccess\n");		   

	while(1){
		memset ( &m_servAddr, 0, sizeof ( m_servAddr ) );
		memset ( &m_cliAddr, 0, sizeof ( m_cliAddr ) );
		m_servAddr.sin_family = AF_INET;
		m_servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		m_servAddr.sin_port = htons(port);

		// create socket 
		m_sock = socket ( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		if(m_sock== INVALID_SOCKET){
			printf("create socket Error\n");
		//	WSACleanup();
			sleep((clock_t) 5);
			continue;
		}
    
		//bind
		ret = bind ( m_sock,(struct sockaddr* )&m_servAddr, sizeof ( m_servAddr ) );
		if (ret==SOCKET_ERROR){
			printf("bind Error\n");
			closesocket (m_sock);
			sleep((clock_t) 5);
			continue;
		}    
		// listen
		ret=listen(m_sock, MAXCONNECTIONS);
		if (ret==SOCKET_ERROR){
			printf("listen Error\n");
			closesocket (m_sock);
			sleep((clock_t) 5);
			continue;
		} 
		printf(" \n-------- Server Program Started --------\n");


  // Wait until all threads have terminated.
        for (i = 0; i<MAXCONNECTIONS; i++) {

		  m_newSock[i] = accept(m_sock, NULL, NULL);
		  if (m_newSock[i]== INVALID_SOCKET){
		      closesocket (m_sock); 
			  sleep((clock_t) 5);
			  continue;
		  }
          hThreadArray[i] = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
			func_ptr[i],             // thread function name
            &m_newSock[i],             // argument to thread function 
            0,                      // use default creation flags 
            &dwThreadIdArray[i]);   // returns the thread identifier 
            if (hThreadArray[i] == NULL) {
               printf(" create thread error\n");  
			   ExitProcess(3);
            }
        } //for
   		closesocket(m_sock);
		printf("Socket closed\n");
        
        WaitForMultipleObjects(MAXCONNECTIONS, hThreadArray, TRUE, INFINITE);
        CloseHandle(hThreadArray[0]);
        CloseHandle(hThreadArray[1]);
    } // while
	WSACleanup(); 
	return 0;

}

DWORD WINAPI MyThreadFunction1( void *pMySocket ){
	int m_newSock, ret, retsent;
	char buffer1[ BUFFER_SIZE ];

    m_newSock = *((int*)pMySocket);
	printf("MyThreadFunction1 %d\n", m_newSock);
	while (1){
		memset(buffer1,0x0,BUFFER_SIZE); // init buffer 
		//printf(" receive bytes ");
		ret= recv(m_newSock,buffer1,BUFFER_SIZE,0); // wait for data 
		if (ret >= 0){
			printf(" From Client1 received - %s\n",buffer1);
			strcpy(buffer1,"I am Server:ThreadFunction1");
			retsent = send(m_newSock, buffer1, ret,0); 
			//printf(" Sent status to Client1 = %d",retsent);
			if (retsent ==SOCKET_ERROR){
				printf("send Error d%\n", WSAGetLastError());
				closesocket (m_newSock);
				break;
			} 
			//printf("successfully sent\n");
		}
		else if(ret==0)
			printf(" receive NULL\n");
		else {
			printf("receive Error: %d\n", WSAGetLastError());
			closesocket (m_newSock);
			break;
		}
	}// while
	ret = shutdown(m_newSock, SD_SEND);
	if (ret == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(m_newSock);
	}
	return 1;
}

DWORD WINAPI MyThreadFunction2( void *pMySocket ){
	int m_newSock, ret, retsent;
	char buffer2[ BUFFER_SIZE ];
	
    m_newSock = *((int*)pMySocket);
	printf("MyThreadFunction2 %d\n", m_newSock);
	while (1){
		memset(buffer2,0x0,BUFFER_SIZE); // init buffer 
		//printf(" receive bytes ");
		ret= recv(m_newSock,buffer2,BUFFER_SIZE,0); // wait for data 
		if (ret >= 0){
			printf(" From Client2 received - %s\n",buffer2);
			strcpy(buffer2,"I am Server:ThreadFunction2 ");
			retsent = send(m_newSock, buffer2, ret,0); 
			//printf(" Sent status to Client2 = %d",retsent);
			if (retsent ==SOCKET_ERROR){
				printf("send Error d%\n", WSAGetLastError());
				closesocket (m_newSock);
				break;
			} 
			//printf("successfully sent\n");
		}
		else if(ret==0)
			printf(" receive NULL\n");
		else {
			printf("receive Error: %d\n", WSAGetLastError());
			closesocket (m_newSock);
			break;
		}
	}// while
	ret = shutdown(m_newSock, SD_SEND);
	if (ret == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(m_newSock);
	}
	return 2;
}
