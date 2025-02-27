#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "check_N.h"
#include "input.h"
#include "output.h"
#include "alg.h"
#include "erase.h"

#define BAD -1
#define EXIT -10



int check_Line(char *str, Line *ln);
int get_N();




int main(){
	printf("\nВведите кол-во строк матрицы.");
	int numbofLines = get_N();
	Line *lns = malloc(numbofLines*sizeof(Line));
	Matrix matr = {numbofLines, lns};
	// int inp = input(lns, numbofLines);
	int inp = input(&matr);
	if (inp == EXIT || inp == BAD){
		printf("\nexiting...");
		free(lns);
		// exit(0)
		return -1;
	}
	printf("\n\nИсходная матрица:\n");
	output(&matr);
	alg(&matr);
	printf("\n\nПосле применения алгоритма:\n");
	output(&matr);
	erase(&matr);
	return 0;
}



