#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "check_Line.h"
// #include "check_Number.h"
#include "check_N.h"
// #include "structs.h"

#define BAD -1
#define EXIT -10

// int input(Line *lns, int numbofLines){
int input(Matrix *pmtr){
	Line *lns = pmtr->matr;
	int numbofLines = pmtr->lines;
	char *str;
	Line *pln;
	double *line_elems;
	for (int i=0; i<numbofLines; i++){
		printf("\nВведите кол-во элементов в строке.");
		int n = get_N();
		if (n == EXIT){
			return EXIT;
		}
		line_elems = malloc(sizeof(double)*n);
		if (line_elems == NULL){
			printf("Ошибка выделения памяти");
			return BAD;
		}
		Line ln = {n, line_elems};
		pln = &ln;
		
		
		str = readline("\n> ");
		if (str == NULL){
			printf("\nexiting...\n");
			free(str);
			free(line_elems);
			free(lns);
			exit(0);
		}
		int test = check_Line(str, pln);
		while (test != 0){
			printf("\nНекорректный ввоd, попробуйте ещё раз: ");
			free(str);
			str = readline("\n> ");
			if (str == NULL){
				printf("\nexiting...\n");
				free(str);
				free(lns);
				exit(0);
			}
			test = check_Line(str, pln);
		}
		lns[i] = *pln;
		free(str);
	}
	return 0;
	
}
