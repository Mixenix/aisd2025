#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>


#define BAD -2
#define EXIT -10


int check_N(char *text);
int isInteger(double N);


int get_N(){
	char *text = readline("\nВведите число >=1: ");
	int checker = check_N(text);
	while (checker != 0){
		if (checker == EXIT){return EXIT;}
		free(text);
		text = readline("\nЧто-то не так, введите число >=1: ");
		checker = check_N(text);
	}
	int n = atoi(text);
	free(text);
	return n;
}


int check_N(char *text){
	if (text == NULL){
		return EXIT;
	}
	int slen = strlen(text);
	int OK_notOK = 0;
	if (slen == 0){
		OK_notOK = BAD;
		return OK_notOK;
	}
	for (int i=0; i < slen; i++){
		if (!(isdigit(text[i]))){
			OK_notOK = BAD;
			return OK_notOK;
		}
	}
	if (*text == '0'){
		OK_notOK = BAD;
	}
	return OK_notOK;
}

int isInteger(double N){
    int X = N;
 
    double temp2 = N - X;
 
    if (temp2 > 0.000001) { // fix1
        return 0;
    }
    return 1;
}
