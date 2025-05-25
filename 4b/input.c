#include "tree.h"

ERROR input_string(char** str_val, char* prompt) {
	if (str_val == NULL) return BAD;

	char* line = readline(prompt);
	if (!line) {
		*str_val = NULL;
		return EXIT;
	}

	if (strlen(line) == 0) {
		*str_val = line; 
		if (*str_val) {
			free(*str_val);
		}
		*str_val = line;
		return GOOD;
	}

	if (*str_val && *str_val != line) {
		free(*str_val);
	}
	*str_val = line;

	return GOOD;
}

ERROR input_int_action(int* action, char* prompt) {
	char* line = readline(prompt);
	if (!line) return EXIT;

	char* endptr;
	long val;

	errno = 0;
	val = strtol(line, &endptr, 10);

	if (line == endptr || *endptr != '\0') {
		free(line);
		return INVALID_INPUT;
	}
	if (errno == ERANGE || val > INT_MAX || val < INT_MIN) {
		free(line);
		return INVALID_INPUT;
	}

	free(line);
	*action = (int)val;
	return GOOD;
}

ERROR input_unsigned_int(unsigned int* value, char* prompt) {
	char* line = readline(prompt);
	if (!line) return EXIT;

	char* p = line;
	while (*p && isspace((unsigned char)*p)) p++;
	if (*p == '-') {
		free(line);
		return INVALID_INPUT;
	}
	
	char* endptr;
	unsigned long val_ul;

	errno = 0;
	val_ul = strtoul(line, &endptr, 10);

	if (line == endptr || *endptr != '\0') {
		free(line);
		return INVALID_INPUT;
	}
	if (errno == ERANGE || val_ul > UINT_MAX) {
		free(line);
		return INVALID_INPUT;
	}
	
	free(line);
	*value = (unsigned int)val_ul;
	return GOOD;
}
