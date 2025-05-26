#include "graph.h"

static Queue* createQueue(ERROR* err) {
	Queue* q = malloc(sizeof(Queue));
	if (!q) {
		*err = BAD_ALLOC;
		return NULL;
	}
	q->front = q->rear = NULL;
	q->count = 0;
	*err = GOOD;
	return q;
}

static void enqueue(Queue* q, int vertex_index, ERROR* err) {
	if (!q) { *err = BAD; return; }
	QueueNode* newNode = malloc(sizeof(QueueNode));
	if (!newNode) {
		*err = BAD_ALLOC;
		return;
	}
	newNode->vertex_index = vertex_index;
	newNode->next = NULL;
	if (q->rear == NULL) {
		q->front = q->rear = newNode;
	} else {
		q->rear->next = newNode;
		q->rear = newNode;
	}
	q->count++;
	*err = GOOD;
}

static int dequeue(Queue* q, ERROR* err) {
	if (!q || q->front == NULL) {
		*err = BAD;
		return -1;
	}
	QueueNode* temp = q->front;
	int vertex_index = temp->vertex_index;
	q->front = q->front->next;
	if (q->front == NULL) {
		q->rear = NULL;
	}
	free(temp);
	q->count--;
	*err = GOOD;
	return vertex_index;
}

static bool isQueueEmpty(Queue* q) {
	return (q == NULL || q->front == NULL);
}

static void freeQueue(Queue* q) {
	if (!q) return;
	while (!isQueueEmpty(q)) {
		ERROR temp_err;
		dequeue(q, &temp_err);
	}
	free(q);
}

ERROR isExitReachableBFS(Graph* graph, unsigned int start_x, unsigned int start_y, bool* reachable) {
	if (!graph || !reachable) return BAD;
	*reachable = false;
	ERROR err = GOOD;

	int start_idx = findVertexIndex(graph, start_x, start_y, &err);
	if (err != GOOD || start_idx == -1) return NOT_FOUND;
	if (graph->vertices[start_idx].type != ENTRANCE) return INVALID_INPUT;

	Queue* q = createQueue(&err);
	if (err != GOOD) return err;

	bool* visited = calloc(graph->capacity, sizeof(bool));
	if (!visited) {
		freeQueue(q);
		return BAD_ALLOC;
	}

	enqueue(q, start_idx, &err);
	if (err != GOOD) { free(visited); freeQueue(q); return err; }
	visited[start_idx] = true;

	while (!isQueueEmpty(q)) {
		int u_idx = dequeue(q, &err);
		if (err != GOOD) { free(visited); freeQueue(q); return err; }

		if (graph->vertices[u_idx].type == EXIT_CELL) {
			*reachable = true;
			break;
		}

		AdjListNode* adj_node = graph->vertices[u_idx].head;
		while (adj_node) {
			int v_idx = adj_node->dest_vertex_index;
			if (graph->vertices[v_idx].is_active && !visited[v_idx]) {
				visited[v_idx] = true;
				enqueue(q, v_idx, &err);
				if (err != GOOD) { free(visited); freeQueue(q); return err; }
			}
			adj_node = adj_node->next;
		}
	}

	free(visited);
	freeQueue(q);
	return GOOD;
}

