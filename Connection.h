#include <sys/socket.h>
#include "wrapper.h"

int TCPConnection(int port, char* IP_address)
{
    int sockfd, i;
    struct sockaddr_in servaddr;
    servaddr.sin_family=AF_INET;
    Inet_Pton(AF_INET, IP_address, &servaddr.sin_addr);
    servaddr.sin_port=htons(port);
    sockfd=Socket(AF_INET, SOCK_STREAM, 0);

    if(Connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0)
    {return -1;}
    else
    {return sockfd;}
}

