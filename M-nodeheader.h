#ifndef MNODEHEADER_H_INCLUDED
#define MNODEHEADER_H_INCLUDED

#include "GeneralHeader.h"
#include "Binder.h"
#include "Connection.h"
#include <pthread.h>

int  nblocks, Nfd, Nonline, total;

void* Nconnect()
{
    block trash;
    int inc;
    do
    {
        sleep(2);
        Nfd=TCPConnection(13000, "127.0.0.1");   
    }while(Nfd<0);
    Write(Nfd, &nblocks, sizeof(int));
    Read(Nfd, &inc, sizeof(int));
    for(int i=0; i<inc; i++)
    {
        Read(Nfd, &trash, sizeof(block));
    }
    GREEN printf("Connected to a new N-node!\n");
    Nonline=TRUE;
    total=0;
}

void Exit()
{
    int n=-1;
    if(Nonline==TRUE)
        Write(Nfd, &n, sizeof(int));
    shutdown(Nfd, 0);
    exit(0);
}

void addBlock(block **list, block *singleblock)
{
    int count=0, i=0, nval;
    char value[5]; value[4]='\0';
    nblocks++;
    *list=realloc(*list, sizeof(block)*nblocks);
    (*list+nblocks-1)->id=singleblock->id;
    (*list+nblocks-1)->time=singleblock->time;
    strcpy((*list+nblocks-1)->str, singleblock->str);
    if(singleblock->str[0]!='G')
    {
        while(count<2)
        {
            i++;
            if(singleblock->str[i]==':')
                count++;
        }
        i++;
        for(int j=0; j<4; j++)
        {
            value[j]=singleblock->str[i++];
        }
        nval=StrToInt(value);
        if(total+nval>total)
        total+=nval;
    }
    GREEN printf("New block");
    WHITE printf(" | id: %d | time: %d\ntransaction: %s\n", (*list+nblocks-1)->id, (*list+nblocks-1)->time, (*list+nblocks-1)->str);
}

int matchIPTransaction(matchblock *singleblock, block **listblock, ListInt *matching)
{
    int match=0, count=0, i=0, k=0;
    for(int j=1; j<nblocks; j++)  
    {  
        count=0;
        i=0;
        k=0;
        do
        {
            if((*listblock+j)->str[i]==':')
                count++;
            if(singleblock->IP[i]!=(*listblock+j)->str[i])
                break;
            else
                i++;
        }
        while(count<2);
        if(count>=2)
        {
            Push_BackI(matching, j);
            match++;
        }
        else
        {
            count=0;
            i=0;
            while(count<3){if((*listblock+j)->str[i++]==':'){count++;}}
            count=0;
            do
            {
                if((*listblock+j)->str[i]==':')
                    count++;
                if(singleblock->IP[k++]!=(*listblock+j)->str[i])
                    break;
                else
                    i++;
            }
            while(count<2);
            if(count>=2)
            {
                Push_BackI(matching, j);
                match++;
            }
        }
    }
    return match;
}

#endif
