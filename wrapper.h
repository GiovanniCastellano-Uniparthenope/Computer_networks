#ifndef WRAPPER_H_INCLUDED
#define WRAPPER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>         //exit()
#include <unistd.h>         //read(), write()
#include <sys/socket.h>     //(Included into arpa/inet.h), socket(), connect(), bind(), listen(), accept()
#include <arpa/inet.h>      //inet_pton(), htons(), struct sockaddr, sockaddr_in
#include <string.h> 
#include <errno.h>

int Socket(int family, int stream, int parameter)//Client exclusive
{
	int sockfd;
	if((sockfd=socket(family, stream, parameter))<0)
	{
		fprintf(stderr, "Socket error\n");
		exit(1);
	}
	return sockfd;
}

void Inet_Pton(int family, char* IP_address, struct in_addr* sin_addr)//Client exclusive
{
	if (inet_pton(family, IP_address, sin_addr) < 0)
	{
      		fprintf(stderr,"inet_pton error for %s\n", IP_address);
      		exit (1);
    	}
}

int Connect(int sockfd, struct sockaddr* serveraddr, int size)//Client exclusive
{
	if (connect(sockfd, (struct sockaddr*) serveraddr,  size) < 0)
	{
    	return -1;
  	}
    return 0;
}

int Bind(int listenfd, struct sockaddr* servaddr, int size)//Server exclusive
{
	if ( bind(listenfd, (struct sockaddr *) servaddr, size) < 0 )
	{
    	return -1;
	}
    return 0;
}

void Listen(int listenfd, int buffer)//Server exclusive
{
	if ( listen(listenfd, buffer) < 0 )
	{
		fprintf(stderr, "Listen error\n");
		exit(1);
	}
}

int Accept(int listenfd, struct sockaddr* clientaddr, int *param)//Server exclusive
{
	int connfd;
	if ( ( connfd = accept(listenfd, (struct sockaddr*) clientaddr, param) ) < 0)
	{
		fprintf(stderr, "Accept error\n");
        return -1;
	}
	return connfd;
}

int Write(int connfd, void* buff, int size)
{
	if ( write(connfd, buff, size) != size) 
	{
      	return -1;
    }
    return 0;
}

int Read(int connfd, void *buff, int size)
{
    if ( read(connfd, buff, size) != size)
    {
        return -1;
    }
}

#endif
