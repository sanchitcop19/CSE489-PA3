#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/global.h"

#define MAX 5

int front = 0;
int rear = -1;
int itemCount = 0;

timeout_qpair* peek() {
   return queue[front];
}

bool isEmpty() {
   return itemCount == 0;
}

bool isFull() {
   return itemCount == MAX;
}

int size() {
   return itemCount;
}  

void push(timeout_qpair* data) {

   if(!isFull()) {
	
      if(rear == MAX-1) {
         rear = -1;            
      }       

      queue[++rear] = data;
      itemCount++;
   }
}

timeout_qpair* pop() {
	int delete = front;
   timeout_qpair* data = queue[front++];
queue[delete] = NULL;	
   if(front == MAX) {
      front = 0;
   }
	
   itemCount--;
   return data;  
}

void printq(){
	
	int start = front;
	while(start%5 != (rear+1)%5){
		if (queue[start%5])printf("index: %i, router id: %u, timeout: %u\n",(start%5) , queue[start%5]->r->id, queue[start%5]->to->tv_sec);	
		start++;
	}
}

int getfront(){
	return front;
}

int getrear(){
	return rear;
}
