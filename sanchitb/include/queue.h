#ifndef QUEUE_H
#define QUEUE_H
#include "global.h"
timeout_qpair* peek();
bool isEmpty();
bool isFull();
int size();
void push(timeout_qpair* data);
timeout_qpair* pop();
void printq();
#endif
