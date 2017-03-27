typedef struct t_node_s t_node;
 struct t_node_s{
     int val;
     t_node*left,*right;
 };
 typedef struct node_s node;
 struct node_s{
     int val;
     node*next
 };
 
t_node* sortedListToBST(node* head) {

    if(!head) return NULL;
    node*slow,*fast,*prev;
    fast=slow=head;
    prev=NULL;
    while(fast->next&& fast->next->next ){
        prev=slow;
        slow=slow->next;
        fast=fast->next->next;
    }
    fast=slow->next;
    slow->next=NULL;
    
    t_node*root=malloc(sizeof(*root));
    root->val=slow->val;
    root->right=sortedListToBST(fast);
    if(!prev) root->left=NULL;
    else {
        prev->next=NULL;
        root->left=sortedListToBST(head);
    }
    
    return root;
    
    
    
    
    
}
