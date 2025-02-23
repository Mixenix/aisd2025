#include "check_Line.h"
#include <stdlib.h>

void erase (Matrix *pmtr){
	int numbofLines = pmtr->lines;
	for (int i=0; i<numbofLines; i++){
		free(((*pmtr).matr)[i].a);
	}
	free(pmtr->matr);
}
