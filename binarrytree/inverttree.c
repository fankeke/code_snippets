 struct TreeNode* invertTree(struct TreeNode* root) {
    if(!root) return NULL;
    
    struct TreeNode*temp=root->left;
    root->left=root->right;
    root->right=temp;
    
    invertTree(root->left);
    invertTree(root->right);
    
    return root;
}
