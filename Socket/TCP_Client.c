#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include<netinet/in.h>
#include <arpa/inet.h>

#define TCP_IPv4_Address "192.168.123.17"
#define TCP_IPv4_Port 20018

int main(int argc, char *argv[])
{
    int socket_fd;
    struct sockaddr_in server_sock;
    char buf[1024] = {0};
    
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > socket_fd)
    {
        printf("socket error, errno is %d, errstring is %s\n",errno,strerror(errno));
    }
    
    memset(&server_sock, 0, sizeof(struct sockaddr_in));
    server_sock.sin_family = AF_INET;
    server_sock.sin_port  = htons(TCP_IPv4_Port);
    server_sock.sin_addr.s_addr = inet_addr(TCP_IPv4_Address);
    
    int ret = connect(socket_fd, (struct sockaddr *)&server_sock, sizeof(struct sockaddr));
    if (0 > ret)
    {
        printf("connect error, errno is %d, errstring is %s\n",errno, strerror(errno));
        return -1;
    }
    
    printf("connect success!\n");
    
    while(1)
    {
        printf("Please Input Message:");
        scanf("%s",buf);
        write(socket_fd, buf, sizeof(buf));
        printf("Please wait server response \n");
        memset(buf,0,sizeof(buf));
        read(socket_fd,buf,sizeof(buf));
        printf("Server Response: %s\n", buf);
    }
    close(socket_fd);
    return 0;
}