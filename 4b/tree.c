#include "tree.h"


double SCAPEGOAT_ALPHA = DEFAULT_ALPHA;
static int max_nodes_after_rebuild = 0;

int sg_get_size(Node* node) {
	if (!node) return 0;
	return 1 + sg_get_size(node->left) + sg_get_size(node->right);
}


Node* sg_create_node(char* key, unsigned int value, Node* parent, ERROR* err) {
	*err = GOOD;
	Node* new_node = (Node*)malloc(sizeof(Node));
	if (!new_node) {
		*err = BAD_ALLOC;
		return NULL;
	}
	new_node->key = strdup(key);
	if (!new_node->key) {
		free(new_node);
		*err = BAD_ALLOC;
		return NULL;
	}
	new_node->value = value;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->parent = parent;
	return new_node;
}


void sg_free_node(Node* node) {
	if (!node) return;
	free(node->key);
	free(node);
}

void sg_free_tree(Node* root) {
	if (!root) return;
	sg_free_tree(root->left);
	sg_free_tree(root->right);
	sg_free_node(root);
}

void flatten_subtree_to_array(Node* node, Node** arr, int* index){
	if (!node) return;
	flatten_subtree_to_array(node->left, arr, index);
	arr[(*index)++] = node;
	flatten_subtree_to_array(node->right, arr, index);
}

Node* build_balanced_from_array(Node** arr, int start, int end, Node* parent){
	if (start > end) return NULL;
	int mid = start + (end - start) / 2;
	Node* node = arr[mid];
	node->parent = parent;
	node->left = build_balanced_from_array(arr, start, mid - 1, node);
	node->right = build_balanced_from_array(arr, mid + 1, end, node);
	return node;
}

Node* actual_rebuild_logic(Node* scapegoat_node, Node* parent_of_scapegoat) {
	if (!scapegoat_node) return NULL;

	int subtree_size = sg_get_size(scapegoat_node);
	if (subtree_size == 0) return NULL;

	Node** arr = (Node**)malloc(subtree_size * sizeof(Node*));
	if (!arr) return NULL;

	int index = 0;
	flatten_subtree_to_array(scapegoat_node, arr, &index);

	for (int i = 0; i < subtree_size; ++i) {
		arr[i]->left = arr[i]->right = arr[i]->parent = NULL;
	}

	Node* new_subtree_root = build_balanced_from_array(arr, 0, subtree_size - 1, parent_of_scapegoat);
	free(arr);
	
	return new_subtree_root;
}


ERROR sg_insert_elem(Node** root_ptr, char* key, unsigned int value) {
	Node* current = *root_ptr;
	Node* parent = NULL;
	int depth = 0;

	while (current != NULL) {
		parent = current;
		if (strcmp(key, current->key) < 0) {
			current = current->left;
		} else{
			current = current->right;
		}
		depth++;
	}

	ERROR err_node = GOOD;
	Node* new_node = sg_create_node(key, value, parent, &err_node);
	if (err_node == BAD_ALLOC) return BAD_ALLOC;

	if (!parent) {
		*root_ptr = new_node;
	} else if (strcmp(key, parent->key) < 0) {
		parent->left = new_node;
	} else {
		parent->right = new_node;
	}

	int total_nodes = sg_get_size(*root_ptr);
	max_nodes_after_rebuild = (total_nodes > max_nodes_after_rebuild) ? total_nodes : max_nodes_after_rebuild;

	if (depth > (log(total_nodes) / log(1.0 / SCAPEGOAT_ALPHA)) + 1 && total_nodes > 1){ // + 1 чтоб наверняка || проверка на условие жертвы
		Node* scapegoat_candidate = new_node->parent;
		Node* child_on_path = new_node;

		while(scapegoat_candidate != NULL) {
			int size_scapegoat_candidate = sg_get_size(scapegoat_candidate);
			int size_child_on_path = sg_get_size(child_on_path);
			if (size_child_on_path > SCAPEGOAT_ALPHA * size_scapegoat_candidate){
				Node* parent_of_scapegoat = scapegoat_candidate->parent;
				Node* rebuilt_subtree_root = actual_rebuild_logic(scapegoat_candidate, parent_of_scapegoat);

				if (!rebuilt_subtree_root && size_scapegoat_candidate > 0){
					return BAD_ALLOC;
				}

				if (!parent_of_scapegoat) {
					*root_ptr = rebuilt_subtree_root;
				} else if (parent_of_scapegoat->left == scapegoat_candidate) {
					parent_of_scapegoat->left = rebuilt_subtree_root;
				} else {
					parent_of_scapegoat->right = rebuilt_subtree_root;
				}
				max_nodes_after_rebuild = sg_get_size(*root_ptr);
				break;
			}
			child_on_path = scapegoat_candidate;
			scapegoat_candidate = scapegoat_candidate->parent;
		}
	}
	return GOOD;
}


