int depth(struct TreeNode*node)
 {
     if(!node) return 0;
     int l=depth(node->left)+1;
     int r=depth(node->right)+1;
     return l>r?l:r;
 }
 int myabs(int a,int b)
 {
     if(a>b) return a-b;
     return b-a;
 }
bool isBalanced(struct TreeNode* root) {
    if(!root) return true;
    int l=depth(root->left);
    int r=depth(root->right);
    if(myabs(l,r)>1) return false;
    return isBalanced(root->left)&&isBalanced(root->right);
}
