#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structs.h"

#ifndef QUEUESTUFF_H
#define QUEUESTUFF_H
void init(Queue *q);
bool isEmpty(Queue *q);
int put(Queue *q, Task task);
Task pop(Queue *q);
#endif
