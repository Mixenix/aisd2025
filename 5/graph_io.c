#include "graph.h"

char* readLineSimple( char* prompt){
	printf("%s", prompt);
	char buffer[MAX_COORD_STR_LEN * 2];
	if (fgets(buffer, sizeof(buffer), stdin) == NULL){
		return NULL;
	}
	buffer[strcspn(buffer, "\n")] = 0;
	if (strlen(buffer) == 0 && feof(stdin)) return NULL;
	return strdup(buffer);
}

ERROR getUnsignedInt(unsigned int* value, char* msg){
	char* input_str = NULL;
	unsigned long temp_val;
	char* endptr;

	while (1){
		free(input_str);
		input_str = readLineSimple(msg);
		if (input_str == NULL) return EXIT;

		if (strlen(input_str) == 0){
			printf(WRONG_INP);
			continue;
		}
		
		bool valid_digits = true;
		for(int k=0; input_str[k] != '\0'; ++k){
			if(!isdigit(input_str[k])){
				valid_digits = false;
				break;
			}
		}
		if(!valid_digits){
			printf(WRONG_INP);
			continue;
		}

		temp_val = strtoul(input_str, &endptr, 10);

		if (*endptr != '\0' || temp_val > UINT_MAX){
			printf(WRONG_INP);
		}
		else{
			*value = (unsigned int)temp_val;
			free(input_str);
			return GOOD;
		}
	}
}

ERROR getCoordinates(unsigned int* x, unsigned int* y,  char* msg_x,  char* msg_y){
	ERROR err;
	printf("\nВведите координаты: \n");
	err = getUnsignedInt(x, msg_x);
	if (err != GOOD) return err;
	err = getUnsignedInt(y, msg_y);
	return err;
}

ERROR getCellType(CellType* type,  char* msg){
	char* input_str = NULL;
	int choice;
	unsigned int temp_choice;

	while (1){
		printf("%s (0: Обычная, 1: Вход, 2: Выход): ", msg);
		ERROR err = getUnsignedInt(&temp_choice, ""); 
		if (err == EXIT){ free(input_str); return EXIT; }
		if (err != GOOD){
			 printf(WRONG_INP);
			 continue;
		}
		choice = (int)temp_choice;

		if (choice >= 0 && choice <= 2){
			*type = (CellType)choice;
			free(input_str);
			return GOOD;
		} else{
			printf(WRONG_INP);
		}
	}
}
