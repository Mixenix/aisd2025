typedef struct Task {
    int ang;
    double res;
    struct Task* next;
} Task;


typedef struct Queue {
    Task* front;
    Task* rear;
} Queue;
