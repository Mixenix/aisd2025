#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "all.h"


#define CHNLS 0
#define DEG1 1
#define DEG2 2
#define POS1 3
#define POS2 4
#define POS3 5
#define EPSILON 0.000000000001
#define DELTA 1.0


void input(int *nOfChnls, int *ang1, int *ang2, double *p1, double *p2, double *p3, ERROR *glob_err){
	char *inp = readline(INV);
	if (inp == NULL){
		free(inp);
		*glob_err = EXIT;
		return;
	}
	ERROR *err = malloc(sizeof(ERROR));
	if (err == NULL){
		*glob_err = BAD_ALLOC;
		return;
	}
	*err = GOOD;
	check_Line(inp, nOfChnls, ang1, ang2, p1, p2, p3, err);
	while (*err != GOOD){
		printf("\nНекорректный формат, попробуйте ещё раз (или bad alloc!):\n");
		free(inp);
		inp = readline(INV);
		if (inp == NULL){
			free(err);
			*glob_err = EXIT;
			return;
		}
		check_Line(inp, nOfChnls, ang1, ang2, p1, p2, p3, err);
	}
	free(err);
	free(inp);
	*glob_err = GOOD;
}


void check_Line(char *str, int *nOfChnls, int *ang1, int *ang2, double *p1, double *p2, double *p3, ERROR *err){
	if (str[0] == '\0'){
		*err = BAD;
		return;
	}
	char *srk = strdup(str);
	if (srk == NULL){
		*err = BAD_ALLOC;
		return;
	}
	char *token = strtok(srk, " ");
	if (token == NULL){
		*err = BAD_ALLOC;
		free(srk);
		return;
	}
	int cntr = 0;
	while (token != NULL){
		if (cntr == CHNLS){
			if (check_Number(token) == false){
				free(srk);
				*err = BAD;
				return;
			}
			double f = atof(token);
			if (isInteger(f) == false){
				free(srk);
				*err = BAD;
				return;
			}
			*nOfChnls = (int)f;
		}
		else if (cntr == DEG1){
			if (check_Number(token) == false){
				free(srk);
				*err = BAD;
				return;
			}
			double f = atof(token);
			if (isInteger(f) == false){
				free(srk);
				*err = BAD;
				return;
			}
			*ang1 = (int)f;
		}
		else if (cntr == DEG2){
			if (check_Number(token) == false){
				free(srk);
				*err = BAD;
				return;
			}
			double f = atof(token);
			if ((isInteger(f) == false) || (f < *ang1) || (f < 0)){
				free(srk);
				*err = BAD;
				return;
			}
			*ang2 = (int)f;
		}
		else if(cntr == POS1){
			if (check_Number(token) == false){
				free(srk);
				*err = BAD;
				return;
			}
			double f = atof(token);
			if ((f < EPSILON) || (f > DELTA)){
				free(srk);
				*err = BAD;
				return;
			}
			*p1 = f;
		}
		else if(cntr == POS2){
			if (check_Number(token) == false){
				free(srk);
				*err = BAD;
				return;
			}
			double f = atof(token);
			if ((f < EPSILON) || (f > DELTA)){
				free(srk);
				*err = BAD;
				return;
			}
			*p2 = f;
		}
		else if(cntr == POS3){
			if (check_Number(token) == false){
				free(srk);
				*err = BAD;
				return;
			}
			double f = atof(token);
			if ((f < EPSILON) || (f > DELTA)){
				free(srk);
				*err = BAD;
				return;
			}
			*p3 = f;
		}
		else if(cntr > POS3){
			free(srk);
			*err = BAD;
			return;
		}
		token = strtok(NULL, " ");	
		cntr++;		
	}
	if (cntr <= POS3){
		free(srk);
		free(token);
		*err = BAD;
		return;
	}
	free(srk);
	free(token);
	*err = GOOD;
}

bool check_Number(char *text) {
    if (text == NULL || text[0] == '\0') {
        return false;
    }

    int dot_counter = 0;
    size_t length = 1;
    for (char character = text[1]; character != '\0';
         ++length, character = text[length]) {
        bool const is_valid_character =
            (character >= '0' && character <= '9') ||
            (character == '.' && ++dot_counter == 1);

        if (is_valid_character == false) {
            return false;
        }
    }

    char const first_character = text[0];
    bool is_character_sign = (first_character == '-' || first_character == '+');
    if ((is_character_sign || first_character == '.') && length == 1) {
        return false;
    }
    if (length == 2 && is_character_sign && text[1] == '.') {
        return false;
    }
    return (is_character_sign || first_character == '.') ||
           (first_character >= '0' && first_character <= '9');
}


bool isInteger(double N){
 	return N - (int)N < EPSILON;
}
