
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
node*stack_top(stack*st)
{
    return *(st->top);
}

int* inordertrave(node*root,int*size)
{
    stack*st=stack_init(10000);
    int*ans=malloc(sizeof(int)*10000);
    if(!root){
	*size=0;
	return ans;
    }
    int count=0;
    node*cur=root;
    while(!stack_empty(st)||cur){
	if(cur){
	    stack_push(st,cur);
	    cur=cur->left;
	}else{
	    cur=stack_pop(st);
	    ans[count++]=cur->val;
	    cur=cur->right;
	}
    }
    *size=count;
    return st;
}
