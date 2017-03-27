Swap Nodes in Pairs

Given a linked list, swap every two adjacent nodes and return its head.

For example,
Given 1->2->3->4, you should return the list as 2->1->4->3.

Your algorithm should use only constant space. You may not modify the values in the list, only nodes itself can be changed.


https://leetcode.com/problems/swap-nodes-in-pairs/


//my code:

typedef struct node_s node;
 struct node_s {
     int val;
     node*next;
 };
node* swapPairs(node* head) {
   if(!head||!head->next) return head;
   node*h,*q,*r,*prev=NULL,*p=head;
   int flag=0;
   while(p&&p->next){                                       //const space and O(n). but hard to understand .....
       q=p->next;r=q->next;
       p->next=r;q->next=p;
       if(prev)prev->next=q;
       prev=p;p=r;
       if(flag==0) h=q;
       flag=1;
   }
   return h;
}

//best code
node*swapnode(node*a,node*b)
{
  a->next=b->next;
  b->next=a;
  return b;
}
node* swapPairs(node*head){
  if(!head||!head->next) return head;
  node dummy,*p;
  dummy.next=head;
  p=&dummy;
  while(p->next&&p->next->next){
    p->next=swapnode(p->next,p->next->next);
    p=p->next->next;
  }
  return dummy.next;
}



