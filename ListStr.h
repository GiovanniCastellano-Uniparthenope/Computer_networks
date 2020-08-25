#ifndef LISTSTR_H_INCLUDED
#define LISTSTR_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

#define IPSIZE 15

typedef struct ListStr{
    char      str[IPSIZE+1];
    struct ListStr *pointed;
    struct ListStr    *next;
    struct ListStr    *prev;
    struct ListStr    *last;
}ListStr;

void InitializeS(ListStr **new_list)
{
    (*new_list) = (ListStr*)malloc(sizeof(ListStr));
    (*new_list)->next = (ListStr*)malloc(sizeof(ListStr));
    (*new_list)->next->prev=(*new_list);
    (*new_list)->prev=NULL;
    (*new_list)->next->next=NULL;
    (*new_list)->last=(*new_list)->next;
    (*new_list)->pointed=NULL;
}

ListStr* Push_BackS(ListStr *lis, char *val)
{
    ListStr* nod;
    nod = (ListStr*)malloc(sizeof(ListStr));
    nod->prev=lis->last->prev;
    nod->next=lis->last;
    nod->last=lis->last;
    lis->last->prev->next=nod;
    lis->last->prev=nod;
    for(int i=0; i<IPSIZE+1; i++)
    {
        nod->str[i]=val[i];
    }
    if(lis->pointed==NULL)
        lis->pointed=nod;
    return nod;
}

int NextS(ListStr* lis)
{
    if(lis->pointed==NULL)
    {
        if(lis->next->next!=NULL)
        {
            lis->pointed=lis->next;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if(lis->pointed->next->next!=NULL)
        {
            lis->pointed=lis->pointed->next;
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

int PreviousS(ListStr* lis)
{
    if(lis->pointed==NULL)
    {
        if(lis->last->prev->prev!=NULL)
        {
            lis->pointed=lis->last->prev;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if(lis->pointed->prev->prev!=NULL)
        {
            lis->pointed=lis->pointed->prev;
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

int GetPointedS(ListStr* lis, char yourstr[])
{
    if(lis->pointed==NULL)
    {
        return 0;
    }
    else
    {
        for(int i=0; i<IPSIZE+1; i++)
        {
            yourstr[i]=lis->pointed->str[i];
        }
        return 1;
    }
    
}

int RemovePointedS(ListStr* lis)
{
    if(lis->pointed!=NULL)
    {
        lis->pointed->prev->next=lis->pointed->next;
        lis->pointed->next->prev=lis->pointed->prev;
        if(lis->pointed->prev->prev!=NULL)
        {
            lis->pointed=lis->pointed->prev;
            return 1;
        }
        else
        {
            if(lis->pointed->next->next!=NULL)
            {
                lis->pointed=lis->pointed->next;
                return 1;
            }
            else
            {
                lis->pointed=NULL;
                return 2;
            }
        }
    }
    else
    {
        return 0;
    }
}

int ErasePointedS(ListStr* lis)
{
    ListStr* erasethis;
    if(lis->pointed!=NULL)
    {
        erasethis=lis->pointed;
        lis->pointed->prev->next=lis->pointed->next;
        lis->pointed->next->prev=lis->pointed->prev;
        if(lis->pointed->prev->prev!=NULL)
        {
            lis->pointed=lis->pointed->prev;
            free(erasethis);
            return 1;
        }
        else
        {
            if(lis->pointed->next->next!=NULL)
            {
                lis->pointed=lis->pointed->next;
                free(erasethis);
                return 1;
            }
            else
            {
                lis->pointed=NULL;
                free(erasethis);
                return 2;
            }
        }
    }
    else
    {
        return 0;
    }
}

void RollToStartS(ListStr* lis)
{
    while(PreviousS(lis));
}

void RollToEndS(ListStr* lis)
{
    while(NextS(lis));
}


#endif // LIST_H_INCLUDED
