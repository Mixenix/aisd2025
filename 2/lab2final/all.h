#include <stdio.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <stdlib.h>
#include "time.h"

#define MAX_SIZE 10000

typedef struct Task Task;
typedef struct Queue Queue;

typedef struct Task{
    int ang;
    double res;
    Task *next;
} Task;

typedef struct Queue{
	Task tasks[MAX_SIZE];
	int front;
	int rear;
	Task *pfront;
    Task *prear;
} Queue;

typedef int ERROR;

#define INV "Введите через пробел кол-во ведомых узлов, угол1, угол2 и вероятности p1, p2, p3:\n"
#define GOOD 0
#define BAD -2
#define EXIT -100
#define SIZE_EXCEEDED -4
#define QUEUE_EMPTY -5
#define BAD_ALLOC -11



#ifndef QUEUE_H
#define QUEUE_H
void init(Queue *q);
bool isEmpty(Queue *q);
bool isFull(Queue *q);
void put(Queue *q, Task task, ERROR *err);
void clear(Queue *q);
Task pop(Queue *q);
void process(int nOfChnls, int ang1, int ang2, double p1, double p2, double p3, ERROR *glob_err);
#endif


#ifndef INPUT_H
#define INPUT_H
void input(int *nOfChnls, int *ang1, int *ang2, double *p1, double *p2, double *p3, ERROR *glob_err);
void check_Line(char *str, int *nOfChnls, int *ang1, int *ang2, double *p1, double *p2, double *p3, ERROR *err);
bool check_Number(char *text);
bool isInteger(double N);
#endif


#ifndef SINCALC_H
#define SINCALC_H
double calcSin(int angle);
#endif
