#include "tree.h"

double SCAPEGOAT_ALPHA = DEFAULT_ALPHA;
static int max_nodes_after_rebuild = 0;

void update_size(Node* node) {
    if (!node) return;
    node->size = 1 + (node->left ? node->left->size : 0) + (node->right ? node->right->size : 0);
}

int get_size(Node* node) {
    return node ? node->size : 0;
}

Node* create_node(char* key, unsigned int value, Node* parent, ERROR* err) {
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
    new_node->size = 1;
    return new_node;
}

void free_node(Node* node) {
    if (!node) return;
    free(node->key);
    free(node);
}

void free_tree(Node* root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free_node(root);
}

void flatten_subtree_to_array(Node* node, Node** arr, int* index) {
    if (!node) return;
    flatten_subtree_to_array(node->left, arr, index);
    arr[(*index)++] = node;
    flatten_subtree_to_array(node->right, arr, index);
}

Node* build_balanced_from_array(Node** arr, int start, int end, Node* parent) {
    if (start > end) return NULL;
    int mid = start + (end - start) / 2;
    Node* node = arr[mid];
    node->parent = parent;
    node->left = build_balanced_from_array(arr, start, mid - 1, node);
    node->right = build_balanced_from_array(arr, mid + 1, end, node);
    update_size(node);
    return node;
}

Node* actual_rebuild_logic(Node* scapegoat_node, Node* parent_of_scapegoat) {
    if (!scapegoat_node) return NULL;

    int subtree_size = get_size(scapegoat_node);
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

ERROR insert_elem(Node** root_ptr, char* key, unsigned int value) {
    if (!root_ptr) return BAD;

    Node* current = *root_ptr;
    Node* parent = NULL;
    int depth = 0;

    while (current != NULL) {
        parent = current;
        current->size++;
        if (strcmp(key, current->key) < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
        depth++;
    }

    ERROR err_node = GOOD;
    Node* new_node = create_node(key, value, parent, &err_node);
    if (err_node == BAD_ALLOC) return BAD_ALLOC;

    if (!parent) {
        *root_ptr = new_node;
    } else if (strcmp(key, parent->key) < 0) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }

    int total_nodes = (*root_ptr) ? (*root_ptr)->size : 0;
    max_nodes_after_rebuild = (total_nodes > max_nodes_after_rebuild) ? total_nodes : max_nodes_after_rebuild;

    if (depth > (log(total_nodes) / log(1.0 / SCAPEGOAT_ALPHA)) + 1) {
        Node* scapegoat_candidate = new_node->parent;
        Node* child_on_path = new_node;
        while (scapegoat_candidate != NULL) {
            int size_scapegoat_candidate = scapegoat_candidate->size;
            int size_child_on_path = child_on_path->size;
            if (size_child_on_path > SCAPEGOAT_ALPHA * size_scapegoat_candidate) {
                Node* parent_of_scapegoat = scapegoat_candidate->parent;
                Node* rebuilt_subtree_root = actual_rebuild_logic(scapegoat_candidate, parent_of_scapegoat);
                if (!rebuilt_subtree_root && size_scapegoat_candidate > 0) {
                    return BAD_ALLOC;
                }
                if (!parent_of_scapegoat) {
                    *root_ptr = rebuilt_subtree_root;
                } else if (parent_of_scapegoat->left == scapegoat_candidate) {
                    parent_of_scapegoat->left = rebuilt_subtree_root;
                } else {
                    parent_of_scapegoat->right = rebuilt_subtree_root;
                }
                if (rebuilt_subtree_root) update_size(rebuilt_subtree_root);
                max_nodes_after_rebuild = (*root_ptr) ? (*root_ptr)->size : 0;
                break;
            }
            child_on_path = scapegoat_candidate;
            scapegoat_candidate = scapegoat_candidate->parent;
        }
    }
    return GOOD;
}

ERROR remove_elem(Node** root_ptr, char* key) {
    if (!root_ptr || !*root_ptr) return EMPTY_TREE;
    
    Node* current = *root_ptr;
    Node* parent = NULL;
    Node* node_to_delete = NULL;

    while (current != NULL) {
        int cmp = strcmp(key, current->key);
        if (cmp == 0) {
            node_to_delete = current;
            break;
        }
        current->size--;
        parent = current;
        current = (cmp < 0) ? current->left : current->right;
    }

    if (!node_to_delete) return NOT_FOUND;

    if (!node_to_delete->left || !node_to_delete->right) {
        Node* child = node_to_delete->left ? node_to_delete->left : node_to_delete->right;
        
        if (!parent) {
            *root_ptr = child;
        } else if (parent->left == node_to_delete) {
            parent->left = child;
        } else {
            parent->right = child;
        }
        
        if (child) {
            child->parent = parent;
            Node* update_node = parent;
            while (update_node) {
                update_size(update_node);
                update_node = update_node->parent;
            }
        }
        
        free_node(node_to_delete);
    } else {
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
        } else {
            successor_parent->right = successor->right;
        }
        
        if (successor->right) {
            successor->right->parent = successor_parent;
        }
        
        Node* update_node = successor_parent;
        while (update_node) {
            update_size(update_node);
            update_node = update_node->parent;
        }
        
        free_node(successor);
    }

    int current_size = (*root_ptr) ? (*root_ptr)->size : 0;
    if (current_size > 0 && max_nodes_after_rebuild > 0 && 
        current_size < SCAPEGOAT_ALPHA * max_nodes_after_rebuild) {
        Node* new_root = actual_rebuild_logic(*root_ptr, NULL);
        if (new_root || current_size == 0) {
            *root_ptr = new_root;
            if (new_root) update_size(new_root);
            max_nodes_after_rebuild = current_size;
        } else {
            return BAD_ALLOC;
        }
    }

    return GOOD;
}

