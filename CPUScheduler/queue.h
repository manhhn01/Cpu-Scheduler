#ifndef item_queue
#define item_queue int
#endif

#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

typedef struct QueueNode
{
    item_queue data;
    QueueNode *next;
} QueueNode;

typedef struct Queue
{
    QueueNode *front = NULL, *rear = NULL;
    int count = 0;
} Queue;

QueueNode *createQueueNode(item_queue x)
{
    QueueNode *temp = (QueueNode *)malloc(sizeof(QueueNode));
    temp->data = x;
    temp->next = NULL;
    return temp;
}

void queuePush(Queue *Q, item_queue x)
{
    QueueNode *P = createQueueNode(x);
    if (Q->count == 0)
    {
        Q->front = Q->rear = P;
    }
    else
    {
        Q->rear->next = P;
        Q->rear = P;
    }
    Q->count++;
}

item_queue queuePop(Queue *Q)
{
    if (Q->count == 0)
    {
        return -1;
    }
    else
    {
        item_queue temp = Q->front->data;
        QueueNode* popPtr = Q->front;        
        if (Q->count != 1)
        {
            Q->front = Q->front->next;
        }
        else
        {
            Q->front = NULL;
            Q->rear = NULL;
        }
        Q->count--;
        free(popPtr);
        return temp;
    }
}

#endif