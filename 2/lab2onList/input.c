#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>


#define INV "Введите через пробел: кол-во ведомых узлов, угол1, угол2 и вероятности p1, p2, p3:\n"
#define EXIT -10
#define BAD -2
#define GOOD 0
#define CHNLS 0
#define DEG1 1
#define DEG2 2
#define POS1 3
#define POS2 4
#define POS3 5

bool check_Number(char *text);
bool isInteger(double N);
int check_Line(char *str, int *nOfChnls, int *ang1, int *ang2, double *p1, double *p2, double *p3);

int input(int *nOfChnls, int *ang1, int *ang2, double *p1, double *p2, double *p3){
	char *inp = readline(INV);
	if (inp == NULL){
		printf("\nexiting///");
		free(inp);
		return EXIT;
	}
	int chck = check_Line(inp, nOfChnls, ang1, ang2, p1, p2, p3);
	while (chck	!= GOOD){
		printf("\nНекорректный формат, попробуйте ещё раз:\n");
		// printf(INV);
		free(inp);
		inp = readline(INV);
		if (inp == NULL){
			printf("\nexiting///");
			free(inp);
			return EXIT;
		}
		chck = check_Line(inp, nOfChnls, ang1, ang2, p1, p2, p3);
	}
	free(inp);
	return GOOD;
}


// int get_N(){
// 	char *text = readline("\nВведите число >=1: ");
// 	int checker = check_N(text);
// 	while (checker != 0){
// 		if (checker == EXIT){return EXIT;}
// 		free(text);
// 		text = readline("\nЧто-то не так, введите число >=1: ");
// 		checker = check_N(text);
// 	}
// 	int n = atoi(text);
// 	free(text);
// 	return n;
// }

int check_Line(char *str, int *nOfChnls, int *ang1, int *ang2, double *p1, double *p2, double *p3){
	if (str[0] == '\0'){
		return BAD;
	}
	char *srk = strdup(str);
	char *token = strtok(srk, " ");
	// bool isOk = true;
	int cntr = 0;
	int cnt = 0;
	bool checker = false;
	// double *elems = NULL;
	while (token != NULL){
		if (cntr == CHNLS){
			checker = check_Number(token);
			if (checker == false){
				free(srk);
				// free(token);
				return BAD;
			}
			double f = atof(token);
			checker = isInteger(f);
			if (checker == false){
				printf("hahFFFFa");
				free(srk);
				// free(token);
				return BAD;
			}
			// if ((int)f <= 0){
			// 	free(srk);
			// 	// free(token);
			// 	return BAD;
			// }
			*nOfChnls = (int)f;
		}
		else if (cntr == DEG1){
			checker = check_Number(token);
			if (checker == false){
				free(srk);
				// free(token);
				return BAD;
			}
			double f = atof(token);
			checker = isInteger(f);
			if (checker == false){
				free(srk);
				// free(token);
				return BAD;
			}
			*ang1 = (int)f;
		}
		else if (cntr == DEG2){
			checker = check_Number(token);
			if (checker == false){
				free(srk);
				// free(token);
				return BAD;
			}
			double f = atof(token);
			checker = isInteger(f);
			if ((checker == false) || (f < *ang1) || (f < 0)){
				free(srk);
				// free(token);
				return BAD;
			}
			*ang2 = (int)f;
		}
		else if(cntr == POS1){
			checker = check_Number(token);
			if (checker == false){
				free(srk);
				// free(token);
				return BAD;
			}
			double f = atof(token);
			if ((f < 0.000000000001) || (f > 1.0000001)){
				free(srk);
				// free(token);
				return BAD;
			}
			*p1 = f;
		}
		else if(cntr == POS2){
			checker = check_Number(token);
			if (checker == false){
				free(srk);
				// free(token);
				return BAD;
			}
			double f = atof(token);
			if ((f < 0.000000000001) || (f > 1.0)){
				free(srk);
				// free(token);
				return BAD;
			}
			*p2 = f;
		}
		else if(cntr == POS3){
			checker = check_Number(token);
			if (checker == false){
				free(srk);
				// free(token);
				return BAD;
			}
			double f = atof(token);
			if ((f < 0.000000000001) || (f > 1.0)){
				free(srk);
				// free(token);
				return BAD;
			}
			*p3 = f;
		}
		else if(cntr > POS3){
			free(srk);
			return BAD;
		}
		token = strtok(NULL, " ");	
		cntr++;		
	}
	if (cntr <= POS3){
		// printf()
		free(srk);
		free(token);
		return BAD;
	}
	free(srk);
	free(token);
	return GOOD;
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
    // verifications for first left character
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
    int X = N;
 
    double temp2 = N - X;
 
    if (temp2 > 0.000000001) { // fix1
        return false;
    }
    return true;
}
