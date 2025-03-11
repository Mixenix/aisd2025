#include "all.h"

#define BAD -2
#define GOOD 0

void initV(QueueVec *q);
bool isEmptyV(QueueVec *q);
bool isFullV(QueueVec *q);
int putV(QueueVec *q, Task task);
Task popV(QueueVec *q);
void vectorProcess(int nOfChnls, int ang1, int ang2, double p1, double p2, double p3);



void initV(QueueVec *q){
	q->front = 0;
	q->rear = 0;
}

bool isEmptyV(QueueVec *q){
	return q->front == q->rear;
}

bool isFullV(QueueVec *q){
	if ((q->rear + 1) % 1000 == q->front){return true;}
	return false;
}

int putV(QueueVec *q, Task task){
	if (isFullV(q)){
		printf("\nОчередь полная...");
		return BAD;
	}
	q->tasks[q->rear] = task;
	q->rear = (q->rear+1) % 1000;
	return GOOD;
}

Task popV(QueueVec *q){
	if (isEmptyV(q)){
		printf("\nОчередь пустая...");
		Task nothing = {-10, -10, NULL};
		return nothing;
	}
	Task task = q->tasks[q->front];
	q->front = (q->front +1) % 1000;
	return task;
}

void vectorProcess(int nOfChnls, int ang1, int ang2, double p1, double p2, double p3){
	QueueVec toChannels[nOfChnls];
	QueueVec toLeader[nOfChnls];
	for (int i = 0; i < nOfChnls; i++) {
	    initV(&toChannels[i]);
	    initV(&toLeader[i]);
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
	            if (randomvalue < p1 && isEmptyV(&toChannels[i])) {
	                Task task = {res[j].ang, 0, NULL};
	                if (putV(&toChannels[i], task) == GOOD) {
	                    break;
	                }
	                else if (putV(&toChannels[i], task) == BAD){goto brk;}
	            }
	        }
	    }

	    for (int i = 0; i < nOfChnls; i++) {
	        if (isEmptyV(&toChannels[i])) continue;
	        double randomvalue = (double)rand() / RAND_MAX;
	        if (randomvalue < p2) {
	            Task task = popV(&toChannels[i]);
	            task.res = calcSin(task.ang);
	            if (putV(&toLeader[i], task) == BAD){goto brk;}
	        }
	    }

	    for (int i = 0; i < nOfChnls; i++) {
	        while (!isEmptyV(&toLeader[i])) {
	            Task task = popV(&toLeader[i]);
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

