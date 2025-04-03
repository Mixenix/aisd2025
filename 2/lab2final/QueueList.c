#include "all.h"


struct Task{
	int ang;
	double res;
	Task *next;
};

struct Queue{
	Task *front;
	Task *rear;
};


int typeOstuff(){return 0;} // 0 - list, 1 - vector 


bool isEmpty(Queue *q){
	return q->front == NULL;
}

bool isFull(Queue *q){return false;}

void init(Queue *q){
	q->front = NULL;
	q->rear = NULL;
}

void put(Queue *q, Task task, ERROR *err){
	Task *new = (Task *)malloc(sizeof(Task));
	if (new == NULL){
		*err = BAD_ALLOC;
		return;
	}
	new->ang = task.ang;
	new->res = task.res;
	new->next = NULL;

	if (isEmpty(q)){
		q->front = q->rear = new;
		*err = GOOD;
		return;
	}
	q->rear->next = new;
	q->rear = new;
	*err = GOOD;
	return;
}

Task pop(Queue *q, ERROR *err){
	if (isEmpty(q)){
		printf("\nОчередь пустая...");
		*err = QUEUE_EMPTY;
		// Task nothing = {-10, -10, NULL};
		// return nothing;
		return NULL;
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

void clear(Queue *q){
	while (!isEmpty(q)){
		pop(q);
	}
}

int process(int nOfChnls, int ang1, int ang2, double p1, double p2, double p3){
	Queue *toChannels = malloc(sizeof(Queue)*nOfChnls);
	Queue *toLeader = malloc(sizeof(Queue)*nOfChnls);
	if (toChannels == NULL || toLeader == NULL){
		return BAD;
	}
	ERROR *err = malloc(sizeof(ERROR));
	*err = GOOD;
	for (int i = 0; i < nOfChnls; i++) {
		init(&toChannels[i]);
		init(&toLeader[i]);
	}
	int nOfAngles = ang2 - ang1 + 1;
	Task *res = malloc(sizeof(Task)*nOfAngles);
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
					if (put(&toChannels[i], task, err) == GOOD){break;}
					goto brk;
				}
			}
		}
		for (int i = 0; i < nOfChnls; i++) {
			if (isEmpty(&toChannels[i])) continue;
			double randomvalue = (double)rand() / RAND_MAX;
			if (randomvalue < p2){
				Task task = pop(&toChannels[i], err);
				task.res = calcSin(task.ang);
				if (put(&toLeader[i], task, err) == BAD){goto brk;}
			}
		}
		for (int i = 0; i < nOfChnls; i++) {
			while (!isEmpty(&toLeader[i])){
				Task task = pop(&toLeader[i], err);
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
}
