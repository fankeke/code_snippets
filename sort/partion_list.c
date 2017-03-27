https://leetcode.com/problems/partition-list/


Given a linked list and a value x, partition it such that all nodes less than x come before nodes greater than or equal to x.

You should preserve the original relative order of the nodes in each of the two partitions.

For example,
Given 1->4->3->2->5->2 and x = 3,
return 1->2->2->4->3->5.

 typedef struct node_s node;
 struct node_s{
     int val;
     node*next;
 };
 
 //my code :
node* partition(node* head, int x) {
    if(!head||!head->next) return head;
    node dummy,*prep,*preq,*p,*q;
    dummy.next=head;
    prep=&dummy;
    while(prep->next){
        p=prep->next;
        while(p&&p->val<x){prep=p; p=p->next;}      // find the last node p ,the value less than x;
        q=p;
        while(q&&q->val>=x){preq=q;q=q->next;}      //find the first node q,the value less than x; 
        if(!q) break;         //game over ...
        
        preq->next=q->next;  //delete q and insert infront of p;
        q->next=prep->next;
        prep->next=q;
        
        prep=prep->next;    //go on ...
        
        
    }
    return dummy.next;
}


//best code:

node* partition(node* head, int x) {
    if(!head||!head->next) return head;
    node dummy1,dummy2,*p1,*p2;
    p1=&dummy1;p2=&dummy2;
    while(head){
        if(head->val<x) p1=p1->next=head;
        else            p2=p2->next=head;
        
        head=head->next;
    }
    p2->next=NULL;
    p1->next=dummy2.next;
    return dummy1.next;
  
}
