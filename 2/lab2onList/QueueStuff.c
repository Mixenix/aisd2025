#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structs.h"


void init(Queue *q);
bool isEmpty(Queue *q);
void put(Queue *q, Task task);
Task pop(Queue *q);



bool isEmpty(Queue *q){
	if (q->front == NULL){return true;}
	return false;
}


void init(Queue *q){
	q->front = NULL;
	q->rear = NULL;
}

void put(Queue *q, Task task){
	Task *new = (Task *)malloc(sizeof(Task));
	new->ang = task.ang;
	new->res = task.res;
	new->next = NULL;

	if (isEmpty(q)){
		q->front = q->rear = new;
		return;
	}
	q->rear->next = new;
	q->rear = new;
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


