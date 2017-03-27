

int maxDepth(tree*root)
{
    if(!node) return 0;
    int leftdepth=maxDepth(root->left);
    int rightdepth=maxDepth(root->right);
    return leftdept>rightdepth?leftdept+1:rightdept+1;
}



int minDepth(tree*root)
{
    if(!node) return 0;
    int leftdepth=maxDepth(root->left)+1;
    int rightdepth=maxDepth(root->right)+1;
    if(leftdept==1&&rightdepth!=1) return rightdepth;
    if(leftdept!=1&&rightdepth==1) return leftdepth;
    return leftdept>rightdepth?leftdept:rightdept;
}
