//all my code 

node* reverse(node*h)
{
	node dummy,*r;
	dummy.next=NULL;
	while(h){
		r=h->next;
		h->next=dummy.next;
		dummy.next=h;
		h=r;
	}
	return dummy.next;
}

//意思是把链表从第m节点到第n节点的顺序反转
node*reverseBetween(node*head,int m,int n){
	if(m==n) return head;
	if(!head||!head->next) return head;
	
	node dummy,*p,*prep,*q,*nextq,*temp;
	
	dummy.next=head;
	prep=q=&dummy;
	
	while (--m)prep=prep->next; //note --m;
	while (n--)q=q->next;     //note n--;
	/*now prep point to prev p, q point to q exactly*/
	
	
	p=prep->next;
	nextq=q->next;//save q->next
	temp=p;//save p ;
	q->next=NULL;
	
	prep->next=reverse(p); //now p to q is a longly list
	temp->next=nextq;
	
	return dummy.next;
}


//意思是把链表第m节点和第n节点的顺序交换
node*reverseTwo(node*head,int m,int n){
	if(m==n) return head;
	if(!head||!head->next) return head;
	
	node dummy,*p,*prep,*q,*preq;
	
	dummy.next=head;
	prep=preq=&dummy;
	
	while (--m)prep=prep->next;
	while (--n)preq=preq->next;
	
	p=prep->next;q=preq->next;
	if(p->next==q){//p and q are neighbour
		p->next=q->next;
		q->next=p;
		prep->next=q;
		return dummy.next;
	}
	preq->next=q->next;q->next=p->next;prep->next=q; //delete q and insert q
	p->next=preq->next;preq->next=p; //insert p;
	return dummy.next;
}
