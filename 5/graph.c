#include "graph.h"

void freeAdjList(AdjListNode** head_ref){
	AdjListNode* current = *head_ref;
	AdjListNode* next_node;
	while (current != NULL){
		next_node = current->next;
		free(current);
		current = next_node;
	}
	*head_ref = NULL;
}

Graph* createGraph(int initial_capacity, ERROR* err){
	if (initial_capacity <= 0){
		initial_capacity = INITIAL_CAPACITY;
	}
	Graph* graph = malloc(sizeof(Graph));
	if (!graph){
		*err = BAD_ALLOC;
		return NULL;
	}
	graph->vertices = malloc(sizeof(Vertex) * initial_capacity);
	if (!graph->vertices){
		free(graph);
		*err = BAD_ALLOC;
		return NULL;
	}
	graph->num_vertices = 0;
	graph->capacity = initial_capacity;
	for (int i = 0; i < initial_capacity; ++i){
		graph->vertices[i].is_active = false;
		graph->vertices[i].head = NULL;
	}
	*err = GOOD;
	return graph;
}

void freeGraph(Graph* graph){
	if (!graph) return;
	for (int i = 0; i < graph->capacity; ++i){
		if (graph->vertices[i].is_active){
			freeAdjList(&(graph->vertices[i].head));
		}
	}
	free(graph->vertices);
	free(graph);
}

int findVertexIndex(Graph* graph, unsigned int x, unsigned int y, ERROR* err){
	if (!graph){
		*err = BAD;
		return -1;
	}
	for (int i = 0; i < graph->capacity; ++i){
		if (graph->vertices[i].is_active &&
			graph->vertices[i].x == x &&
			graph->vertices[i].y == y){
			*err = GOOD;
			return i;
		}
	}
	*err = NOT_FOUND;
	return -1;
}

ERROR addVertex(Graph* graph, unsigned int x, unsigned int y, CellType type){
	if (!graph) return BAD;
	ERROR find_err = GOOD;
	if (findVertexIndex(graph, x, y, &find_err) != -1){
		return ALREADY_EXISTS;
	}

	int insert_idx = -1;
	for (int i = 0; i < graph->capacity; ++i){
		if (!graph->vertices[i].is_active){
			insert_idx = i;
			break;
		}
	}

	if (insert_idx == -1){
		int new_capacity = graph->capacity * 2;
		Vertex* new_vertices = realloc(graph->vertices, sizeof(Vertex) * new_capacity);
		if (!new_vertices) return BAD_ALLOC;
		graph->vertices = new_vertices;
		for (int i = graph->capacity; i < new_capacity; ++i){
			graph->vertices[i].is_active = false;
			graph->vertices[i].head = NULL;
		}
		insert_idx = graph->capacity;
		graph->capacity = new_capacity;
	}

	graph->vertices[insert_idx].x = x;
	graph->vertices[insert_idx].y = y;
	graph->vertices[insert_idx].type = type;
	graph->vertices[insert_idx].head = NULL;
	graph->vertices[insert_idx].is_active = true;
	graph->num_vertices++;
	return GOOD;
}

ERROR addEdge(Graph* graph, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2){
	if (!graph) return BAD;
	ERROR err = GOOD;
	int src_idx = findVertexIndex(graph, x1, y1, &err);
	if (err != GOOD || src_idx == -1) return NOT_FOUND;

	int dest_idx = findVertexIndex(graph, x2, y2, &err);
	if (err != GOOD || dest_idx == -1) return NOT_FOUND;

	unsigned int dx = (x1 > x2) ? (x1 - x2) : (x2 - x1);
	unsigned int dy = (y1 > y2) ? (y1 - y2) : (y2 - y1);
	if (!((dx == 1 && dy == 0) || (dx == 0 && dy == 1))){
		return INVALID_OPERATION;
	}

	AdjListNode* current = graph->vertices[src_idx].head;
	while (current){
		if (current->dest_vertex_index == dest_idx){
			return ALREADY_EXISTS;
		}
		current = current->next;
	}

	AdjListNode* newNode = malloc(sizeof(AdjListNode));
	if (!newNode) return BAD_ALLOC;
	newNode->dest_vertex_index = dest_idx;
	newNode->next = graph->vertices[src_idx].head;
	graph->vertices[src_idx].head = newNode;

	return GOOD;
}

ERROR removeVertex(Graph* graph, unsigned int x, unsigned int y){
	if (!graph) return BAD;
	ERROR err = GOOD;
	int v_idx = findVertexIndex(graph, x, y, &err);
	if (err != GOOD || v_idx == -1) return NOT_FOUND;

	graph->vertices[v_idx].is_active = false;
	freeAdjList(&(graph->vertices[v_idx].head));
	graph->num_vertices--;
	for (int i = 0; i < graph->capacity; ++i){
		if (graph->vertices[i].is_active){
			AdjListNode* current = graph->vertices[i].head;
			AdjListNode* prev = NULL;
			while (current != NULL){
				if (current->dest_vertex_index == v_idx){
					if (prev == NULL){
						graph->vertices[i].head = current->next;
					} else{
						prev->next = current->next;
					}
					AdjListNode* temp = current;
					current = current->next;
					free(temp);
				} else{
					prev = current;
					current = current->next;
				}
			}
		}
	}
	return GOOD;
}

