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




