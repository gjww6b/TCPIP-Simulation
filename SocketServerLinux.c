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

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 
    int addrlen = sizeof(serv_addr);
    char sendBuff[1025];
    char readbuff[1025];
    time_t ticks; 
    char str[INET_ADDRSTRLEN];
    int ret;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
       printf("couldn't create socket\n");
    }
    printf("listenfd= %d \n", listenfd);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(13400); 
    //inet_pton(AF_INET, "192.168.1.5", &(serv_addr.sin_addr));
 
    inet_ntop(AF_INET, &(serv_addr.sin_addr), str, INET_ADDRSTRLEN);
    printf(" Server %s is running at\n", str );
    ret = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret < 0 )
    {
      printf("bind failed %d\n", ret);
    } 
    printf("bind\n");

    ret=listen(listenfd, 10);
    if(ret<0)
    {
      printf("listen faild %d\n", ret);
    } 
    printf("listen to %d\n", listenfd);

    //connfd = accept(listenfd, (struct sockaddr *)&serv_addr, (socklen_t*)&addrlen); 
      connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
    if (connfd <0)
    { 
        printf("accept error"); 
        exit(EXIT_FAILURE); 
    } 
    printf("accepted\n");
    while(1){
      // 	memset(readbuff, 0x0, 1025);
        bzero(readbuff,1025);
	ret = recv(connfd, readbuff,1025,0);
	if (ret > 0){
          printf("received- %s\n", readbuff);
          strcpy(sendBuff, "received");
          ret = send(connfd,sendBuff,8, 0);
          if (ret <0){
             printf("send Error\n");
             close(connfd);
             break;
          } 
	}
	else if(ret==0)
             printf("received NULL\n");
        else{
             printf("receive Error\n");
             close(connfd);
	     break;
	}
        close(listenfd);
        printf("close listenfd\n");	
       /* ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff)); 

        close(connfd);*/
        sleep(1);
     }
}
