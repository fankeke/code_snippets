typedef struct node_s node;
 struct node_s{
     int val;
     node*next;
 };
node* addTwoNumbers(node* l1, node* l2) {
    if (l1==NULL||l2==NULL) {
        return l1==NULL?l2:l1;
    }
    node dummy,*last,*p,*q,*cur;
    last=&dummy;last->next=NULL;
    p=l1;q=l2;
    int carry=0,dig=0;
    while(p||q){
        dig=(!p?0:p->val)+(!q?0:q->val)+carry;
        cur=malloc(sizeof(node));
        carry=dig/10;dig=dig%10;
        cur->val=dig;cur->next=NULL;
        last->next=cur;last=cur;
        p=p?p->next:NULL;
        q=q?q->next:NULL;
        printf("%d\n",cur->val);
    }
    if(carry){
        cur=malloc(sizeof(node));
        cur->val=carry;cur->next=NULL;
        last->next=cur;
    }
    return dummy.next;
}
