#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 20018

#define BUFF_SIZE 1024

#define SOCKET_NO_CONNECT 1

int main(int argc, char *argv[])
{
	int sockfd, num;
	char buf[BUFF_SIZE];
	struct hostent *he;
	struct sockaddr_in server,peer;
    char timebuf[128];
    time_t now;
    char clientMessage[BUFF_SIZE+1];
	if (argc != 3)
	{
		printf("Usage:%s <IP address> <message>\n",argv[0]);
	}
	
	if ((sockfd = socket(AF_INET, SOCK_DGRAM,0)) == -1)
    {
		printf("socket error!\n");
        return -1;
	}
    
    memset(&server, 0, sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port = htons(PORT);
    #if (SOCKET_NO_CONNECT == 1)
        if ((he = gethostbyname(argv[1])) == NULL)
        {
            printf("gethostbyname() error!\n");
            return -1;
        }
        server.sin_addr = *((struct in_addr *)he->h_addr);
        
    #else
        server.sin_addr.s_addr = inet_addr(argv[1]);
        if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
        {
            printf("connect error!\n");
            return -1;
        }
    #endif
    
   
    while(1)
    {
        
        time(&now);
        strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", gmtime(&now)); 
        memset(clientMessage, 0, sizeof(clientMessage));
        snprintf(clientMessage, sizeof(clientMessage)-1,"%s [%s]",argv[2],timebuf);
    #if (SOCKET_NO_CONNECT == 1)
        sendto(sockfd, clientMessage, strlen(clientMessage), 0, (struct sockaddr *)&server, sizeof(server));
        socklen_t len = sizeof(server);
        if((num = recvfrom(sockfd,buf, sizeof(buf)-1, 0, (struct sockaddr *)&peer, &len)) == -1)
        {
            printf("recvfrom() error!\n");
            return -1;
        }
        if (len != sizeof(server) || memcmp((void *)&server,(void *)&peer,len) != 0)
        {
            printf("Receive message from other server.\n");
            continue;
        }
    #else
        send(sockfd, clientMessage, strlen(clientMessage), 0);
        
        if ((num = recv(sockfd, buf, BUFF_SIZE, 0)) == -1)
        {
            printf("recv error!\n");
            return -1;
        }
    #endif    
        buf[num] = '\0';
        printf("Server message: %s\n",buf);
        
        sleep(2);
    }
    
    close(sockfd);
}

