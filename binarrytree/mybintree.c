

用到两个棧，一个用来存放路径，一个用来控制走向，即flag ：0 左边，1，右边，2，回溯

//my code:


typedef struct node_s node;
 struct node_s {
     int val;
     node*right,*left;
 };
 typedef struct {
     node**array;
     int cur;
 }stack;
 typedef struct {
     int *array;
     int cur;
 }stack_flag;
 
 stack*stack_init(int n)
 {
     stack*s=malloc(sizeof(stack));
     s->array=(node**)malloc(sizeof(node*)*n);
     s->cur=0;
     return s;
 }
 stack_flag*stack_flag_init(int n)
 {
     stack_flag*sf=malloc(sizeof(stack_flag));
     sf->array=(int*)malloc(sizeof(int)*n);
     sf->cur=0;
     return sf;
 }
 void stack_push(stack*s,node*p)
 {
     s->array[s->cur++]=p;
 }
 void stack_flag_push(stack_flag*sf,int p)
 {
     sf->array[sf->cur++]=p;
 }
 node*stack_pop(stack*s)
 {
     s->cur--;
     return s->array[s->cur];
 }
 int stack_flag_pop(stack_flag*sf)
 {
     sf->cur--;
     return sf->array[sf->cur];
 }
 
 node*stack_top(stack*s)
 {
     int cur=s->cur-1;
     return s->array[cur];
 }
 int stack_flag_top(stack_flag*sf)
 {
     int cur=sf->cur-1;
     return sf->array[cur];
 }
 
 int stack_empty(stack*s)
 {
     return s->cur==0;
 }
 int stack_size(stack*s)
 {
     return s->cur;
 }
 
void pathxxx(node* root) {
        
    
    
    stack*s=stack_init(10000);
    stack_flag*sf=stack_flag_init(10000);
    
    stack_push(s,root);
    stack_flag_push(sf,0);
    
    while(!stack_empty(s)){
        node*p=stack_top(s);
        int flag=stack_flag_top(sf);
        if(p->left&&flag<=0){            //存在left，且未访问过
            stack_flag_pop(sf);stack_flag_push(sf,1);  //标记访问过left
            stack_push(s,p->left);stack_flag_push(sf,0);//left入棧，同时left的标记设为未访问过0
        }else if(p->right&&flag<=1){        //存在right，且right未访问过 
            stack_flag_pop(sf);stack_flag_push(sf,2);         
            stack_push(s,p->right);stack_flag_push(sf,0);
        }else{                                              //需要回溯了，要么是到了叶子节点，要么是某个节点的left，right都访问过了
            if(!p->left&&!p->right){                        //情况1：叶子节点
                xxxx
            }                                         
                                                            //情况二：访问过了
            
            
            stack_pop(s);stack_flag_pop(sf);              //两种情况下都需要回溯
        }
    }
}
