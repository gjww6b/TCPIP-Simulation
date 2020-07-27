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
#define MAXLINE 1024 
int main(int argc, char *argv[])
{
    int len, sockfd = 0;
    struct sockaddr_in serv_addr, cli_addr, multicast_addr; 
    int addrlen = sizeof(serv_addr);
    char buff[MAXLINE];
    char *hello = "Hello from PDC";
    char str[INET_ADDRSTRLEN];
    int ret;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1){
       printf("couldn't create socket\n");
    }
    printf("sockfd= %d \n", sockfd);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(&cli_addr, 0, sizeof(cli_addr));

   	
    // fill the PDC server's info
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT); 

    //inet_pton(AF_INET, "192.168.1.5", &(serv_addr.sin_addr));

    inet_pton(AF_INET, "239.255.0.3", &(multicast_addr.sin_addr));

    inet_ntop(AF_INET, &(serv_addr.sin_addr), str, INET_ADDRSTRLEN);
    printf(" Server is running at %s\n", str );

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
  
	while(1){
          ret = sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &cli_addr, len); 

    //      ret = sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &multicast_addr, len); 
          if (ret <0){
             printf("send Error\n");
             close(sockfd);
             break;
          }
          printf("sent...\n");
          sleep(1);  
        } 
          
    }
    else{
        printf("Can't read");
        close(sockfd);
    } 
    printf("good!");
    close(sockfd);
  }
