typedef struct node_s node;
 struct node_s{
     int val;
     node*next;
 };
node* reverseList(node* head) {
    if(!head) return NULL;
    node dummy,*r,*p;
    p=head;dummy.next=NULL;
    while(p){
        r=p;p=p->next;
        r->next=dummy.next;dummy.next=r;
    }
    return dummy.next;
}
