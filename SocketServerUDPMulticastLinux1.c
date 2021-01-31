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
#define PORTMULTICAST     4321
#define MAXLINE 1024 
int main(int argc, char *argv[])
{
    int len, sockMultiCastfd; //sockfd = 0;
    struct sockaddr_in serv_addr, cli_addr, multicast_addr; 
    struct in_addr localInterface;
    int addrlen = sizeof(serv_addr);
    char buff[MAXLINE];
    char *hello = "ACK from PDC";
    char *SensorData ="Radar, Camera, LiDar, GPS, IMU, ultrasonic raw data.....";
    char str[INET_ADDRSTRLEN];
    int ret;

    memset(&multicast_addr, '0', sizeof(multicast_addr));
    sockMultiCastfd = 0;
     // create multicast socket for transport the sensor data
    sockMultiCastfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockMultiCastfd == -1){
       printf("couldn't create Multicast socket\n");
    }

    printf("UDP Multicast socket= %d \n", sockMultiCastfd);
   	
   // fill the Multicast' info
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_addr.s_addr = inet_addr("239.255.0.3");
    multicast_addr.sin_port = htons(PORTMULTICAST); 

 
    inet_ntop(AF_INET, &(multicast_addr.sin_addr), str, INET_ADDRSTRLEN);
    printf(" Multicase address is %s\n", str );

     localInterface.s_addr=inet_addr("169.254.198.81");
         ret = setsockopt(sockMultiCastfd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface));
	 if (ret < 0){
               int errsv=errno;
	       printf("setting local interface for multicast udp is failed %s\n", strerror(errno));
	       close (sockMultiCastfd);
	       exit(1);
	  }
	  printf("Local Interface is setting for MultiCast UDP\n"); 

	  while(1){
            ret = sendto(sockMultiCastfd, (const char *)SensorData, strlen(SensorData), 0, (const struct sockaddr *) &multicast_addr, sizeof(multicast_addr)); 

    //      ret = sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &multicast_addr, sizeof(multicast_addr)); 
            if (ret <0){
               int errsv = errno;
               printf("send Error: %s\n", strerror(errsv));
               close(sockMultiCastfd);
               break;
             }
            printf("sent %s...\n", SensorData);
            sleep(1);  
          } 
    printf("good!");
    close(sockMultiCastfd);
  }
