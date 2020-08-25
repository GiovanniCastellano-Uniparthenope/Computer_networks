#ifndef MNODEHEADER_H_INCLUDED
#define MNODEHEADER_H_INCLUDED

#include "GeneralHeader.h"
#include "Connection.h"

int sockfd;

void Exit()
{
    int request=DISCONNECT;
    Write(sockfd, &request, sizeof(int));
    shutdown(sockfd, 0);
    exit(0);
}

#endif
