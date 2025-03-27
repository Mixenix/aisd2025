#include <stdio.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <stdlib.h>
#include "time.h"

typedef struct Task Task;
typedef struct Queue Queue;


#ifndef QUEUE_H
#define QUEUE_H
void init(Queue *q);
bool isEmpty(Queue *q);
bool isFull(Queue *q);
int put(Queue *q, Task task);
Task pop(Queue *q);
int typeOstuff();
int process(int nOfChnls, int ang1, int ang2, double p1, double p2, double p3);
#endif


#ifndef INPUT_H
#define INPUT_H
int input(int *nOfChnls, int *ang1, int *ang2, double *p1, double *p2, double *p3);
int check_Line(char *str, int *nOfChnls, int *ang1, int *ang2, double *p1, double *p2, double *p3);
bool check_Number(char *text);
bool isInteger(double N);
#endif


#ifndef SINCALC_H
#define SINCALC_H
double calcSin(int angle);
#endif
