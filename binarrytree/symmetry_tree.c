Symmetric Tree My Submissions Question
Total Accepted: 84077 Total Submissions: 257912 Difficulty: Easy
Given a binary tree, check whether it is a mirror of itself (ie, symmetric around its center).

For example, this binary tree is symmetric:

    1
   / \
  2   2
 / \ / \
3  4 4  3
But the following is not:
    1
   / \
  2   2
   \   \
   3    3
https://leetcode.com/problems/symmetric-tree/


//对于二茶树的递归，容易忽略的情况就是，传递两个递归参数，left，right。
／／而一般的递归都只是传递了一个参数。

best code:

typedef struct node_s node;
struct node_s {
    int val;
    node*left,*right;
};
bool helper(node*left,node*right)
{
    if(!left&&!right) return true;
    if(!left||!right||left->val!=right->val) return false;
    return helper(left->left,right->right)&&helper(left->right,right->left);
}
bool isSymmetric(node* root) {
   if(!root) return true;
   return helper(root->left,root->right);            //传递两个参数的递归
}
