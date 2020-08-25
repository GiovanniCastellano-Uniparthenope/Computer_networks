#ifndef NNODEHEADER_H_INCLUDED
#define NNODEHEADER_H_INCLUDED

#include "GeneralHeader.h"
#include "Binder.h"
#include "time.h"

char sock1[23], sock2[23], sock3[23], sock4[23];

block *blocks;
int id, connected;

void sigpipe()
{
    connected=0;
}

void generateBlock()
{
    blocks=realloc(blocks, sizeof(block)*(id+2));
    id++;
    int seed=time(NULL), tme;
    srand(seed);
    tme=5+rand()%10;
    int s1=1+rand()%3, s2=1+rand()%3;
    while(s2==s1)
    {
        s2=1+rand()%3;
    }
    char str[100];
    int i=0, j, count=0;
    switch(s1)
    {
        case 1 : while(count<2){if(sock1[i]==':'){count++;} str[i]=sock1[i]; i++;}
        case 2 : while(count<2){if(sock2[i]==':'){count++;} str[i]=sock2[i]; i++;}
        case 3 : while(count<2){if(sock3[i]==':'){count++;} str[i]=sock3[i]; i++;}
        case 4 : while(count<2){if(sock4[i]==':'){count++;} str[i]=sock4[i]; i++;}
    }
    for(j=0; j<4; j++)
    {
        str[i++]=(1+rand()%8) TO_CHAR;
    }
    str[i++]=':';
    j=-1;
    count=0;
    switch(s2)
    {
        case 1 : while(count<2){if(sock1[++j]==':'){count++;} str[i++]=sock1[j];}
        case 2 : while(count<2){if(sock2[++j]==':'){count++;} str[i++]=sock2[j];}
        case 3 : while(count<2){if(sock3[++j]==':'){count++;} str[i++]=sock3[j];}
        case 4 : while(count<2){if(sock4[++j]==':'){count++;} str[i++]=sock4[j];}
    }
    for(j=0; j<3; j++)
    {
        str[i]=(1+rand()%8) TO_CHAR; 
        i++;
    }
    str[i]=':';
    BLUE printf("Waiting %d seconds\n", tme);
    sleep(tme);
    
    blocks[id].id=id;
    blocks[id].time=tme;
    for(j=0; j<=i; j++)
        blocks[id].str[j]=str[j];
}

void generateSockets()
{
    int num, seed=time(NULL), i=0, counter=0;
    srand(seed);
    strcpy(sock1, "127.0.0.1xx:15xxy:");
    strcpy(sock2, "127.0.0.1xx:15xxy:");
    strcpy(sock3, "127.0.0.1xx:15xxy:");
    strcpy(sock4, "127.0.0.1xx:15xxy:");
    
    i=0;
    while(counter<2)
    {
        if(sock1[i]=='x')
        {
            num=rand()%9;
            sock1[i]=num TO_CHAR;
        }
        else if(sock1[i]=='y')
        {
            num=1+rand()%8;
            sock1[i]=num TO_CHAR;
        }
        else if(sock1[i]==':')
        {
            counter++;
        }
        i++;
    }
    counter=0;
    i=0;
    while(counter<2)
    {
        if(sock2[i]=='x')
        {
            num=rand()%9;
            sock2[i]=num TO_CHAR;
        }
        else if(sock2[i]=='y')
        {
            num=1+rand()%8;
            sock2[i]=num TO_CHAR;
        }
        else if(sock2[i]==':')
        {
            counter++;
        }
        i++;
    }
    counter=0;
    i=0;
    while(counter<2)
    {
        if(sock3[i]=='x')
        {
            num=rand()%9;
            sock3[i]=num TO_CHAR;
        }
        else if(sock3[i]=='y')
        {
            num=1+rand()%8;
            sock3[i]=num TO_CHAR;
        }
        else if(sock3[i]==':')
        {
            counter++;
        }
        i++;
    }
    counter=0;
    i=0;
    while(counter<2)
    {
        if(sock4[i]=='x')
        {
            num=rand()%9;
            sock4[i]=num TO_CHAR;
        }
        else if(sock4[i]=='y')
        {
            num=1+rand()%8;
            sock4[i]=num TO_CHAR;
        }
        else if(sock4[i]==':')
        {
            counter++;
        }
        i++;
    }
    GREEN printf("Created a new block\n");
}


#endif
