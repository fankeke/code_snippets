https://leetcode.com/submissions/detail/47124957/


//my code:

typedef struct node_s node;
 struct node_s {
     int val;
     node*next;
 };
 node* insertnode(node*head,node*i)
 {
     node *pre,*p=head;
     while(p&&p->val<i->val){
         pre=p;
         p=p->next;
     }
     if(p==head){
         i->next=head;
         head=i;
         return head;
     }
     i->next=p;
     pre->next=i;
     return head;
 }
node* insertionSortList(node* head) {
    if(!head||!head->next){
        return head;
    }
    node *p,*q;
    p=head->next;
    head->next=NULL;
    while(p){
        q=p->next;p->next=NULL;                  
        head=insertnode(head,p);        //一个一个的把节点插入到链表中
        p=q;
    }
    return head;
    
}



//best code :


node* insertionSortList(node* head) {
    if(!head||!head->next){
        return head;
    }
    node        dummy,*cur,*next,*pre;
    pre=&dummy;
    pre->next=NULL;
    next=NULL;
    cur=head;
    
    //也是直接一个个的插入，不过不用调用函数，
    //需要注意的就是要pre的用法
    while(cur){
        next=cur->next;
        while(pre->next&&pre->next->val<cur->val){
            pre=pre->next;
        }
        cur->next=pre->next;
        pre->next=cur;
        pre=&dummy;
        cur=next;
            
    }
    return dummy.next;
}
