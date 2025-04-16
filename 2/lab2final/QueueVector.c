#include "all.h"

typedef struct Task{
	int ang;
	double res;
	Task *next;
} Task;

typedef struct Queue{
	Task tasks[MAX_SIZE];
	int front;
	int rear;
} Queue;


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

void process(int nOfChnls, int ang1, int ang2, double p1, double p2, double p3, ERROR *glob_err){
	printf("\nНа векторе\n");
	Queue *toChannels = malloc(sizeof(Queue)*nOfChnls);
	if (toChannels == NULL){
		*glob_err = BAD_ALLOC;
		return;
	}
	Queue *toLeader = malloc(sizeof(Queue)*nOfChnls);
	if (toLeader == NULL){
		free(toChannels);
		*glob_err = BAD_ALLOC;
		return;
	}
	ERROR *err = malloc(sizeof(ERROR));
	if (err == NULL){
		*glob_err = BAD_ALLOC;
		free(toChannels);
		free(toLeader);
		return;
	}
	*err = GOOD;
	for (int i = 0; i < nOfChnls; i++) {
		init(&toChannels[i]);
		init(&toLeader[i]);
	}
	int nOfAngles = ang2 - ang1 + 1;
	Task *res = malloc(sizeof(Task)*nOfAngles);
	if (res == NULL){
		*glob_err = BAD_ALLOC;
		free(toLeader);
		free(toChannels);
		free(err);
		}
	for (int i = 0; i < nOfAngles; i++){
		res[i].ang = ang1 + i;
		res[i].res = -10;
		res[i].next = NULL;
	}
	srand(time(NULL));
	int done = 0;
	while (done < nOfAngles) {
		for (int j = 0; j < nOfAngles; j++) {
			if (res[j].res != -10) continue;
			for (int i = 0; i < nOfChnls; i++) {
				double randomvalue = (double)rand() / RAND_MAX;
				if (randomvalue < p1 && isEmpty(&toChannels[i])) {
					Task task = {res[j].ang, 0, NULL};
					put(&toChannels[i], task, err);
					if (*err == BAD_ALLOC){
                   		*glob_err = BAD_ALLOC;
                   	}
					if (*err  == GOOD){break;}
					goto brk;
				}
			}
		}
		for (int i = 0; i < nOfChnls; i++) {
			if (isEmpty(&toChannels[i])) continue;
			double randomvalue = (double)rand() / RAND_MAX;
			if (randomvalue < p2){
				Task task = pop(&toChannels[i]);
				task.res = calcSin(task.ang);
				put(&toLeader[i], task, err);
				if (*err == BAD_ALLOC){
					*glob_err = BAD_ALLOC;
              		goto brk;
              	}
				if (*err == BAD){goto brk;}
			}
		}
		for (int i = 0; i < nOfChnls; i++) {
			while (!isEmpty(&toLeader[i])){
				Task task = pop(&toLeader[i]);
				double randomvalue = (double)rand() / RAND_MAX;
				if (randomvalue < p3){
					for (int j = 0; j < nOfAngles; j++) {
						if (res[j].ang == task.ang && res[j].res == -10) {
							res[j].res = task.res;
							done++;
							break;
						}
					}
				}
			}
		}
	}
	printf("\nРезультат:\n");
	for (int i = 0; i < nOfAngles; i++) {
		printf("%d\t%lf\n", res[i].ang, res[i].res);
	}
	for (int i=0; i<nOfChnls; i++){
		clear(&toLeader[i]);
		clear(&toChannels[i]);
	}
	brk:
	free(res);
	free(toLeader);
	free(toChannels);
	free(err);
	if (*glob_err == BAD_ALLOC){return;}
	*glob_err = GOOD;
}
