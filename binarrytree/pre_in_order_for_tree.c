#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct node_s node;
struct node_s{
    int 	val;
    node 	*left,*right;
};

node*create_tree(int*pre,int*in,int len)
{
    if(len<=0) return NULL;
    node*root;
    int len1,len2,*p;
    root=(node*)malloc(sizeof(node));
    root->val=pre[0];
    for(p=in;*p!=root->val;p++);
    len1=in-p;
    len2=len-len1-1;
    root->left=create_tree(pre+1,in,len1);
    root->right=create_tree(pre+len1+1,p+1,len2);
    return root;
}


    
//调用的时候post指向后续的最后一个位置
node*create_tree(int*in,int*post,int len)
{
    if(len<=0)return NULL;
    node*root;
    int len1,len2,*p;
    root=(node*)malloc(sizeof(node));
    root->val=post[0];
    for(p=in;*p!=root->val;p++);
    len1=p-in;
    len2=len-len1-1;
    root->right=create_tree(p+1,post-1,len2);//现求出的是右边
    root->left=create_tree(in,post-len2-1,len1);
    return root;
}














