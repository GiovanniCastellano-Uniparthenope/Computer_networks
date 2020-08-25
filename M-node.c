#include "M-nodeheader.h"

int main(int argc, char *argv[])
{
    if(argc<2)
    {
        RED printf("Need N-node host IP as parameter\n");
        exit(1);
    }
    int sockfd, maxfd, connfd, incoming, request, disconnect, index, matched;
    ListInt *wallets, *matching;
    ListStr *waddr;
    pthread_t Nconn;
    struct sockaddr walladdr;
    struct timeval checker = {3, 0};
    InitializeI(&wallets);
    InitializeI(&matching);
    fd_set fset;
    matchblock matchs;
    block *blocks, singleblock;
    blocks=(block*)malloc(sizeof(block));
    nblocks=0; total=0;
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, Exit);
    
    //Connect to N-node
    Nfd=TCPConnection(13000, argv[1]);
    WHITE printf("M-node %d (ME) - ", 15000);
    GREEN printf("operative\n");
    WHITE printf("N-node %s:%d - ", argv[1], 13000);
    if(Nfd<0){RED printf("down\n");}else{GREEN printf("operative\n");}
    if(Nfd<0)
    {
        YELLOW printf("Waiting for an N-node\n");
        while(Nfd<0)
        {
            Nfd=TCPConnection(13000, argv[1]);
            sleep(2);
        }
        WHITE printf("N-node %s:%d - ", argv[1], 13000);
        GREEN printf("operative\n");
    }
    Nonline=TRUE;
    if(Write(Nfd, &nblocks, sizeof(int))<0)
        Nonline=FALSE;
    if(Nonline==TRUE)
    {
        if(Read(Nfd, &incoming, sizeof(int))<0)
            Nonline=FALSE;
    }
    if(Nonline==FALSE)
    {
        pthread_create(&Nconn, NULL, Nconnect, NULL);
    }
    for(int i=0; i<incoming; i++)
    {
        Read(Nfd, &singleblock, sizeof(block));
        addBlock(&blocks, &singleblock);
    }

    sockfd=TCPBindToPort(15000);
    Listen(sockfd, 10);
    WHITE printf("Listening on socket %d\n", sockfd);
    while(1)
    {
	    FD_ZERO(&fset);
        FD_SET(sockfd, &fset);
        if(Nonline==TRUE)
        {
            FD_SET(Nfd, &fset);
            maxfd=max(sockfd, Nfd);
        }
        else
            maxfd=sockfd;
        RollToStartI(wallets);        
        do
        {
            connfd=GetPointedI(wallets);
            if(connfd!=0)
                FD_SET(connfd, &fset);
            maxfd=max(maxfd, connfd);
        }while(NextI(wallets)!=0);
        maxfd++;
        RollToStartI(wallets);
        checker.tv_sec=3;
        checker.tv_usec=0;
        select(maxfd+1, &fset, NULL, NULL, &checker);
        if(FD_ISSET(sockfd, &fset))
        {
            Push_BackI(wallets, Accept(sockfd, NULL, NULL));
            GREEN printf("New client!\n");
        }
        if(FD_ISSET(Nfd, &fset))
        {
            if(Read(Nfd, &singleblock, sizeof(block))<0)
            {
                Nonline=FALSE;
                RED printf("N-node disconnected\n");
                YELLOW printf("Waiting for a new one\n");
                pthread_create(&Nconn, NULL, Nconnect, NULL);
            }
            else if(singleblock.id>nblocks)
            {
                if(Write(Nfd, &nblocks, sizeof(int))<0)
                    Nonline=FALSE;
                if(Read(Nfd, &incoming, sizeof(int))<0)
                    Nonline=FALSE;
                if(Nonline==TRUE)
                    for(int i=0; i<incoming; i++)
                    {
                        if(Read(Nfd, &singleblock, sizeof(block))<0)
                        {
                            i=incoming;
                            RED printf("N-node disconnected\n");
                            YELLOW printf("Waiting for a new one\n");
                            pthread_create(&Nconn, NULL, Nconnect, NULL);
                        }
                        else
                            addBlock(&blocks, &singleblock);
                    }
                else
                {
                    RED printf("N-node disconnected\n");
                    YELLOW printf("Waiting for a new one\n");
                    pthread_create(&Nconn, NULL, Nconnect, NULL);
                }
            }
            else
            {
                addBlock(&blocks, &singleblock);
            }    
        }
        do
        {
            connfd=GetPointedI(wallets);
            if(connfd!=0)
                if(FD_ISSET(connfd, &fset))
                {
                    disconnect=FALSE;
                    if(Read(connfd, &request, sizeof(int))<0)
                        disconnect=TRUE;
                    if(disconnect==FALSE)
                    {
                        switch(request)
                        {
                            case DISCONNECT: disconnect=TRUE; break;
                            
                            case NTRANSACTIONS: 
                                if(Read(connfd, &request, sizeof(int))<0)
                                    disconnect=TRUE;
                                if(request<0)
                                    request=0;
                                if(request>nblocks-1)
                                    request=nblocks-1;
                                if(Write(connfd, &request, sizeof(int))<0)
                                        disconnect=TRUE;
                                for(int i=nblocks-1; i>nblocks-1-request; i--)
                                {
                                    if(blocks[i].id!=0)
                                        if(Write(connfd, &blocks[i], sizeof(block))<0)
                                            disconnect=TRUE;
                                }
                                BLUE printf("Client on socket %d requested the last %d transactions\n", connfd, request);
                                break;

                            case IDINFO: 
                                if(Read(connfd, &request, sizeof(int))<0)
                                    disconnect=TRUE;
                                if(request>nblocks-1)
                                    request=nblocks-1;
                                if(request<1)
                                    request=1;
                                if(Write(connfd, &blocks[request],sizeof(block))<0)
                                        disconnect=TRUE;
                                BLUE printf("Client on socket %d requested the ID %d\n", connfd, request);
                                break;
                            
                            case TOTAL:
                                if(Write(connfd, &total,sizeof(int))<0)
                                        disconnect=TRUE;
                                BLUE printf("Client on socket %d requested the total, equal to %d right now\n", connfd, total);
                                break;

                            case IPPORTINFO:
                                if(Read(connfd, &matchs, sizeof(matchblock))<0)
                                    disconnect=TRUE;
                                BLUE printf("Client on socket %d requested the IP:Port %s\n", connfd, matchs.IP);
                                matched=matchIPTransaction(&matchs, &blocks, matching);
                                BLUE printf("Found %d matching transactions\n", matched);                                
                                if(Write(connfd, &matched, sizeof(int))<0)
                                        disconnect=TRUE;
                                do
                                {
                                    index=GetPointedI(matching);
                                    if(index!=0)
                                        if(Write(connfd, &blocks[index], sizeof(block))<0)
                                            disconnect=TRUE;
                                }while(ErasePointedI(matching)==1);
                        }
                    }
                    if(disconnect==TRUE)
                    {
                        RED printf("Client on socket %d disconnected\n", GetPointedI(wallets));
                        WHITE
                        ErasePointedI(wallets);                        
                    }
                }
        }while(NextI(wallets)!=0);
    }
}
















