#include "check_Line.h"
#include "check_N.h"
#include <stdio.h>

void output(Matrix matr, int numbofLines){
	for (int i=0; i<numbofLines; i++){
		int lnlength = ((matr.matr)[i].n);
		printf("\nN=%d", lnlength);
		for (int j=0; j<lnlength; j++){
			double chs = (matr.matr)[i].a[j];
			printf(", ");
			if (isInteger(chs) == 1){
				printf("%d", (int)chs);
			}
			else{
				printf("%f", chs);
			}
		}
	}
	printf("\n");
}
