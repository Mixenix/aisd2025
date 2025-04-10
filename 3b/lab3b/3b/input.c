#include "all.h"



int input(unsigned int *action, char *INV, int kindOf){
	char *inp = readline(INV);
	if (inp == NULL){
		printf("\nexiting///");
		free(inp);
		return EXIT;
	}
	if (check_Number(inp) == false){
		printf("\nЧто-то не так, ещё раз...");
		free(inp);
		return BAD;
	}
	double f = atof(inp);
	if (isInteger(f) == false){
		free(inp);
		return BAD;
	}
	free(inp);
	switch (kindOf){
		case KEY:
			*action = (unsigned int)f;
			break;
		case INFO:
			*action = (unsigned int)f;
			break;
		case CHOICE:
			*action = (int)f;
			break;
		case REL:
			*action = (int)f;
			break;
	}
	return GOOD;
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
    if (first_character == '-'){return false;}
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
    int X = N;
 
    double temp2 = N - X;
 	return temp2 < EPSILON;
}
