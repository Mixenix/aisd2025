#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <readline/readline.h>

typedef int ERROR;

#define KEY 101
#define INFO 102
#define REL 103
#define CHOICE 100


#define BAD_ALLOC -4
#define GOOD 0
#define BAD -2
#define NOT_FOUND -5
#define NOT_FOUND_FILE -6
#define FAILED_TO_CREATE_FILE -7
#define FAILED_TO_WRITE -9
#define BAD_FORMAT -8
#define EXIT -10

#define EPSILON 0.000000000001
#define SIZE_PERCENTAGE 0.7
#define INIT_SIZE 17

#define BAD_ALLOC_MESSAGE "\nBad alloc, exiting..."
#define MENU "\nМеню\n0. Вставка элемента   1. Удалить элементы\n2. Удалить элемент по версии   3. Поиск по ключу   4. Поиск по ключу и версии\n5. Импорт из файла   6. Экспорт в файл\n7. Выход\nВыберите действие: "


typedef enum {
    FREE = 0,
    USED = 1,
    DELETED = 2
} BusyType;

typedef struct {
    BusyType busy;
    unsigned int key;
    int release;
    unsigned int *info;
} KeySpace;

typedef struct {
    KeySpace *ks;
    int msize;
    int csize;
} Table;


void export(Table *tbl, char *filename, ERROR *err); // done
void import(Table *tbl, char *filename, ERROR *err);
Table *init(int msize, ERROR *err);
void tbl_free(Table *tbl);
void insert(Table *tbl, unsigned int key, unsigned int info, ERROR *err); //done

int insertion(Table *tbl, unsigned int *tmp, unsigned int *tmp2);
int removal(Table *tbl, unsigned int *tmp, unsigned int *tmp2);
int srch(Table *tbl, unsigned int *tmp);
int removalByVersion(Table *tbl, unsigned int *tmp, unsigned int *tmp2);
int srchByVersion(Table *tbl, unsigned int *tmp, unsigned int *tmp2);
int flImport(Table *tbl);
int flExport(Table *tbl);


void elem_remove(Table *tbl, unsigned int key, int release, ERROR *err); //done
KeySpace *search(Table *tbl, unsigned int key, int release, int *found_count, ERROR *err);
void printTable(Table *tbl);
KeySpace *search_all_versions(Table *tbl, unsigned int key, int *found_count, ERROR *err);
int is_prime(int n);
int next_prime_size(int current_size);
void resize(Table *tbl, ERROR *err);
bool isInteger(double N);
bool check_Number(char *text);
int input(unsigned int *action, char *INV, int kindOf);

#endif
