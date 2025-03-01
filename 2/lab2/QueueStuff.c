#include <stdio.h>
#include <stdbool.h>
#include "structs.h"

void init(Queue *q);
bool isEmpty(Queue *q);
bool isFull(Queue *q);
void put(Queue *q, Task task);
Task pop(Queue *q);



void init(Queue *q){
	q->front = 0;
	q->rear = 0;
}

bool isEmpty(Queue *q){
	if (q->front == q->rear){return true;}
	return false;
}

bool isFull(Queue *q){
	if ((q->rear + 1) % 1000 == q->front){return true;}
	return false;
}

void put(Queue *q, Task task){
	if (isFull(q)){
		printf("\nОчередь полная...");
		return;
	}
	q->tasks[q->rear] = task;
	q->rear = (q->rear+1) % 1000;
}

Task pop(Queue *q){
	if (isEmpty(q)){
		printf("\nОчередь пустая...");
		Task nothing = {-10, -10};
		return nothing;
	}
	Task task = q->tasks[q->front];
	q->front = (q->front +1) % 1000;
	return task;
}


