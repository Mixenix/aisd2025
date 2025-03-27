#include "all.h"

#define GOOD 0
#define BAD -2
#define EXIT -10
#define INV "Введите через пробел кол-во ведомых узлов, угол1, угол2 и вероятности p1, p2, p3:\n"

int main(){
	int nOfChnls;
	int ang1;
	int ang2;
	double p1, p2, p3;
	int inp = GOOD;
	while (inp != EXIT) {
		int realtype = typeOstuff();
		inp = input(&nOfChnls, &ang1, &ang2, &p1, &p2, &p3);
		if (inp == EXIT) return 0;
		switch (realtype){
		case 0:
			printf("\nНа списке");
			break;
		case 1:
			printf("\nНа массиве");
			break;			
		}
		if (process(nOfChnls, ang1, ang2, p1, p2, p3) == BAD){
			printf("\nerror, exit...");
			return 1;
		}
	}
    return 0;
}


