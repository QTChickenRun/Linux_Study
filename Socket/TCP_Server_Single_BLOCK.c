#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>

#define BUFF_SIZE 1024


/*
struct sockaddr_in 
{ 
sa_family_t sin_family; //地址族; AF_INET 
u_int16_t sin_port; //端口号 
struct in_addr sin_addr; //IPV4地址结构体 
} 
struct in_addr 
{ 
u_int32_t s_addr; //IPV4地址 
} 
*/
/*
int bind(int sockfd, const struct sockaddr *addr,  socketlen_t addren);

*/
#define TCP_IPv4_Address "192.168.123.17"
#define TCP_IPv4_Port 20018
#define __BACKLOG__ 128
int main(int argc, char *argv[])
{
    int Serverfd, Clientfd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int iReadLen = 0;
    ssize_t numRead;
    
    Serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == Serverfd)
    {
        printf("socket error!\n");
        return -1;
    }
    
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TCP_IPv4_Port);
    server_addr.sin_addr.s_addr = inet_addr(TCP_IPv4_Address);
    
    if (0 > bind(Serverfd, (struct sockaddr *)&server_addr,sizeof(struct sockaddr)))
    {
        printf("bind error, errno is %d, errstring is %s\n", errno, strerror(errno));
        close(Serverfd);
        return -1;
    }
    
    if (0 > listen(Serverfd,__BACKLOG__))
    {
        printf("bind error, errno is %d, errstring is %s\n", errno, strerror(errno));
        close(Serverfd);
        return -2;
    }
    
    printf("bind and listen success, wait accept!\n");  
    
    while(1)
    {
        socklen_t len = 0;
        /*阻塞等待客户端连接*/
        int client_socket = accept(Serverfd,(struct sockaddr *)&client_addr, &len);
        if (0 > client_socket)
        {
            printf("accept error, errstring is %s",strerror(errno));
            close(Serverfd);
            return -3;
        }
        
        /*获取客户端IP地址失败*/
        char client_ip[16];
        memset(client_ip, 0, sizeof(client_ip));
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, sizeof(client_ip));
        printf("get connect: ip is %x %s %s, port is %d\n",client_addr.sin_addr.s_addr, client_ip,inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
        
        while(1)
        {
            char buf[1024];
            memset(buf, 0, sizeof(buf));
            iReadLen = read(client_socket, buf, sizeof(buf));
            if (0 < iReadLen)
            {
                printf("Client Request: %s\n",buf);
                printf("Client Info is %s\n",inet_ntoa(client_addr.sin_ad));
                printf("Please Input Response:");
                memset(buf,0,sizeof(buf));
                fgets(buf, sizeof(buf), stdin);
                write(client_socket, buf, strlen(buf)+1);
                printf("Please Wait ...\n");
            }
            else
            {
                break;
            }
        }
    }
    
    close(Serverfd);
    return 0;
}
