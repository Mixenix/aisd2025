#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <time.h>
#include "input.h"
#include "sinCalc.h"
#include "QueueStuff.h"
#include "clear.h"

#define GOOD 0
#define BAD -2
#define EXIT -10
#define INV "Введите через пробел: кол-во ведомых узлов, угол1, угол2 и вероятности p1, p2, p3:\n"

int main(){
	int nOfChnls;
	int ang1;
	int ang2;
	double p1, p2, p3;
	int inp = 0;
	while (inp != EXIT){
		inp = input(&nOfChnls, &ang1, &ang2, &p1, &p2, &p3);		
		if (inp == EXIT){
			return 0;
		}
		Queue toChannels[nOfChnls];
		Queue toLeader[nOfChnls];
		for (int i=0; i<nOfChnls; i++){
			init(&toChannels[i]);
			init(&toLeader[i]);
			
		}
		int nOfAngles = ang2 - ang1 + 1;
		Task res[nOfAngles];
		for (int i=0; i<nOfAngles; i++){
			res[i].ang = ang1 + i;
			res[i].res = -10;
			res[i].next = NULL;
		}
		srand(time(NULL));
		int done = 0;
		while (done < nOfAngles){
			for (int i=0; i<nOfChnls; i++){
				if (((double)rand() / RAND_MAX < p1)){
					for (int j=0; j <nOfAngles; j++){
						if (res[j].res == -10){
							Task task = {res[j].ang, 0, NULL};
							if (put(&toChannels[i], task) == BAD){goto brk;}
							break;
						}
					}
				}
				if (((double)rand() / RAND_MAX < p2) && (!isEmpty(&toChannels[i]))){
					Task task = pop(&toChannels[i]);
					task.res = calcSin(task.ang);
					if (put(&toLeader[i], task) == BAD){goto brk;}
				}

				if (((double)rand() / RAND_MAX < p3) && (!isEmpty(&toLeader[i]))){
					Task task = pop(&toLeader[i]);
					for (int j=0; j<nOfAngles; j++){
						if (res[j].ang == task.ang){
							res[j].res = task.res;
							done++;
							break;
						}
					}
				}
			}
		}
		printf("\nРезультат\n");
		for (int i=0; i<nOfAngles; i++){
			printf("%d\t%lf\n", res[i].ang, res[i].res);
		}
		for (int i=0; i<nOfChnls; i++){
			clear(&toLeader[i]);
			clear(&toChannels[i]);
		}
		brk:
	}
	return 0;
}
