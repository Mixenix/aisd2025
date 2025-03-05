#include "QueueStuff.h"

#define BAD -2
#define GOOD 0

void init(Queue *q);
bool isEmpty(Queue *q);
int put(Queue *q, Task task);
Task pop(Queue *q);



bool isEmpty(Queue *q){
	return q->front == NULL;
}


void init(Queue *q){
	q->front = NULL;
	q->rear = NULL;
}

int put(Queue *q, Task task){
	Task *new = (Task *)malloc(sizeof(Task));
	if (new == NULL){
		return BAD;
	}
	new->ang = task.ang;
	new->res = task.res;
	new->next = NULL;

	if (isEmpty(q)){
		q->front = q->rear = new;
		return GOOD;
	}
	q->rear->next = new;
	q->rear = new;
	return GOOD;
}

Task pop(Queue *q){
	if (isEmpty(q)){
		printf("\nОчередь пустая...");
		Task nothing = {-10, -10, NULL};
		return nothing;
	}
	Task *tmp = q->front;
	Task task = *tmp;
	q->front = q->front->next;
	if (q->front == NULL){
		q->rear = NULL;
	}
	free(tmp);
	return task;
}


