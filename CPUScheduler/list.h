#ifndef item_list
#define item_list int
#endif

#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct ListNode
{
    item_list data;
    ListNode *next;
} ListNode;

typedef ListNode *List;


ListNode *createListNode(item_list x)
{
    ListNode *temp = (ListNode *)malloc(sizeof(ListNode));
    temp->data = x;
    temp->next = NULL;
    return temp;
}

void listAdd(List *L, item_list x)
{
    ListNode *P = createListNode(x);
    if (*L == NULL)
        *L = P;
    else
    {
        ListNode *Q = *L;
        while (Q->next != NULL)
        {
            Q = Q->next;
        }
        Q->next = P;
    }
}

item_list listGet(List L, int index)
{
    int i = 0;
    ListNode *P = L;
    while (P->next != NULL && i != index)
    {
        i++;
        P = P->next;
    }
    return P->data;
}

item_list *listGetPtr(List L, int index)
{
    int i = 0;
    ListNode *P = L;
    while (P->next != NULL && i != index)
    {
        i++;
        P = P->next;
    }
    return &P->data;
}

item_list listGetLast(List L)
{
    if (L == NULL)
        return {0};
    ListNode *P = L;
    while (P->next != NULL)
        P = P->next;
    return P->data;
}

int listLength(List L)
{
    ListNode *P = L;
    int length = 0;
    while (P != NULL)
    {
        length++;
        P = P->next;
    }
    return length;
}


#endif