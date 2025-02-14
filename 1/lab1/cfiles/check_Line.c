#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include "check_Number.h"
#include "structs.h"




bool check_Number(char *text);


#define TRUE 1
#define FALSE 0




int check_Line(char *str, Line *ln){
	if (str[0] == '\0'){
		return -1;
	}
	char *srk = strdup(str);
	char *token = strtok(srk, " ");
	bool isOk = TRUE;
	// Line *ln = malloc(sizeof(Line)*1);
	// (*ln).a = malloc(sizeof((*ln).a));
	int cnt = 0;
	double *elems = malloc(0);
	// printf("%d", sizeof(elems) / sizeof(double));
	while (token != NULL){
		int checker = check_Number(token);
		// printf("\nbooL:%d", checker);
		if (checker == 0){
			isOk = FALSE;
			free(srk);
			free(token);
			free(elems);
			return -1;
		}
		double a = atof(token);
		elems = realloc(elems, sizeof(double)+sizeof(double)*cnt);
		elems[cnt] = a;
		cnt++;
		token = strtok(NULL, " ");
	}
	free(srk);
	free(token);
	if (isOk == TRUE){
		if (cnt != (*ln).n){
			free(elems);
			return -1;
		}
		for (int i=0; i<cnt; i++){
			(*ln).a[i] = elems[i];
		}
		free(elems);
		return 0; // (good)
	}
}

