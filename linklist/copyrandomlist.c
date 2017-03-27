 Copy List with Random Pointer My Submissions Question
Total Accepted: 53470 Total Submissions: 208111 Difficulty: Hard
A linked list is given such that each node contains an additional random pointer which could point to any node in the list or null.

Return a deep copy of the list.

https://leetcode.com/problems/copy-list-with-random-pointer/
 
 //best code: others
 
 
 /* so the point of this problem is how to locate every node's random point in copy list
  * this solution pose:  we copy every node of orig list and insert every orig node 
  * so ,if we want find the new node's random point ,we could use the orig node's random point . :p->next->random=p->random->next;
  * after done, we could restore the orig list and extract the copy list .
  */
  
  //notice ,after malloc, we should initilaztion the struct ,or it may cause some problem for you to debug it ....
  //in this ,I don't init the new node's random to NULL ,it crashed , and waste me a lot of time to debug ...
 
 typedef struct node_s node;
 struct node_s {
     int val;
     node*next,*random;
 };
node *copyRandomList(node *head) {
    node dummy,*p,*i,*r;
    
    
    //insert every p after p;
    p=head;
    while(p){
        r=p->next;
        p->next=malloc(sizeof(node));
        p->next->val=p->val;
        p->next->next=r;
        p->next->random=NULL;          //important !!!
        p=r;
    }
    
    p=head;
    while(p){
        if(p->random){
            p->next->random=p->random->next; //what we really want....
        }
        p=p->next->next;// p->next must't be NULL, because we have double's count  node
    }
    
    
    dummy.next=NULL;
    r=&dummy;
    p=head;
    while(p){
        
        //i node is we need. 
        i=p->next;
        r->next=i;r=i;
        
        p->next=i->next;//delete i;
        p=p->next;
    }
    
    return dummy.next;
}
