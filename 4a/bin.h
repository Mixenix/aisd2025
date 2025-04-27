#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <string.h>

#define MAX_INPUT_LENGTH 4000
#define EPSILON 0.000000000001
#define BAD_ALLOC_MESSAGE "\nBad alloc, exiting..."
#define MENU "\n|||МЕНЮ\n0 - Добавить эл-т\t1 - удалить эл-т\n2 - обход дерева\t3 - поиск\n4 - поиск минимального(спец)\n5 - импорт из txt\n\nВыберите действие: "

typedef enum ERROR{
	GOOD = 0,
	BAD = -1,
	BAD_ALLOC = -2,
	EXIT = -10,
	NOT_FOUND = -3,
} ERROR;

typedef struct Node {
	char* key;
	char* info;
	struct Node* left;
	struct Node* right;
	struct Node* parent;
} Node;

typedef struct {
	Node** nodes;
	size_t count;
} SearchResults;

Node *createNode(char* key, char* info, Node* parent, ERROR *err);
void insertElem(Node **root, char *key, char *info, ERROR *err);
void removeElem(Node** root, char *key, ERROR *err);
SearchResults search(Node* root, char* key, ERROR *err);
Node *findMin(Node* root);
void printOut(Node* root, char* min_key);
void printTree(Node* root, int level);
void freedom(Node* root);
void printNode(Node* node);
void importFromTxt(Node **root, char* filename, ERROR *err);

int input(int *action, char **redString, char *INV);
bool isInteger(double N);
bool check_Number(char *text);

#endif
