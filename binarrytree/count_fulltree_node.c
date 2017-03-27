https://leetcode.com/problems/count-complete-tree-nodes/




//best  code: 

typedef struct node_s node;
 struct node_s {
     int val;
     node*left,*right;
 };
 int depthleft(node*r)
 {
     int depth=0;
     while(r){
         depth++;
         r=r->left;
     }
     return depth;
 }
 int depthright(node*r)
 {
     int depth=0;
     while(r){
         depth++;
         r=r->right;
     }
     return depth;
 }
 
 //总结：还是根据拆分，把左右两只分开来看
int countNodes(node* root) {
    if(!root) return 0;
    int leftdepth=depthleft(root->left);
    int rightdepth=depthright(root->right);
    if(leftdepth==rightdepth){
        return (1<<((leftdepth+1)))-1;
    }else{
        return 1+countNodes(root->left)+countNodes(root->right);
    }
}
