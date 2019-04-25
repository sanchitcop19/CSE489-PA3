#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/Queue.h"
#include "../include/global.h"

/* https://codereview.stackexchange.com/questions/141238/implementing-a-generic-queue-in-c*/

void qinit(Queue *q, size_t memSize)
{
   q->sizeOfQueue = 0;
   q->memSize = memSize;
   q->head = q->tail = NULL;
}

int push(Queue *q, const void *data)
{
    node *newNode = (node *)malloc(sizeof(node));

    if(newNode == NULL)
    {
        return -1;
    }

    newNode->data = malloc(q->memSize);

    if(newNode->data == NULL)
    {
        free(newNode);
        return -1;
    }

    newNode->next = NULL;

    memcpy(newNode->data, data, q->memSize);

    if(q->sizeOfQueue == 0)
    {
        q->head = q->tail = newNode;
    }
    else
    {
        q->tail->next = newNode;
        q->tail = newNode;
    }

    q->sizeOfQueue++;
    return 0;
}

void pop(Queue *q, void *data)
{
    if(q->sizeOfQueue > 0)
    {
        node *temp = q->head;
	if (data != NULL)
        memcpy(data, temp->data, q->memSize);

        if(q->sizeOfQueue > 1)
        {
            q->head = q->head->next;
        }
        else
        {
            q->head = NULL;
            q->tail = NULL;
        }

        q->sizeOfQueue--;
        free(temp->data);
        free(temp);
    }
}

void peek(Queue *q, void *data)
{
    if(q->sizeOfQueue > 0)
    {
       node *temp = q->head;
       memcpy(data, temp->data, q->memSize);
    }
}

void printq(Queue *q)
{
  node *temp;
int size = q->sizeOfQueue;
      temp = q->head;
	if (size == 0){printf("[]\n");return;}
printf("[");
	if (temp){
	timeout_qpair* p;
	p = (timeout_qpair*)(temp->data);
	printf("(router id: %hu, timeout: %lu), ", (p->r)->id, (p->to)->tv_sec);
	if (temp->next){
	timeout_qpair* p;
	p = (timeout_qpair*)(temp->next->data);
	printf("(router id: %hu, timeout: %lu), ", (p->r)->id, (p->to)->tv_sec);
printf("]\n");
	if (temp->next->next){
	timeout_qpair* p;
	p = (timeout_qpair*)(temp->next->next->data);
	printf("(router id: %hu, timeout: %lu), ", (p->r)->id, (p->to)->tv_sec);
printf("]\n");
	if (temp->next->next->next){
	timeout_qpair* p;
	p = (timeout_qpair*)(temp->next->next->next->data);
	printf("(router id: %hu, timeout: %lu), ", (p->r)->id, (p->to)->tv_sec);
printf("]\n");
	if (temp->next->next->next->next){
	timeout_qpair* p;
	p = (timeout_qpair*)(temp->next->next->next->next->data);
	printf("(router id: %hu, timeout: %lu), ", (p->r)->id, (p->to)->tv_sec);
printf("]\n");
	}
	}
	}
	}
	}

}
void clearq(Queue *q)
{
  node *temp;

  while(q->sizeOfQueue > 0)
  {
      temp = q->head;
      q->head = temp->next;
      free(temp->data);
      free(temp);
      q->sizeOfQueue--;
  }

  q->head = q->tail = NULL;
}

int size(Queue *q)
{
    return q->sizeOfQueue;
}
