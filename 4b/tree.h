#ifndef TREE_H
#define TREE_H



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>
#include <readline/readline.h>

struct Node_t;

typedef enum ERROR {
	GOOD = 0,
	BAD = -1,
	BAD_ALLOC = -2,
	EXIT = -10,
	NOT_FOUND = -3,
	EMPTY_TREE = -4,
	FILE_ERROR = -5,
	INVALID_INPUT = -6
} ERROR;

#define BAD_ALLOC_MESSAGE "\nОшибка выделения памяти, выход...\n"
#define DEFAULT_ALPHA 0.6666666666666666

typedef struct Node_t {
	char* key;
	unsigned int value;
	struct Node_t* left;
	struct Node_t* right;
	struct Node_t* parent;
} Node;

typedef struct {
	Node** nodes;
	size_t count;
	size_t capacity;
} SearchResults;

extern double SCAPEGOAT_ALPHA;


Node* sg_create_node(char* key, unsigned int value, Node* parent, ERROR* err);
void sg_free_node(Node* node);
void sg_free_tree(Node* root);
Node* actual_rebuild_logic(Node* scapegoat_node, Node* parent_of_scapegoat);
Node* build_balanced_from_array(Node** arr, int start, int end, Node* parent);
void flatten_subtree_to_array(Node* node, Node** arr, int* index);

ERROR sg_insert_elem(Node** root_ptr, char* key, unsigned int value);
ERROR sg_remove_elem(Node** root_ptr, char* key);
void export_dot_nodes_recursive(Node* node, FILE* fp);
void export_dot_edges_recursive(Node* node, FILE* fp);

SearchResults sg_search_by_key(Node* root, char* key, ERROR* err);
SearchResults sg_find_max_keys(Node* root, ERROR* err);
void sg_free_search_results(SearchResults* results);
void search_recursive_helper(Node* node, char* key, SearchResults* results, ERROR* err);

void sg_print_in_order(Node* root);
void sg_print_tree_formatted(Node* root, int level, int dir);


ERROR sg_import_from_txt(Node** root_ptr, char* filename);
ERROR sg_export_to_dot(Node* root, char* filename);
ERROR sg_visualize_tree_graphviz(Node* root, char* dot_filename, char* output_img_filename);

int sg_get_size(Node* node);
Node* sg_rebuild_subtree(Node* scapegoat_node_child_on_path, Node* parent_of_scapegoat, double alpha);




ERROR input_int_action(int* action, char* prompt);
ERROR input_string(char** str_val, char* prompt);
ERROR input_unsigned_int(unsigned int* value, char* prompt);
#endif

