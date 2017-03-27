
typedef struct node_s node;
struct node_s{
    int val;
    node*left,*right;
};

typedef struct{
    node**front,**rear;
}queue;

queue*queue_init(int size)
{
    queue*q=malloc(sizeof(*q));
    q->front=q->rear=malloc(sizeof(node*)*size);
    return q;
}

node* queue_pop(queue*q)
{
    node*n=*(q->front);
    q->front++;
    return n;
}
void queue_push(queue*q,node*n)
{
    *(q->rear)=n;
    q->rear++;
}
int queue_empty(queue*q)
{
    return q->front==q->rear;
}
int queue_size(queue*q)
{
    return q->rear-q->front;
}



//Point!!:two dimesion arry is just a one dimension arrry that every element is a one dimension arry point ...
//so we can operation the POINT what we want..
int** leveltrave(node*root,int**colnum,int*size)
{
    int**ans=malloc(sizeof(int*)*10000);
    int*col=malloc(sizeof(int)*10000);
    if(!root){
	*size=0;
	*colnum=NULL;
	return NULL;
    }
    queue*q=queue_init(10000);
    int count=0;
    node *n,*cur=root;
    queue_push(q,cur);
    while(!queue_empty(q)){
	int i,cur_len=queue_size(q);
	int*arr=malloc(sizeof(int)*cur_len);
	col[count]=cur_len;
	ans[count]=arr;
	for(i=0;i<cur_len;i++){
	    n=queue_pop(q);
	    arr[i]=n->val;
	    if(n->left) queue_push(q,n->left);
	    if(n->right) queue_push(q,n->right);
	}
	count++;
    }
    *size=count;
    *colnum=col;
    return ans;
}
