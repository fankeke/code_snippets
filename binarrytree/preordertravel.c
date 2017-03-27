
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

int* preordertrave(node*root,int*size)
{
    stack*st=stack_init(10000);
    int*ans=malloc(sizeof(int)*10000);
    if(!root){
	*size=0;
	return ans;
    }
    stack_push(st,root);
    int count=0;
    do{
	node*n=stack_pop(st);
	ans[count++]=n->val;
	if(n->right) stack_push(st,n->right);
	if(n->left) stack_push(st,n->left);
    }while(!stack_empty(st));

    *size=count;
    return st;
}
