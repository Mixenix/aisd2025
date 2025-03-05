#include "QueueStuff.h"

#define BAD -2
#define GOOD 0

void init(Queue *q);
bool isEmpty(Queue *q);
bool isFull(Queue *q);
int put(Queue *q, Task task);
Task pop(Queue *q);



void init(Queue *q){
	q->front = 0;
	q->rear = 0;
}

bool isEmpty(Queue *q){
	return q->front == q->rear;
}

bool isFull(Queue *q){
	if ((q->rear + 1) % 1000 == q->front){return true;}
	return false;
}

int put(Queue *q, Task task){
	if (isFull(q)){
		printf("\nОчередь полная...");
		return BAD;
	}
	q->tasks[q->rear] = task;
	q->rear = (q->rear+1) % 1000;
	return GOOD;
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