ERROR shortestPathBellmanFord(Graph* graph,
							unsigned int entrance_x, unsigned int entrance_y,
							unsigned int exit_x, unsigned int exit_y,
							int** path_array_out, int* path_len_out) {
	if (!graph || !path_array_out || !path_len_out) return BAD;
	*path_array_out = NULL;
	*path_len_out = 0;
	ERROR err = GOOD;

	int start_idx = findVertexIndex(graph, entrance_x, entrance_y, &err);
	if (err != GOOD || start_idx == -1) return NOT_FOUND;

	int end_idx = findVertexIndex(graph, exit_x, exit_y, &err);
	if (err != GOOD || end_idx == -1) return NOT_FOUND;

	int num_active_vertices = graph->num_vertices;
	if (num_active_vertices == 0) return GOOD;
	int* dist = malloc(graph->capacity * sizeof(int));
	int* pred = malloc(graph->capacity * sizeof(int));
	if (!dist || !pred) {
		free(dist);
		free(pred);
		return BAD_ALLOC;
	}

	for (int i = 0; i < graph->capacity; ++i) {
		dist[i] = INT_MAX;
		pred[i] = -1;
	}
	dist[start_idx] = 0;
	
	for (int i = 0; i < num_active_vertices -1; ++i) {
		bool relaxed_in_iteration = false;
		for (int u = 0; u < graph->capacity; ++u) {
			if (!graph->vertices[u].is_active || dist[u] == INT_MAX) continue;
			AdjListNode* adj_node = graph->vertices[u].head;
			while (adj_node) {
				int v = adj_node->dest_vertex_index;
				if (graph->vertices[v].is_active) {
					if (dist[u] + 1 < dist[v]) {
						dist[v] = dist[u] + 1;
						pred[v] = u;
						relaxed_in_iteration = true;
					}
				}
				adj_node = adj_node->next;
			}
		}
		if (!relaxed_in_iteration && i < num_active_vertices -2) break;
	}

	if (dist[end_idx] == INT_MAX) {
		free(dist); free(pred);
		*path_len_out = 0;
		*path_array_out = NULL;
		return NOT_FOUND;
	}
	int* path_temp = malloc(num_active_vertices * sizeof(int));
	if (!path_temp) { free(dist); free(pred); return BAD_ALLOC; }
	
	int current_path_idx = 0;
	int crawl = end_idx;
	while (crawl != -1 && current_path_idx < num_active_vertices) {
		path_temp[current_path_idx++] = crawl;
		if (crawl == start_idx) break;
		crawl = pred[crawl];
		 if (crawl == -1 && current_path_idx > 0 && path_temp[current_path_idx-1] != start_idx) {
			 free(dist);
			 free(pred);
			 free(path_temp);
			 return BAD;
		 }
	}
	
	if (path_temp[current_path_idx-1] != start_idx) {
		free(dist);
		free(pred);
		free(path_temp);
		*path_len_out = 0;
		*path_array_out = NULL;
		return NOT_FOUND;
	}

	*path_len_out = current_path_idx;
	*path_array_out = malloc((*path_len_out) * sizeof(int));
	if (!(*path_array_out)) {
		free(dist);
		free(pred);
		free(path_temp);
		return BAD_ALLOC;
	}

	for (int i = 0; i < *path_len_out; ++i) {
		(*path_array_out)[i] = path_temp[(*path_len_out) - 1 - i];
	}

	free(dist);
	free(pred);
	free(path_temp);
	return GOOD;
}

ERROR convertToSinglePath(Graph* graph) {
	if (!graph) return BAD;
	if (graph->num_vertices == 0) return GOOD;

	ERROR err = GOOD;
	AdjListNode** new_adj_lists = calloc(graph->capacity, sizeof(AdjListNode*));
	if (!new_adj_lists) return BAD_ALLOC;

	bool* visited = calloc(graph->capacity, sizeof(bool));
	if (!visited) {
		free(new_adj_lists);
		return BAD_ALLOC;
	}

	Queue* q = createQueue(&err);
	if (err != GOOD) {
		free(visited);
		free(new_adj_lists);
		return err;
	}

	for (int i = 0; i < graph->capacity; ++i) {
		if (graph->vertices[i].is_active && !visited[i]) {
			enqueue(q, i, &err);
			if (err != GOOD) goto freedomMinOstTree;
			visited[i] = true;

			while (!isQueueEmpty(q)) {
				int u_idx = dequeue(q, &err);
				if (err != GOOD) goto freedomMinOstTree;

				AdjListNode* current_original_adj = graph->vertices[u_idx].head;
				while (current_original_adj) {
					int v_idx = current_original_adj->dest_vertex_index;
					if (graph->vertices[v_idx].is_active && !visited[v_idx]) {
						visited[v_idx] = true;
						enqueue(q, v_idx, &err);
						if (err != GOOD) goto freedomMinOstTree;
						AdjListNode* mst_edge_node = malloc(sizeof(AdjListNode));
						if (!mst_edge_node) {
							err = BAD_ALLOC;
							goto freedomMinOstTree;
						}
						mst_edge_node->dest_vertex_index = v_idx;
						mst_edge_node->next = new_adj_lists[u_idx];
						new_adj_lists[u_idx] = mst_edge_node;
					}
					current_original_adj = current_original_adj->next;
				}
			}
		}
	}

freedomMinOstTree:
	freeQueue(q);
	free(visited);

	if (err == GOOD) {
		for (int i = 0; i < graph->capacity; ++i) {
			if (graph->vertices[i].is_active) {
				freeAdjList(&(graph->vertices[i].head));
				graph->vertices[i].head = new_adj_lists[i];
			} else {
				 if (new_adj_lists[i] != NULL) freeAdjList(&new_adj_lists[i]);
			}
		}
		free(new_adj_lists);
	} else {
		for (int i = 0; i < graph->capacity; ++i) {
			if (new_adj_lists[i] != NULL) {
				freeAdjList(&new_adj_lists[i]);
			}
		}
		free(new_adj_lists);
	}
	return err;
}
