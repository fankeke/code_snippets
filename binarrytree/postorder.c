
typedef struct node_s node;
struct node_s{
    int val;
    node*left,*right;
};

typedef struct {
    int **base,**top;
}stack;

stack*stack_init(int size)
{
    stack*st=malloc(sizeof(*st));
    st->base=st->top=(node**)malloc(sizeof(node*)*size);
    return st;
}
node*stack_pop(stack*st)
{
    node*n=*(st->top);
    st->top--;
    return n;
}
void stack_push(stack*st,node*n)
{
    st->top++;
    *(st->top)=n;
}
int stack_empty(stack*st)
{
    return st->base==st->top;
}


//we can learn from preosrder,push order is:root,right,left. can get root,left,right order
//just adjust push order to :root,left,right. can get root,right,left order, just opposite of postorder
//so only to reverset it can get postorder.... left ,right ,root. all we need is another stack st_temp..
int* postordertrave(node*root,int*size)
{
    stack*st=stack_init(10000);
    stack*st_temp=stack_init(10000);
    int*ans=malloc(sizeof(int)*10000);
    if(!root){
	*size=0;
	return ans;
    }
    stack_push(st,root);
    int count=0;
    while(!stack_empty(st)){
	node*n=stack_pop(st);
	stack_push(st_temp,n);
	if(n->left) stack_push(st,n->left);
	if(n->right) stack_push(st,n->right);
    };
    while(!stack_empty(st_temp)){
	node*n=stack_pop(st_temp);
	ans[count++]=n->val;
    }
    *size=count;
    return ans;
}
