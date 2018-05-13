#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define BUFF_SIZE 1024

static void usage(const char* str)
{
    printf("Usage: %s [IP] [PORT]\n",str);
}
// ./server 127.0.0.1 8080
int main(int argc, char *argv[])
{
    if (3 != argc)
    {
        usage(argv[0]);
        return 1;
    }
    
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        printf("socket!\n");
        return 2;
    }
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    
    if ( 0 > (bind(sock,(struct sockaddr *)&server_addr, sizeof(server_addr))))
    {
         printf("bind error!\n");
         return 3;
    }
    
    char buf[BUFF_SIZE];
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    
    while(1)
    {
        ssize_t s = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr *)&client_addr, &len);
        if (s > 0)
        {
            buf[s] = 0;
            printf("%s:%d say # %s\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port),buf);
        }   sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&client_addr,sizeof(client_addr));
        
        
    }
    close(sock);
    return 0;
}