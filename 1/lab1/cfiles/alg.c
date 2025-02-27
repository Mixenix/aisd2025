#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "check_Line.h"
#include "check_N.h"


void alg(Matrix *pmtr){
	int numbofLines = pmtr->lines;
	for (int i=0; i< numbofLines; i++){
		int lnlength = pmtr->matr[i].n;
		int firstI = -1; // prev < i
		int lastJ = -1;  // prev > j
		double tmp;
		for (int j=1; j< lnlength; j++){
			if (firstI == -1){
				if (pmtr->matr[i].a[j-1] < pmtr->matr[i].a[j]){
					firstI = j;
					tmp = (pmtr->matr)[i].a[j];
				}
			}
			if (pmtr->matr[i].a[j-1] > pmtr->matr[i].a[j]){
				lastJ = j;
			}
		}
		if ((firstI == -1) || (lastJ == -1)){
			return;
		}
		else{
			pmtr->matr[i].a[firstI] = pmtr->matr[i].a[lastJ];
			pmtr->matr[i].a[lastJ] = tmp;
		}
	}
}
