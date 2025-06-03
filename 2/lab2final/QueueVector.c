#include "all.h"

void init(Queue *q){
	q->front = 0;
	q->rear = 0;
}

bool isEmpty(Queue *q){
	return q->front == q->rear;
}

bool isFull(Queue *q){
	return (q->rear + 1) % MAX_SIZE == q->front;
}

void put(Queue *q, Task task, ERROR *err){
	if (isFull(q)){
		printf("\nОчередь полная...");
		*err = BAD;
		return;
	}
	q->tasks[q->rear] = task;
	q->rear = (q->rear+1) % MAX_SIZE;
	*err = GOOD;
	return;
}

Task pop(Queue *q){
	if (isEmpty(q)){
		Task nothing = {-10, -10, NULL};
		return nothing;
	}
	Task task = q->tasks[q->front];
	q->front = (q->front +1) % MAX_SIZE;
	return task;
}

void clear(Queue *q){
	while (!isEmpty(q)){
		pop(q);
	}
}


