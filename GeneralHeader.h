#ifndef GENERALHEADER_H_INCLUDED
#define GENERALHEADER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h> //O_CREAT
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <signal.h>

#include "ListStr.h"
#include "ListInt.h"

#define     TRUE        1
#define     FALSE       0

#define     TO_INT      &207
#define     TO_CHAR     |48
#define     DOWNCASE    |32
#define     UPPERCASE   &223

#define     GREEN   printf("\033[1;32m");
#define     RED     printf("\033[1;31m");
#define     BLUE    printf("\033[1;34m");
#define     YELLOW  printf("\033[1;33m");
#define     WHITE   printf("\033[0;0m");

#define max(x, y) ({typeof (x) x_ = (x); typeof (y) y_ = (y); \
x_ > y_ ? x_ : y_;}) 

enum{DISCONNECT, NTRANSACTIONS, IDINFO, TOTAL, IPPORTINFO};

typedef struct matchblock{
    char IP[25];
}matchblock;

typedef struct block{
    int id;
    int time;
    char str[100];
}block;

int StrToInt(char str[])
{
    int j=0, k=0, val=0;
    while(str[j]!='\0'){j++;} j--;
    for(j; j>=0; j--){val+=(str[j] TO_INT)*pow(10, k++);}
    return val;
}

int IPcompare(char *str1, char *str2)
{
    int different=0;
    for(int i=0; i<IPSIZE; i++){
    if(str1[i]!=str2[i]){different++;}
    }return different;
}

void readTransaction(char str[],char* IP1,char* IP2,int* port1,int *port2,int *value, int *randval)
{
    int i=0;
    char P1[10], P2[10], val[30], random[4];
    int j=0;
    while(str[i]!=':')
    {
        IP1[j++]=str[i++];
    }
    IP1[j]='\0'; j=0; i++;
    while(str[i]!=':')
    {
        P1[j++]=str[i++];
    }
    P1[j]='\0'; j=0; i++;
    while(str[i]!=':')
    {
        val[j++]=str[i++];
    }
    val[j]='\0'; j=0; i++;
    while(str[i]!=':')
    {
        IP2[j++]=str[i++];
    }
    IP2[j]='\0'; j=0; i++;
    while(str[i]!=':')
    {
        P2[j++]=str[i++];
    }
    P2[j]='\0'; j=0; i++;
    while(str[i]!=':')
    {
        random[j++]=str[i++];
    }
    random[j]='\0';
    *port1=StrToInt(P1);
    *port2=StrToInt(P2);
    *value=StrToInt(val);
    *randval=StrToInt(random);    
}






















#endif
