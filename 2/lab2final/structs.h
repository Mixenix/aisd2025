typedef struct Task {
    int ang;
    double res;
    struct Task* next;
} Task;


typedef struct QueueVec {
	Task tasks[1000];
	int front;
	int rear;
} QueueVec;


typedef struct QueueList {
    Task* front;
    Task* rear;
} QueueList;


