#include "all.h"

#define BAD -2
#define GOOD 0

struct Task{
	int ang;
	double res;
	Task *next;
};

struct Queue{
	Task tasks[1000];
	int front;
	int rear;
};


bool onList(){return false;}


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
		Task nothing = {-10, -10, NULL};
		return nothing;
	}
	Task task = q->tasks[q->front];
	q->front = (q->front +1) % 1000;
	return task;
}

void vectorProcess(int nOfChnls, int ang1, int ang2, double p1, double p2, double p3){
	Queue toChannels[nOfChnls];
	Queue toLeader[nOfChnls];
	for (int i = 0; i < nOfChnls; i++) {
	    init(&toChannels[i]);
	    init(&toLeader[i]);
	}

	int nOfAngles = ang2 - ang1 + 1;
	Task res[nOfAngles];
	for (int i = 0; i < nOfAngles; i++) {
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
	                if (put(&toChannels[i], task) == GOOD) {
	                    break;
	                }
	                else if (put(&toChannels[i], task) == BAD){goto brk;}
	            }
	        }
	    }

	    for (int i = 0; i < nOfChnls; i++) {
	        if (isEmpty(&toChannels[i])) continue;
	        double randomvalue = (double)rand() / RAND_MAX;
	        if (randomvalue < p2) {
	            Task task = pop(&toChannels[i]);
	            task.res = calcSin(task.ang);
	            if (put(&toLeader[i], task) == BAD){goto brk;}
	        }
	    }

	    for (int i = 0; i < nOfChnls; i++) {
	        while (!isEmpty(&toLeader[i])) {
	            Task task = pop(&toLeader[i]);
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

	printf("\nРезультат:\n");
	for (int i = 0; i < nOfAngles; i++) {
	    printf("%d\t%lf\n", res[i].ang, res[i].res);
	}
	brk:
}

void listProcess(int nOfChnls, int ang1, int ang2, double p1, double p2, double p3){return;}
// здесь то же самое, что и в QueueList.c

