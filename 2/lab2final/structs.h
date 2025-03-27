#define MAX_SIZE 10000

typedef struct Task {
    int ang;
    double res;
    struct Task* next;
} Task;


typedef struct QueueVec {
	Task tasks[MAX_SIZE];
	int front;
	int rear;
} QueueVec;


typedef struct QueueList {
    Task* front;
    Task* rear;
} QueueList;


