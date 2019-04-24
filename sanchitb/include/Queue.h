#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

typedef struct Node
{
  void *data;
  struct Node *next;
}node;

typedef struct QueueList
{
    int sizeOfQueue;
    size_t memSize;
    node *head;
    node *tail;
}Queue;

void qinit(Queue *q, size_t memSize);
int push(Queue *, const void *);
void pop(Queue *, void *);
void peek(Queue *, void *);
void clearq(Queue *);
int size(Queue *);

#endif /* QUEUE_H_INCLUDED */
