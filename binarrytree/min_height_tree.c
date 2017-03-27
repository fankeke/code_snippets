

和求二叉树的高度不同（最大高度），这里的最小高度需排除哪些单支树的0，0不算在内。

所以，需要进行一定的便利。


当然用 path的万能解法，可以把每天路径都存放在stack中，算出每条路径的长度，取最小值即可  （AC）



还有一种方法，用层次便利，如果某层的某个节点为叶子，那么即可到此结束，返回此level即为高度


mindepth(node*root)
{
  if(!root) return 0;
  int depth=1;
  queue_push(q,root);
  while(!queue_empty(q)){
    size=queue_size(q);
    for(i=0;i<size;i++){
        node*n=queue_pop(p);
        if(!p->left&&!p->right) return depth; 
        
        if(p->left) queue_push(q,n->left);
        if(p->right) queue_push(q,n->right);
    }
    
    depht++;
    
  }
  
  return depth;
}

