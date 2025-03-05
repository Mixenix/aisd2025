#include "QueueStuff.h"

void clear(Queue *q){
	while (!isEmpty(q)){
		pop(q);
	}
}
