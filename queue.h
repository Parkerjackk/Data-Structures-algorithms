#ifndef QUEUE_H_
#define QUEUE_H_
#include <stdio.h>
#include <stdbool.h>
#include "t1.h"

typedef struct Queue{
    
    struct Node *front, *rear;

}Queue; 

Queue *create_queue();
void enqueue(Queue* q, int data);
int dequeue(Queue* q);
bool isEmpty(Queue* q);
void delete_queue(Queue *q);

#endif 