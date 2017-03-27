https://leetcode.com/problems/sort-list/

Sort a linked list in O(n log n) time using constant space complexity.




// my code 
//链表排序，妥妥的归并法

 typedef struct node_s node;
 struct node_s{
     int val;
     node *next;
 };
 node *mergeList(node*p,node*q)
 {
     node head;
     node *r,*u;
     r=&head;r->next=NULL;
     while(p&&q){
         if(p->val<=q->val){
             u=p;p=p->next;
         }else{
             u=q;q=q->next;
         }
         r->next=u;u->next=NULL;r=u;
     }
     r->next=p?p:q;
     return head.next;
 }
node* sortList(node* head) {
    if(head==NULL||head->next==NULL)
        return head;
    node    *fast,*slow;
    fast=slow=head;
    while(fast->next&&fast->next->next){
        fast=fast->next->next;
        slow=slow->next;
    }
    fast=slow->next;slow->next=NULL;
    
    slow=sortList(head);
    fast=sortList(fast);
    
    return mergeList(slow,fast);
    
}
