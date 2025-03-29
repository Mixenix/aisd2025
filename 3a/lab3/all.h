#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <string.h>

#define MENU "\n0. Инициализация новой таблицы   1. Вставка нового эл-та\n2. Удаление элемента определённой версии   3. Удаление всех элементов с определённым ключом\n4. Поиск по ключу (все)   5. Поиск по ключу и версии\n6. Импорт таблицы из текстового файла   7. Вывод таблицы\nВведите желаемое действие: "

#define EXIT -10
#define BAD -2
#define GOOD 0

#define NOTUNIQUE -3
#define SIZEEXCEEDED -4
#define NOTDEFINEDYET -1
#define EPSILON 0.000000000001

typedef struct Node Node;
typedef struct KeySpace KeySpace;
typedef struct Table Table;

struct Node {
	int release;
	char *info;
	Node *next;
};

struct KeySpace {
	char *key;
	Node *node;
};

struct Table {
	KeySpace *ks;
	int msize;
	int csize;
};


#ifndef ALL_H
#define ALL_H
bool check_Number(char *text);
bool isInteger(double N);
int input(int *action, char *INV);
char *readtxtline(FILE *f);
Table *readTableFromFile(char *filename);
Table* searchAllByKey(Table* tablePtr, char* keyToFind);
char *searchByKeyAndRelease(Table *tbl, char *keyToFind, int relToFind);
int removeByKey(Table *tbl, char *keyToDel);
int removeByKeyAndRelease(Table *tbl, char *keyToDel, int RelToDel);
#endif



