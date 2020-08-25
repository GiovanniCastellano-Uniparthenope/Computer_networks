#include <sys/socket.h>
#include "wrapper.h"

int TCPBindToPort(int port)
{
    int sockfd, enable=1;
    struct sockaddr_in servaddr, clientaddr;
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=ntohs(port);
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);

    sockfd=Socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    if(Bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0)
    {
        printf("\033[1;33m""Port %d is bound already\nWaiting for system unbound\n", port);
        while(Bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){sleep(2);}
    }
    return sockfd;
}