SearchResults search_by_key(Node* root, char* key, ERROR* err) {
    SearchResults results = {NULL, 0, 0};
    *err = GOOD;
    if (!root) return results;
    
    search_recursive_helper(root, key, &results, err);
    if (*err == BAD_ALLOC) {
        free_search_results(&results);
    }
    return results;
}

void search_recursive_helper(Node* node, char* key, SearchResults* results, ERROR* err) {
    if (!node || *err == BAD_ALLOC) return;
    
    if (strcmp(key, node->key) <= 0) {
        search_recursive_helper(node->left, key, results, err);
        if (*err == BAD_ALLOC) return;
    }
    
    if (strcmp(key, node->key) == 0) {
        if (results->count >= results->capacity) {
            size_t new_capacity = (results->capacity == 0) ? 8 : results->capacity * 2;
            Node** temp = (Node**)realloc(results->nodes, new_capacity * sizeof(Node*));
            if (!temp) {
                *err = BAD_ALLOC;
                return;
            }
            results->nodes = temp;
            results->capacity = new_capacity;
        }
        results->nodes[results->count++] = node;
    }
    
    if (strcmp(key, node->key) >= 0) {
        search_recursive_helper(node->right, key, results, err);
    }
}

SearchResults find_max_keys(Node* root, ERROR* err) {
    SearchResults results = {NULL, 0, 0};
    *err = GOOD;
    if (!root) return results;

    Node* current = root;
    while (current->right != NULL) {
        current = current->right;
    }

    if (current) {
        return search_by_key(root, current->key, err);
    }
    return results;
}

void free_search_results(SearchResults* results) {
    if (results) {
        free(results->nodes);
        results->nodes = NULL;
        results->count = 0;
        results->capacity = 0;
    }
}

void print_in_order(Node* root) {
    if (!root) return;
    print_in_order(root->left);
    printf("  Ключ: %-20s | Значение: %u | Размер поддерева: %d\n", root->key, root->value, root->size);
    print_in_order(root->right);
}

void print_tree_formatted(Node* root, int level, int dir) {
    if (!root) return;

    for (int i = 0; i < level -1; i++) {
        printf("    ");
    }
    if (level > 0) {
         printf(dir <= 0 ? " L--":" R--");
    }
    printf("[%s: %u (%d)]\n", root->key, root->value, root->size);

    print_tree_formatted(root->left, level + 1, 0);
    print_tree_formatted(root->right, level + 1, 1);
}

ERROR import_from_txt(Node** root_ptr, char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return FILE_ERROR;

    char key_buf[256];
    char val_buf[128];
    unsigned int value;
    ERROR err = GOOD;

    while (fgets(key_buf, sizeof(key_buf), file) && fgets(val_buf, sizeof(val_buf), file)) {
        key_buf[strcspn(key_buf, "\r\n")] = 0;
        val_buf[strcspn(val_buf, "\r\n")] = 0;

        if (strlen(key_buf) == 0) continue;

        char* endptr;
        errno = 0;
        unsigned long temp_ul = strtoul(val_buf, &endptr, 10);

        if (val_buf == endptr || *endptr != '\0' || errno == ERANGE || temp_ul > UINT_MAX) {
            printf("Нарушение формата, пропускаем строку...\n");
            continue;
        }
        value = (unsigned int)temp_ul;

        err = insert_elem(root_ptr, key_buf, value);
        if (err == BAD_ALLOC) {
            fclose(file);
            return BAD_ALLOC;
        }
    }
    fclose(file);
    return GOOD;
}

void export_dot_nodes_recursive(Node* node, FILE* fp) {
    if (!node) return;
    fprintf(fp, "  \"%p\" [label=\"%s | %u | %d\"];\n", (void*)node, node->key, node->value, node->size);
    export_dot_nodes_recursive(node->left, fp);
    export_dot_nodes_recursive(node->right, fp);
}

void export_dot_edges_recursive(Node* node, FILE* fp) {
    if (!node) return;
    if (node->left) {
        fprintf(fp, "  \"%p\" -> \"%p\" [label=\"L\"];\n", (void*)node, (void*)node->left);
        export_dot_edges_recursive(node->left, fp);
    }
    if (node->right) {
        fprintf(fp, "  \"%p\" -> \"%p\" [label=\"R\"];\n", (void*)node, (void*)node->right);
        export_dot_edges_recursive(node->right, fp);
    }
}

ERROR export_to_dot(Node* root, char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) return FILE_ERROR;

    fprintf(fp, "digraph ScapegoatTree {\n");
    fprintf(fp, "  node [shape=record, fontname=\"Arial\"];\n");

    if (root) {
        export_dot_nodes_recursive(root, fp);
        export_dot_edges_recursive(root, fp);
    } else {
        fprintf(fp, "  empty [label=\"Tree is Empty\"];\n");
    }

    fprintf(fp, "}\n");
    fclose(fp);
    return GOOD;
}

ERROR visualize_tree_graphviz(Node* root, char* dot_filename, char* output_img_filename) {
    ERROR err = export_to_dot(root, dot_filename);
    if (err != GOOD) return err;

    char command[512];
    snprintf(command, sizeof(command), "dot -Tpng %s -o %s", dot_filename, output_img_filename);

    int result = system(command);
    if (result == 0) {
        printf("Визуализация успешна");
        return GOOD;
    } else {
        return BAD;
    }
}
