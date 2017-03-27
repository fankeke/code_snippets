

typedef struct node_s node;
struct node_s{
    int val;
    node*next;
};

//1,insert sort method
node*sortlist(node*head)
{
    node heada,headb,*a,*b;
    a=&heada;a->next=head;
    b=&headb;b->next=NULL;
    node*p,*q,*r;

    for(p=a->next;p;p=r){ //every time we insert node p into headb list
	r=p->next;
	for(q=b;q->next;q=q->next){
	    if(p->val<q->next->val) break;

	}
	p->next=q->next; //insert !!
	q->next=p;
    }
    return headb.next; //finished,headb now is sorted
}



//2,merge sort method


