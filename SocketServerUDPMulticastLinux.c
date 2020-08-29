//this code is resided on the PDC as TCP socket server to provide the sensor data in the multicast manner
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#define PORT	 13400 
#define PORTMULTICAST     4321
#define MAXLINE 1024 
int main(int argc, char *argv[])
{
    int len, sockMultiCastfd, sockfd = 0;
    struct sockaddr_in serv_addr, cli_addr, multicast_addr; 
    struct in_addr localInterface;
    int addrlen = sizeof(serv_addr);
    char buff[MAXLINE];
    char *hello = "ACK from PDC";
    char *SensorData ="Radar, Camera, LiDar, GPS, IMU, ultrasonic raw data.....";
    char str[INET_ADDRSTRLEN];
    int ret;

      // create the unicast socket for executing command from OSP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1){
       printf("couldn't create UDP Server socket\n");
    }
    printf("UDP server socket= %d \n", sockfd);
  

    memset(&cli_addr, 0, sizeof(cli_addr));
    memset(&multicast_addr, '0', sizeof(serv_addr));
    sockMultiCastfd = 0;
     // create multicast socket for transport the sensor data
    sockMultiCastfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockMultiCastfd == -1){
       printf("couldn't create Multicast socket\n");
    }
    printf("UDP Multicast socket= %d \n", sockMultiCastfd);
   	
   // fill the PDC UDP server's info
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT); 

   // fill the Multicast' info
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_addr.s_addr = inet_addr("239.255.0.3");
    multicast_addr.sin_port = htons(PORTMULTICAST); 

  
    //inet_pton(AF_INET, "239.255.0.3", &(multicast_addr.sin_addr));

    inet_ntop(AF_INET, &(serv_addr.sin_addr), str, INET_ADDRSTRLEN);
    printf(" PDC UDP server address is %s\n", str );

    inet_ntop(AF_INET, &(multicast_addr.sin_addr), str, INET_ADDRSTRLEN);
    printf(" Multicase address is %s\n", str );

    // Bind the socket with the server address 
    ret = bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret < 0 ){
      printf("bind failed %d\n", ret);
    } 
    printf("bind\n");
    bzero(buff,MAXLINE);
    len = sizeof(cli_addr); //len is value/resuslt 
    ret = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cli_addr, &len); 
    inet_ntop(AF_INET, &(cli_addr.sin_addr), str, INET_ADDRSTRLEN);
    printf(" Client is running at %s\n", str );

    if (ret>=0){
        buff[ret] = '\0'; 
	printf("Client : %s\n", buff);

        //PDC received the command from OSP and acknology
        ret = sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &cli_addr, len); 
        if (ret <0){
          printf("send Error\n");
          close(sockfd);
        }
	else{
          printf("sent...\n");
          sleep(1);  
          close(sockfd);

         //set local interface for outbound multicast datagram, PDC IP =169.254.198.81
         localInterface.s_addr=inet_addr("169.254.198.81");
         ret = setsockopt(sockMultiCastfd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface));
	 if (ret < 0){
	       printf("setting local interface for multicast udp is failed\n");
	       close (sockMultiCastfd);
	       exit(1);
	  }
	  printf("Local Interface is setting for MultiCast UDP\n"); 

	  while(1){
            ret = sendto(sockMultiCastfd, (const char *)SensorData, strlen(SensorData), 0, (const struct sockaddr *) &multicast_addr, len); 

    //      ret = sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &multicast_addr, sizeof(multicast_addr)); 
            if (ret <0){
               printf("send Error\n");
               close(sockMultiCastfd);
               break;
             }
            printf("sent %s...\n", SensorData);
            sleep(1);  
          } 
        }  
    }
    else{
        printf("Can't read");
        close(sockfd);
        close(sockMultiCastfd);
    } 
    printf("good!");
    close(sockfd);
    close(sockMultiCastfd);
  }
