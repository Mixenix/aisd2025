#include "all.h"

#define GOOD 0
#define BAD -2
#define EXIT -10
#define INV "Введите через пробел кол-во ведомых узлов, угол1, угол2 и вероятности p1, p2, p3:\n"

int main() {
    int nOfChnls;
    int ang1;
    int ang2;
    double p1, p2, p3;
    int inp = GOOD;
    while (inp != EXIT) {
    	int realtype = onList(); // по умолчанию - на массиве
        inp = input(&nOfChnls, &ang1, &ang2, &p1, &p2, &p3);
        if (inp == EXIT) return 0;
		if (realtype == false){
			printf("\n.На массиве");
	        vectorProcess(nOfChnls, ang1, ang2, p1, p2, p3);
		}
		else{
			printf("\n.На списке");
			listProcess(nOfChnls, ang1, ang2, p1, p2, p3);
        }
    }
    return 0;
}
