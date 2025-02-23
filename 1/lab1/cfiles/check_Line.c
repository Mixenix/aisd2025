#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include "check_Number.h"
#include "structs.h"




bool check_Number(char *text);


#define BAD -1 // fix1
#define GOOD 0 // fix1




int check_Line(char *str, Line *ln){
	if (str[0] == '\0'){
		return BAD;
	}
	char *srk = strdup(str);
	char *token = strtok(srk, " ");
	bool isOk = true;
	int cnt = 0;
	double *elems = malloc(0);
	while (token != NULL){
		int checker = check_Number(token);
		if (checker == 0){
			isOk = false;
			free(srk);
			free(token);
			free(elems);
			return BAD;
		}
		double a = atof(token);
		elems = realloc(elems, sizeof(double)+sizeof(double)*cnt);
		if (elems == NULL){ return BAD;}
		elems[cnt] = a;
		cnt++;
		token = strtok(NULL, " ");
	}
	free(srk);
	free(token);
	if (isOk == true){
		if (cnt != (*ln).n){
			free(elems);
			return BAD;
		}
		for (int i=0; i<cnt; i++){
			(*ln).a[i] = elems[i];
		}
		free(elems);
		return GOOD; // (good)
	}
	return BAD; // fix1
}

