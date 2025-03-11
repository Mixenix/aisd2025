#include "all.h"

void clear(QueueList *q){
	while (!isEmptyL(q)){
		popL(q);
	}
}
