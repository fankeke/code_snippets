

https://leetcode.com/problems/merge-k-sorted-lists/


typedef struct node_s node;
 struct node_s {
     int val;
     node*next;
 };
 node*mergelist(node*a,node*b)
 {
     node dummy,*r;
     dummy.next=NULL;
     r=&dummy;
     while(a&&b){
         if(a->val<b->val){
             r->next=a;a=a->next;
         }else{
             r->next=b;b=b->next;
         }
         r=r->next;r->next=NULL;
     }
     if(a||b) r->next=a?a:b;
     return dummy.next;
 }
node* mergeKLists(node** lists, int size) {
    if(size<1) return NULL;
    if(size==1) return lists[0];
    node*left,*right;
    int len;
    left=mergeKLists(lists,size/2);
    right=mergeKLists(lists+size/2,size-size/2);
    return mergelist(left,right);
    
}


