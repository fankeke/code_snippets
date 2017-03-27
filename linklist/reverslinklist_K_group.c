Reverse Nodes in k-Group My Submissions Question
Total Accepted: 48176 Total Submissions: 184196 Difficulty: Hard
Given a linked list, reverse the nodes of a linked list k at a time and return its modified list.

If the number of nodes is not a multiple of k then left-out nodes in the end should remain as it is.

You may not alter the values in the nodes, only nodes itself may be changed.

Only constant memory is allowed.

For example,
Given this linked list: 1->2->3->4->5

For k = 2, you should return: 2->1->4->3->5

For k = 3, you should return: 3->2->1->4->5

Subscribe to see which companies asked this question


///my code:
 
 typedef struct node_s node;
 struct node_s {
     int val;
     node*next;
 };
 node*reverse(node*h)
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
node* reverseKGroup(node* head, int k) {
    if(!head||!head->next) return head;
    node dummy,*p,*q,*temp,*nextq;
    dummy.next=head;
    int n;
    p=&dummy;                                                /*思路其实很简单，就是把K长度的一小段反转，接着做到底，需要
                                                              注意的就是 1，dummy使得没有特殊情况，2，需要保存反转后的起止节点，这样
                                                              temp就是保存反转后的末尾节点，reverse返回反转后的其实节点，nextq为了进行
                                                              下一步需要保存。3，注意q使得函数会结束需要做出判断（不足K个点），4更新
                                                              p的时候，注意更新到temp。即上一段的末尾节点。
    while(p->next){
        n=k;
        q=p->next;
        while(q&&--n) q=q->next;
        if(!q) break;
        temp=p->next;
        nextq=q->next;q->next=NULL;
        p->next=reverse(p->next);
        temp->next=nextq;
        p=temp;
    }
    return dummy.next;
}
