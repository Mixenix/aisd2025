#include <math.h>


double calcSin(int angle){
    double radians = angle * M_PI / 180.0;
    return sin(radians);
}
