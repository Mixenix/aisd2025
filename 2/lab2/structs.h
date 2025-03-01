typedef struct {
	int ang;
	double res;
} Task;


typedef struct {
	Task tasks[1000];
	int front;
	int rear;
} Queue;


