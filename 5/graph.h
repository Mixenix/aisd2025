#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define INITIAL_CAPACITY 10
#define MAX_COORD_STR_LEN 10000 // чтоб считывать координаты как строчку
#define BAD_ALLOC_MESSAGE "\nBad alloc, exiting"
#define WRONG_INP "\nНеверный ввод, попробуйте ещё раз: \n"

#define MENU "\n|||MENU|||\n0. Добавить вершину\t1. Добавить ребро\n2. Удалить вершину\t3. Удалить ребро\n4. Изменить данные вершины\t5. Вывод графа\t6. Визуализация графа\n7. Поиск выхода\t8. Кратчайший путь\t9.Построить мин остовное дерево\n"

typedef enum ERROR {
    GOOD = 0,
    BAD = -1,
    BAD_ALLOC = -2,
    EXIT = -10,
    NOT_FOUND = -3,
    ALREADY_EXISTS = -4,
    INVALID_INPUT = -5,
    INVALID_OPERATION = -6
} ERROR;

typedef enum CellType {
    NORMAL,
    ENTRANCE,
    EXIT_CELL
} CellType;

struct Graph;

typedef struct AdjListNode {
    int dest_vertex_index;
    struct AdjListNode* next;
} AdjListNode;

typedef struct Vertex {
    unsigned int x;
    unsigned int y;
    CellType type;
    AdjListNode* head;
    bool is_active;
} Vertex;

typedef struct Graph {
    Vertex* vertices;
    int num_vertices;
    int capacity;
} Graph;


Graph* createGraph(int initial_capacity, ERROR* err);
void freeGraph(Graph* graph);
void freeAdjList(AdjListNode** head_ref);

int findVertexIndex(Graph* graph, unsigned int x, unsigned int y, ERROR* err);


ERROR addVertex(Graph* graph, unsigned int x, unsigned int y, CellType type);


ERROR addEdge(Graph* graph, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

ERROR removeVertex(Graph* graph, unsigned int x, unsigned int y);

ERROR removeEdge(Graph* graph, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

ERROR changeVertexData(Graph* graph, unsigned int x, unsigned int y, CellType new_type);

void printGraphAdjList(Graph* graph, ERROR* err);
void exportGraphToDot(Graph* graph, char* filename, ERROR* err);
void visualizeGraph(Graph* graph, ERROR* err);

char* cellTypeToString(CellType type);

char* readLineSimple(char* prompt);
ERROR getUnsignedInt(unsigned int* value, char* msg);
ERROR getCoordinates(unsigned int* x, unsigned int* y, char* msg_x, char* msg_y);
ERROR getCellType(CellType* type, char* msg);


typedef struct QueueNode {
    int vertex_index;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode *front, *rear;
    int count;
} Queue;

ERROR isExitReachableBFS(Graph* graph, unsigned int start_x, unsigned int start_y, bool* reachable);
ERROR shortestPathBellmanFord(Graph* graph,
                              unsigned int entrance_x, unsigned int entrance_y,
                              unsigned int exit_x, unsigned int exit_y,
                              int** path_array, int* path_len);
ERROR convertToSinglePath(Graph* graph);

#endif
