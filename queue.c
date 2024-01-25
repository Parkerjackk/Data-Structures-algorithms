#include "t1.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

Queue* create_queue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) {
        return 0;
    }

    queue->front = NULL;
    queue->rear = NULL;

    return queue;
}


bool isEmpty(Queue* queue)
{
    return queue->front == NULL;
}

void enqueue(Queue* queue, int x)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = x;
    newNode->next = NULL;

    if (isEmpty(queue)) //if the queue is empty, the new node will be inserted in the front.
    {
        queue->front = newNode;
        queue->rear = newNode;
    }
    else //otherwise, it will be placed behind the end of the queue
    {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

int dequeue(Queue* queue)
{
    if (isEmpty(queue))
    {
        return -1;
    }
    int data = queue->front->data;
    
    Node* tmp = queue->front; // create a temp pointer on the front of the queue
    queue->front = queue->front->next; // set the front to fronts next
    
    if (queue->front == NULL){
        queue->rear = NULL;
    }

    free(tmp);

    return data;
}

void delete_queue(Queue* queue) 
{
    while (!isEmpty(queue)) 
    {
        dequeue(queue);
    }

    free(queue);
}