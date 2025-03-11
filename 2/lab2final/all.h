#include <stdio.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <stdlib.h>
#include "structs.h"
#include "time.h"


#ifndef QUEUELIST_H
#define QUEUELIST_H
void initL(QueueList *q);
bool isEmptyL(QueueList *q);
bool isFullL(QueueList *q);
int putL(QueueList *q, Task task);
Task popL(QueueList *q);
void listProcess(int nOfChnls, int ang1, int ang2, double p1, double p2, double p3);
#endif


#ifndef QUEUEVEC_H
#define QUEUEVEC_H
void initV(QueueVec *q);
bool isEmptyV(QueueVec *q);
bool isFullV(QueueVec *q);
int putV(QueueVec *q, Task task);
Task popV(QueueVec *q);
void vectorProcess(int nOfChnls, int ang1, int ang2, double p1, double p2, double p3);
#endif


#ifndef CLEAR_H
#define CLEAR_H
void clear(QueueList *q);
#endif


#ifndef INPUT_H
#define INPUT_H
int get_N();
int input(int *VecOrList, int *nOfChnls, int *ang1, int *ang2, double *p1, double *p2, double *p3);
int check_Line(char *str, int *VecOrList, int *nOfChnls, int *ang1, int *ang2, double *p1, double *p2, double *p3);
#endif


#ifndef SINCALC_H
#define SINCALC_H
double calcSin(int angle);
#endif
