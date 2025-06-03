#include "bin.h"

#define MAX_STACK_SIZE 100000

Node *createNode(char* key, char* info, Node* parent, ERROR *err){
	Node* node = (Node*)malloc(sizeof(Node));
	if (node == NULL){
		*err = BAD_ALLOC;
		return NULL;
	}
	
	node->key = strdup(key);
	
	if (node->key == NULL){
		*err = BAD_ALLOC;
		free(node);
		return NULL;
	}
	node->info = strdup(info);
	if (node->info == NULL){
		*err = BAD_ALLOC;
		free(node->key);
		free(node);
		return NULL;
	}
	node->left = NULL;
	node->right = NULL;
	node->parent = parent;
	
	return node;
}

void insertElem(Node **root, char *key, char *info, ERROR *err){
	Node *parent = NULL;
	Node *cur = *root;

	while (cur != NULL){
		parent = cur;
		if (strcmp(key, cur->key) >= 0){
			cur = cur->right;
		}
		else{
			cur = cur->left;
		}
	}

	ERROR err2 = GOOD;
	Node *newNode = createNode(key, info, parent, &err2);
	if (newNode == NULL){
		*err = BAD_ALLOC;
		return;
	}
	if (parent == NULL){
		*root = newNode;
	}
	else if (strcmp(key, parent->key) < 0){
		parent->left = newNode;
	}
	else{
		parent->right = newNode;
	}
}


void removeElem(Node **root, char *key, ERROR *err){
	Node *parent = NULL;
	Node *cur = *root;

	while (cur != NULL && strcmp(cur->key, key) != 0){
		parent = cur;
		if (strcmp(key, cur->key) < 0){
			cur = cur->left;
		}
		else{
			cur = cur->right;
		}
	}
	if (cur == NULL){
		*err = NOT_FOUND;
		return;
	}
	char *prev_info = strdup(cur->info);
	if (prev_info == NULL){
		*err = BAD_ALLOC;
		return;
	}
	if (cur->left != NULL && cur->right != NULL){
		Node *good = cur->right;
		while (good->left != NULL){
			good = good->left;
		}
		free(cur->key);
		free(cur->info);
		cur->key = strdup(good->key);
		if (cur->key == NULL){
			*err = BAD_ALLOC;
			free(prev_info);
			return;
		}
		cur->info = strdup(good->info);
		if (cur->info == NULL){
			*err = BAD_ALLOC;
			free(prev_info);
			free(cur->key);
			return;
		}

		ERROR err2 = GOOD;
		removeElem(&cur->right, good->key, &err2);
		if (err2 == BAD_ALLOC){
			free(prev_info);
			free(cur->key);
			free(cur->info);
			return;
		}
	}
	else{
		Node *child;
		if (cur->left != NULL){
			child = cur->left;
		}
		else{
			child = cur->right;
		}

		if (parent == NULL){
			*root = child;
		}
		else if (parent->left == cur){
			parent->left = child;
		}
		else{
			parent->right = child;
		}
		
		if (child != NULL){
			child->parent = parent;
		}
		free(cur->key);
		free(cur->info);
		free(cur);
	}
	free(prev_info);
	return;
}


SearchResults search(Node *root, char *key, ERROR *err){
	SearchResults results = {NULL, 0};
	int capacity = 0;

	Node *stack[10000];
	int top = -1;
	Node *cur = root;

	while (cur != NULL || top >= 0){
		while (cur != NULL){
			stack[++top] = cur;
			cur = cur->left;
		}
		cur = stack[top--];

		if (strcmp(cur->key, key) == 0){
			if (results.count >= capacity){
				capacity = (capacity == 0) ? 4 : capacity * 2;
				results.nodes = realloc(results.nodes, capacity*(sizeof(Node *)));
				if (results.nodes == NULL){
					*err = BAD_ALLOC;
					results.nodes = NULL;
					results.count = 0;
					return results;
				}
			}

			results.nodes[results.count++] = cur;
		}
		else{
			break;
		}

		cur = cur->right;
	}

	return results;
}


Node *findMin(Node *root){
	if (root == NULL){return NULL;}

	Node *cur = root;
	while (cur->left != NULL){
		cur = cur->left;
	}
	return cur;
}

void printNode(Node *node){
	// if (node){printf("\n%s: %s\n", node->key, node->info);}
	return;
}


void printOut(Node *root, char *min_key){
	if (root == NULL){return;}
	printOut(root->right, min_key);

	if (min_key == NULL || strcmp(root->key, min_key) > 0){
		printNode(root);
		printOut(root->left, min_key);
	}
}

void printTree(Node *root, int level){
	if (root == NULL){return;}
	// printTree(root->right, level+1);
	printTree(root->left, level+1);

	for (int i=0; i<level; i++){
		printf("    ");
	}
	printf("%s\n", root->key);
	// printTree(root->left, level+1);
	printTree(root->right, level+1);
}

void freedom(Node *root){
	if (root == NULL){return;}
	freedom(root->left);
	freedom(root->right);

	free(root->key);
	free(root->info);
	free(root);
}


void importFromTxt(Node **root, char* filename, ERROR *err) {
	FILE* file = fopen(filename, "r");
	if (!file){
		*err = BAD;
		printf("Не удалось открыть файл %s, попробуйте ещё раз\n", filename);
		return;
	}

	char key[MAX_INPUT_LENGTH];
	char info[MAX_INPUT_LENGTH];
	
	while (fgets(key, sizeof(key), file) && fgets(info, sizeof(info), file)) {
		key[strcspn(key, "\n")] = 0;
		info[strcspn(info, "\n")] = 0;

		ERROR err2 = GOOD;
		insertElem(root, key, info, &err2);
		if (err2 == BAD_ALLOC){
			*err = BAD_ALLOC;
			fclose(file);
			return;
		}
	}
	
	fclose(file);
	printf("Дерево загружено из файла %s\n", filename);
}


void exportToDot(Node *root, char *filename, ERROR *err){
	FILE *file = fopen(filename, "w");
	if (!file){
		*err = BAD;
		return;
	}
	fprintf(file, "digraph BST {\n");
	fprintf(file, "\tnode [fontname=\"Arial\", shape=circle, ordering=\"out\"];\n");
	
	Node* stack[MAX_STACK_SIZE];
	int top = -1;
	Node* cur = root;

	while (cur != NULL || top >= 0) {
		while (cur != NULL) {
			stack[++top] = cur;
			fprintf(file, "\t\"%p\" [label=\"%s\\n%s\"];\n", 
				   (void*)cur, cur->key, cur->info);
			cur = cur->left;
		}

		cur = stack[top--];
		
		if (cur->left) {
			fprintf(file, "\t\"%p\" -> \"%p\" [label=\"L\"];\n", 
				   (void*)cur, (void*)cur->left);
		}
		if (cur->right) {
			fprintf(file, "\t\"%p\" -> \"%p\" [label=\"R\"];\n", 
				   (void*)cur, (void*)cur->right);
		}

		cur = cur->right;
	}

	fprintf(file, "}\n");
	fclose(file);
}


void visualizeTree(Node* root, ERROR *err) {
	char* dot_file = "tree.dot";
	char* png_file = "tree.png";
	
	exportToDot(root, dot_file, err);
	if (*err != GOOD){
		printf("\nНе удалось создать DOT-файл.");
		return;
	}
	
	char command[256];
	snprintf(command, sizeof(command), "dot -Tpng %s -o %s", dot_file, png_file);
	
	if (system(command) == 0) {
		printf("\nВизуализация успешна");
	} else {
		printf("\nОшибка, нет graphviz`a");
	}
}