ERROR removeEdge(Graph* graph, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2){
	if (!graph) return BAD;
	ERROR err = GOOD;
	int src_idx = findVertexIndex(graph, x1, y1, &err);
	if (err != GOOD || src_idx == -1) return NOT_FOUND;

	int dest_idx = findVertexIndex(graph, x2, y2, &err);
	if (err != GOOD || dest_idx == -1) return NOT_FOUND;

	AdjListNode* current = graph->vertices[src_idx].head;
	AdjListNode* prev = NULL;
	bool found_edge = false;

	while (current != NULL){
		if (current->dest_vertex_index == dest_idx){
			if (prev == NULL){
				graph->vertices[src_idx].head = current->next;
			} else{
				prev->next = current->next;
			}
			free(current);
			found_edge = true;
			break; 
		}
		prev = current;
		current = current->next;
	}

	return found_edge ? GOOD : NOT_FOUND;
}

ERROR changeVertexData(Graph* graph, unsigned int x, unsigned int y, CellType new_type){
	if (!graph) return BAD;
	ERROR err = GOOD;
	int v_idx = findVertexIndex(graph, x, y, &err);
	if (err != GOOD || v_idx == -1) return NOT_FOUND;

	graph->vertices[v_idx].type = new_type;
	return GOOD;
}

char* cellTypeToString(CellType type){
	switch (type){
		case NORMAL: return "Обычная";
		case ENTRANCE: return "Вход";
		case EXIT_CELL: return "Выход";
		default: return "Что-то";
	}
}

void printGraphAdjList(Graph* graph, ERROR* err){
	if (!graph){
		*err = BAD;
		printf("\nГраф неинициализирован\n");
		return;
	}
	if (graph->num_vertices == 0){
		printf("Граф пуст\n");
		*err = GOOD;
		return;
	}
	printf("\n|||Списки смежности\n");
	for (int i = 0; i < graph->capacity; ++i){
		if (graph->vertices[i].is_active){
			printf("Вершина %d [(%u,%u), %s]: -> ",
				   i,
				   graph->vertices[i].x,
				   graph->vertices[i].y,
				   cellTypeToString(graph->vertices[i].type));
			AdjListNode* current = graph->vertices[i].head;
			if (!current){
				printf("NULL");
			}
			while (current){
				printf("%d[(%u,%u)] ",
					   current->dest_vertex_index,
					   graph->vertices[current->dest_vertex_index].x,
					   graph->vertices[current->dest_vertex_index].y);
				if(current->next) printf("-> ");
				current = current->next;
			}
			printf("\n");
		}
	}
	*err = GOOD;
}

void exportGraphToDot(Graph* graph, char* filename, ERROR* err){
	if (!graph){
		*err = BAD;
		return;
	}
	FILE* file = fopen(filename, "w");
	if (!file){
		*err = BAD;
		return;
	}

	fprintf(file, "digraph Labyrinth{\n");
	fprintf(file, "\tnode [fontname=\"Arial\", shape=record];\n");
	fprintf(file, "\trankdir=LR;\n");

	for (int i = 0; i < graph->capacity; ++i){
		if (graph->vertices[i].is_active){
			fprintf(file, "\t\"v%d\" [label=\"{%d | (%u,%u) | %s}\"];\n",
					i, i, graph->vertices[i].x, graph->vertices[i].y,
					cellTypeToString(graph->vertices[i].type));
		}
	}

	for (int i = 0; i < graph->capacity; ++i){
		if (graph->vertices[i].is_active){
			AdjListNode* current = graph->vertices[i].head;
			while (current){
				if (graph->vertices[current->dest_vertex_index].is_active){
					fprintf(file, "\t\"v%d\" -> \"v%d\";\n", i, current->dest_vertex_index);
				}
				current = current->next;
			}
		}
	}

	fprintf(file, "}\n");
	fclose(file);
	*err = GOOD;
}

void visualizeGraph(Graph* graph, ERROR* err){
	if (!graph){
		*err = BAD;
		return;
	}
	char* dot_file = "labyrinth.dot";
	char* png_file = "labyrinth.png";

	exportGraphToDot(graph, dot_file, err);
	if (*err != GOOD){
		printf("\nВизуализация нот гуд\n");
		return;
	}

	char command[256];
	snprintf(command, sizeof(command), "dot -Tpng %s -o %s", dot_file, png_file);

	if (system(command) == 0){
		printf("Визуализация успешна\n");
	} else{
		printf("Что-то не так\n");
		*err = BAD;
	}
}
