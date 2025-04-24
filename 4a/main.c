#include "bin.h"


void pre_order_traversal(Node* root) {
    if (root == NULL) return;
    
    printf("%s ", root->info);
    pre_order_traversal(root->left);
    pre_order_traversal(root->right);
}
