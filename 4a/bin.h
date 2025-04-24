#ifndef BIN_H
#define BIN_H




#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Node{
	char *key;
	char *info;
	struct Node *left;
	struct Node *right;
	struct Node *parent;
} Node;

typedef struct {
	Node **res;
	int count;
} SearchResults;