ERROR sg_remove_elem(Node** root_ptr, char* key) {
	if (!*root_ptr) return EMPTY_TREE;

	Node* current = *root_ptr;
	Node* parent = NULL;
	Node* node_to_delete = NULL;

	while (current != NULL) {
		if (strcmp(key, current->key) == 0) {
			node_to_delete = current;
			break;
		}
		parent = current;
		if (strcmp(key, current->key) < 0) {
			current = current->left;
		} else {
			current = current->right;
		}
	}

	if (!node_to_delete) return NOT_FOUND;
	if (!node_to_delete->left || !node_to_delete->right) {
		Node* child = node_to_delete->left ? node_to_delete->left : node_to_delete->right;
		if (!parent){
			*root_ptr = child;
		} else if (parent->left == node_to_delete) {
			parent->left = child;
		} else {
			parent->right = child;
		}
		if (child) child->parent = parent;
		sg_free_node(node_to_delete);
	}
	else {
		Node* successor_parent = node_to_delete;
		Node* successor = node_to_delete->right;
		while (successor->left != NULL) {
			successor_parent = successor;
			successor = successor->left;
		}

		free(node_to_delete->key);
		node_to_delete->key = strdup(successor->key);
		if (!node_to_delete->key) return BAD_ALLOC;
		node_to_delete->value = successor->value;

		if (successor_parent->left == successor) {
			successor_parent->left = successor->right;
		} else{
			successor_parent->right = successor->right;
		}
		if (successor->right) successor->right->parent = successor_parent;
		sg_free_node(successor);
	}
	
	int current_size = sg_get_size(*root_ptr);
	if (current_size > 0 && max_nodes_after_rebuild > 0 && current_size < SCAPEGOAT_ALPHA * max_nodes_after_rebuild) {
		Node* new_root = actual_rebuild_logic(*root_ptr, NULL);
		if (new_root || current_size == 0){
			 *root_ptr = new_root;
			 max_nodes_after_rebuild = current_size;
		} else {
			return BAD_ALLOC;
		}
	}

	return GOOD;
}


void search_recursive_helper(Node* node, char* key, SearchResults* results, ERROR* err) {
	if (!node || *err == BAD_ALLOC) return;

	if (strcmp(key, node->key) <= 0){
		search_recursive_helper(node->left, key, results, err);
		if (*err == BAD_ALLOC) return;
	}

	if (strcmp(key, node->key) == 0) {
		if (results->count >= results->capacity) {
			results->capacity = (results->capacity == 0) ? 8 : results->capacity * 2;
			Node** temp_nodes = (Node**)realloc(results->nodes, results->capacity * sizeof(Node*));
			if (!temp_nodes) {
				*err = BAD_ALLOC;
				sg_free_search_results(results);
				return;
			}
			results->nodes = temp_nodes;
		}
		results->nodes[results->count++] = node;
	}
	if (strcmp(key, node->key) >= 0) {
		 search_recursive_helper(node->right, key, results, err);
	}
}

SearchResults sg_search_by_key(Node* root, char* key, ERROR* err) {
	SearchResults results = {NULL, 0, 0};
	*err = GOOD;
	if (!root) return results;
	search_recursive_helper(root, key, &results, err);
	return results;
}


SearchResults sg_find_max_keys(Node* root, ERROR* err) {
	SearchResults results = {NULL, 0, 0};
	*err = GOOD;
	if (!root) return results;

	Node* current = root;
	while (current->right != NULL) {
		current = current->right;
	}

	if (current) {
		return sg_search_by_key(root, current->key, err);
	}
	return results;
}

void sg_free_search_results(SearchResults* results) {
	if (results) {
		free(results->nodes);
		results->nodes = NULL;
		results->count = 0;
		results->capacity = 0;
	}
}


void sg_print_in_order(Node* root) {
	if (!root) return;
	sg_print_in_order(root->left);
	printf("  Key: %-20s | Value: %u\n", root->key, root->value);
	sg_print_in_order(root->right);
}


void sg_print_tree_formatted(Node* root, int level, int dir) {
	if (!root) return;

	for (int i = 0; i < level -1; i++) {
		printf("    ");
	}
	if (level > 0) {
		 printf(dir <= 0 ? " L--":" R--");
	}
	printf("[%s: %u]\n", root->key, root->value);

	sg_print_tree_formatted(root->left, level + 1, 0);
	sg_print_tree_formatted(root->right, level + 1, 1);
}
