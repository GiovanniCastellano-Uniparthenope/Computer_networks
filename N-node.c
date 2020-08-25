#include "N-nodeheader.h"

int main()
{
    int sockfd, connfd, requested=1, connected=FALSE, diff;
    fd_set fset;
    struct timeval checker = {0, 0};
    signal(SIGPIPE, sigpipe);
    YELLOW printf("Starting...\n");
    id=0;  
    generateSockets();
    GREEN printf("Created random hosts\n");
    blocks=(block*)malloc(sizeof(block));
    blocks[0].id=0;
    blocks[0].time=0;
    strcpy(blocks[0].str, "Genesis");
    sockfd=TCPBindToPort(13000);
    GREEN printf("Done!\n");
    Listen(sockfd, 1);

    while(1)
    {
        connfd=Accept(sockfd, NULL, NULL); connected=TRUE;
        if(Read(connfd, &requested, sizeof(int))<0)
            connected=FALSE;
        if(requested>=0)
        {
            if(requested>id)
                requested=id;
            diff=id-requested+1;
            if(Write(connfd, &diff, sizeof(int))<0)
                connected=FALSE;
            for(int i=requested; i<=id; i++)
                if(Write(connfd, &blocks[i], sizeof(block))<0)
                    connected=FALSE;
        }
        while(connected==TRUE)
        {
            FD_ZERO(&fset);
            FD_SET(connfd, &fset);
            select(max(connfd, sockfd)+1, &fset, NULL, NULL, &checker);
            if(FD_ISSET(connfd, &fset))
            {
                if(Read(connfd, &requested, sizeof(int))<0)
                    connected=FALSE;
                if(requested<1)
                    requested=1;
                if(requested>id)
                    requested=id;
                diff=id-requested+1;
                if(Write(connfd, &diff, sizeof(int))<0)
                    connected=FALSE;
                for(int i=requested; i<=id; i++)
                    if(Write(connfd, &blocks[i], sizeof(block))<0)
                        connected=FALSE;
            }
            if(connected==TRUE)
            {
                generateBlock();
                if(Write(connfd, &blocks[id], sizeof(block))<0)
                    connected=FALSE;
            }
        }
        RED printf("M-node disconnected\n");
        YELLOW printf("Waiting for a new one\n");
    }
    
}
