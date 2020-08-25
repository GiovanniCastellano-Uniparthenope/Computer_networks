#ifndef LISTINT_H_INCLUDED
#define LISTINT_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

typedef struct ListInt{
    int value;
    struct ListInt *pointed;
    struct ListInt *next;
    struct ListInt *prev;
    struct ListInt *last;
}ListInt;

void InitializeI(ListInt **new_list)
{
    (*new_list) = (ListInt*)malloc(sizeof(ListInt));
    (*new_list)->next = (ListInt*)malloc(sizeof(ListInt));
    (*new_list)->next->prev=(*new_list);
    (*new_list)->prev=NULL;
    (*new_list)->next->next=NULL;
    (*new_list)->last=(*new_list)->next;
    (*new_list)->pointed=NULL;
}

ListInt* Push_BackI(ListInt *lis, int val)
{
    ListInt* nod;
    nod = (ListInt*)malloc(sizeof(ListInt));
    nod->prev=lis->last->prev;
    nod->next=lis->last;
    nod->last=lis->last;
    lis->last->prev->next=nod;
    lis->last->prev=nod;
    nod->value=val;
    if(lis->pointed==NULL)
        lis->pointed=nod;
    return nod;
}

int NextI(ListInt* lis)
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

int PreviousI(ListInt* lis)
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

int GetPointedI(ListInt* lis)
{
    if(lis->pointed==NULL)
    {
        return 0;
    }
    else
    {
        return lis->pointed->value;
    }
}

int RemovePointedI(ListInt* lis)
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

int ErasePointedI(ListInt* lis)
{
    ListInt* erasethis;
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

void RollToStartI(ListInt* lis)
{
    while(PreviousI(lis));
}

void RollToEndI(ListInt* lis)
{
    while(NextI(lis));
}


#endif // LIST_H_INCLUDED
