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
   timeout_qpair* data = queue[front++];
	
   if(front == MAX) {
      front = 0;
   }
	
   itemCount--;
   return data;  
}

void printq(){
	
	int start = front;
	while(start <= rear){
		if (start == MAX){
			start = 0;
		}
		printf("router id: %u, timeout: %u\n", queue[start]->r->id, queue[start]->to->tv_sec);	
		start++;
	}
}
