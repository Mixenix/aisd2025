typedef struct Node{
	char *key;
	char *info;
	struct Node *left;
	struct Node *right;
	struct Node *parent;
} Node;


void pre_order_traversal(TreeNode* root) {
    if (root == NULL) return;
    
    printf("%d ", root->data);
    pre_order_traversal(root->left);
    pre_order_traversal(root->right);
}
