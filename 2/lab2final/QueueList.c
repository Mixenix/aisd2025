#include "all.h"

bool isEmpty(Queue *q){
    return q->pfront == NULL;
}

bool isFull(Queue *q){return false;}

void init(Queue *q) {
    q->pfront = NULL;
    q->prear = NULL;
}

void put(Queue *q, Task task, ERROR *err) {
    Task *new_task = (Task *)malloc(sizeof(Task));
    if (new_task == NULL) {
        *err = BAD_ALLOC;
        return;
    }
    new_task->ang = task.ang;
    new_task->res = task.res;
    new_task->next = NULL;

    if (isEmpty(q)) {
        q->pfront = q->prear = new_task;
        *err = GOOD;
        return;
    }
    q->prear->next = new_task;
    q->prear = new_task;
    *err = GOOD;
}

Task pop(Queue *q) {
    if (isEmpty(q)) {
        Task nothing = {-10, -10, NULL};
        return nothing;
    }
    Task *tmp = q->pfront;
    Task task = *tmp;
    q->pfront = q->pfront->next;
    if (q->pfront == NULL) {
        q->prear = NULL;
    }
    free(tmp);
    return task;
}

void clear(Queue *q){
    while (!isEmpty(q)) {
        pop(q);
    }
}
