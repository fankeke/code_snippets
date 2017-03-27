
//我的hash表很简单，就是多个链表并列在一起。。。。


typedef struct node_s node;
struct node_s {
    int val;
    node*next;
};

typedef struct {
    int     size;
    node    **table;
}hash;
int insertnode(hash*h,int val)
{
    int index=val%(h->size);
    index=index>=0?index:-index;
    node*p=h->table[index];
    while(p){
        if(p->val==val) return 0;
        p=p->next;
    }
    
    p=(node*)malloc(sizeof(node));
    p->val=val;
    p->next=h->table[index];
    h->table[index]=p;
    return 1;
}
bool containsDuplicate(int* nums, int numsSize) {
    if(numsSize<=1) return false;
    int i;
    hash    *h;
    if((h=malloc(sizeof(hash)))==NULL) return false;
    
    h->size=numsSize/2;
    h->table=(node**)malloc(sizeof(node*)*h->size);
    if(h->table==NULL) return false;
    
    for(i=0;i<h->size;i++) h->table[i]=NULL;
    
    for(i=0;i<numsSize;i++){
        if((insertnode(h,nums[i]))==0) return true;
    }
    return false;
    
}
