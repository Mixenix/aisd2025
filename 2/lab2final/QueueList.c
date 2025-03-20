#include "all.h"

#define BAD -2
#define GOOD 0

struct Task{
	int ang;
	double res;
	Task *next;
};

struct Queue{
	Task *front;
	Task *rear;
};


bool onList(){return true;}


bool isEmpty(Queue *q){
	return q->front == NULL;
}

bool isFull(Queue *q){return false;}

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

void clear(Queue *q){
	while (!isEmpty(q)){
		pop(q);
	}
}

int listProcess(int nOfChnls, int ang1, int ang2, double p1, double p2, double p3){
	Queue *toChannels = malloc(sizeof(Queue)*nOfChnls);
    Queue *toLeader = malloc(sizeof(Queue)*nOfChnls);
    if (toChannels == NULL || toLeader == NULL){
    	return BAD;
    }
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
                    int putter = put(&toChannels[i], task);
                    if (putter == GOOD) {
                        break;
                    }
                    else if (putter == BAD){goto brk;}
                }
            }
        }
        
        for (int i = 0; i < nOfChnls; i++) {
            if (isEmpty(&toChannels[i])) continue;
            double randomvalue = (double)rand() / RAND_MAX;
            if (randomvalue < p2){
                Task task = pop(&toChannels[i]);
                task.res = calcSin(task.ang);
                if (put(&toLeader[i], task) == BAD){goto brk;}
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
}

int vectorProcess(int nOfChnls, int ang1, int ang2, double p1, double p2, double p3){return GOOD;}
// наверху | чтобы занулить функцию, потому что прототип есть, а фактически она не нужна
