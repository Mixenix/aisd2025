#include "all.h"


int main(){
	int nOfChnls;
	int ang1;
	int ang2;
	double p1, p2, p3;
	int inp = GOOD;
	ERROR *glob_err = malloc(sizeof(ERROR));
	*glob_err = GOOD;
	while (*glob_err != EXIT) {
		if (*glob_err == BAD_ALLOC){
			printf("\nBad alloc, exiting...");
			free(glob_err);
			return 1;
		}
		input(&nOfChnls, &ang1, &ang2, &p1, &p2, &p3, glob_err);
		if (*glob_err == EXIT){
			free(glob_err);
			return 0;
		}
		process(nOfChnls, ang1, ang2, p1, p2, p3, glob_err);
		if (*glob_err == BAD_ALLOC){
			printf("\nBad alloc, exiting...");
			free(glob_err);
			return 1;
		}
	}
	free(glob_err);
    return 0;
}




void process(int nOfChnls, int ang1, int ang2, double p1, double p2, double p3, ERROR *glob_err){
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
