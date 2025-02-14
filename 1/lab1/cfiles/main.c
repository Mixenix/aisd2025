#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "check_N.h"
#include "input.h"
#include "output.h"
#include "alg.h"
#include "erase.h"


int check_Line(char *str, Line *ln);
int get_N();
void alg(Matrix *pmtr, int numbofLines);




int main(){
	printf("\nВведите кол-во строк матрицы.");
	int numbofLines = get_N();
	if (numbofLines == -2){
		printf("\nexiting...\n");
		exit(0);
	}
	Line *lns = malloc(numbofLines*sizeof(Line));
	Matrix matr = {numbofLines, lns};
	input(lns, numbofLines);
	printf("\n\nИсходная матрица:\n");
	output(matr, numbofLines);
	alg(&matr, numbofLines);
	printf("\n\nПосле применения алгоритма:\n");
	output(matr, numbofLines);
	erase(&matr, numbofLines);
	free(lns);
	return 0;
}



