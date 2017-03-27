
/*因为实在是难得撸红黑树，（上次实现了BST），所以找到了skiplist，
  这货实现起来比较简单，而且效率都是log（n）
  */
  
  //referece:       http://dsqiu.iteye.com/blog/1705530
  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SKIPLEVEL	100

typedef struct skiplist_node_s skiplist_node;
typedef struct skiplist_s skiplist;

struct skiplist_node_s{
    int 			key;
    int				value;
    skiplist_node		*forward[1];        //柔性数组 
};

struct skiplist_s{
    skiplist_node		*head;            
    int				level;                  //保存现有节点中层数最大的节点的层数
};

static int skiprandomlevel()              //对于一个新的节点，随机其层数
{
    int k=1;
    int temp;
    while(1){
	temp=random()%100;
	if(temp%2) k++;
	else	break;
    }
    return k;
}

static skiplist_node*
slcreatenode(int level,int key,int value)
{
    skiplist_node	*n;
    if((n=malloc(sizeof(*n)+
		    sizeof(skiplist_node*)*level))==NULL){
	return NULL;
    }
    n->key=key;
    n->value=value;
    //level the initiation of  forwards[] to caller
    return n;
}


skiplist* slcreate(void)
{
    int i;
    skiplist	*sl;
    if((sl=malloc(sizeof(*sl)))==NULL)
	return NULL;

    sl->level=0;
    if((sl->head=slcreatenode(SKIPLEVEL,0,0))==NULL){
	free(sl);
	return NULL;
    }
    for(i=0;i<SKIPLEVEL;i++){
	sl->head->forward[i]=NULL;
    }
    return  sl;
}

int sldelete(skiplist*sl,int value)
{
    skiplist_node		*update[SKIPLEVEL],*p,*q=NULL;
    int				 i,k;

    k=sl->level;
    p=sl->head;

    for(i=k-1;i>=0;i--){
	q=p->forward[i];		//never back to head
	if(q==NULL||q->value>value) update[i]=p;
	else{
	    while(q&&q->value<value){
		p=q;
		q=q->forward[i];
	    }
	    update[i]=p;
	}
    }
    if(q->value==value){
	for(i=0;i<k;i++){
	    if(update[i]->forward[i]==q){
		update[i]->forward[i]=q->forward[i];
	    }
	}
	free(q);
	for(i=k-1;i>=0;i--){
	    if(sl->head->forward[i]==NULL){
		sl->level--;
	    }
	}

	return 0;
    }
    return -1;
}


int slinsert(skiplist*sl,int key,int value)
{
    skiplist_node		*update[SKIPLEVEL];
    skiplist_node		*p,*q=NULL;
    int				 k,i;

    p=sl->head;
    k=sl->level;

    for(i=k-1;i>=0;i--){
	q=p->forward[i];		//never back to head
	if(q==NULL||q->value>=value){
	    update[i]=p;
	}else{
	    while(q&&q->value<value){
		p=q;
		q=q->forward[i];
	    }
	    update[i]=p;
	}
    }

    k=skiprandomlevel();

    if(k>sl->level){
	for(i=sl->level;i<k;i++)
	    update[i]=sl->head;
	sl->level=k;
    }

    q=slcreatenode(k,key,value);
    for(i=0;i<k;i++){
	q->forward[i]=update[i]->forward[i];
	update[i]->forward[i]=q;
    }

    return 0;

}

void sldisplay(skiplist*sl)
{
    skiplist_node		*p,*q=NULL;
    int				 k,i;

    k=sl->level;

    for(i=k-1;i>=0; i--){
	p=sl->head;
	q=p->forward[i];
	while(q){
	    printf("%d-->",q->value);
	    q=q->forward[i];
	}
	printf("\n");
    }
    printf("\n");
}
skiplist_node*slsearch(skiplist*sl,int value)
{
    skiplist_node		*p,*q=NULL;
    int				 i,k;
    k=sl->level;

    p=sl->head;

    for(i=k-1;i>=0;i--){
	q=p->forward[i];		//never back to head
	if(q==NULL||q->value>value) continue;
	while(q&&q->value<=value){
	    if(q->value==value) return q;
	    p=q;
	    q=q->forward[i];
	}
    }
    return NULL;
}
/*---------------------------------------------------------------------------
---------------------------------------------------------------------------
---------------------------------------------------------------------------
*/

typedef struct doublelist_s doublelist;
typedef struct doublelist_node_s doublelist_node;

struct doublelist_node_s{
    int key;
    int value;
    doublelist_node*next;
    doublelist_node*prev;
};

struct doublelist_s{
    doublelist_node*head;
    int dummy;
};

int doubleinsert(doublelist*dl,int key,int value)
{
    doublelist_node*p,*h;
    p=malloc(sizeof(*p));
    p->key=key;
    p->value=value;

    h=dl->head;
    //insert head's next
    p->next=h->next;
    p->prev=h;
    h->next->prev=p;
    h->next=p;
    return 0;
}

doublelist_node*doublesearch(doublelist*dl,int value)
{
    doublelist_node*p=dl->head;
    while(p->next!=dl->head){
	if(p->value==value)
	    return p;
	p=p->next;
    }
    return NULL;
}

int double_delete(doublelist*dl,int value)
{
    doublelist_node	*p,*q;
    p=doublesearch(dl,value);
    if(p==NULL){
	return -1;
    }

    q=p->prev;
    q->next=p->next;
    p->next->prev=q;
    free(q);

    return 0;
}

doublelist_node*doublecreatenode(int key,int value)
{
    doublelist_node	*p;
    p=malloc(sizeof(*p));
    p->key=key;
    p->value=value;
    return p;
}

doublelist*doublecreate()
{
    doublelist*dl;
    dl=malloc(sizeof(*dl));
    dl->head=doublecreatenode(0,0);
    dl->head->next=dl->head->prev=dl->head;
    dl->dummy=0;
    return dl;
}

void doubledisplay(doublelist*dl)
{
    doublelist_node	*p=dl->head;
    while(p->next!=dl->head){
	printf("%d-->",p->next->value);
	p=p->next;
    }
    printf("\n");
}

int main(int argc,char**argv)
{
    srand(time(NULL));
    skiplist			*sl;
    skiplist_node		*p;
    doublelist			*dl;
    doublelist_node		*dp;

    sl=slcreate();
    dl=doublecreate();

    int temp,i,n=atoi(argv[1]);
    for(i=0;i<n;i++){
	temp=random()%10000;
	slinsert(sl,i,temp);
	doubleinsert(dl,i,temp);
    }
    sldisplay(sl);
    doubledisplay(dl);

    int value,rc;
    scanf("%d",&value);
    p=slsearch(sl,value);
    dp=doublesearch(dl,value);
    if(p==NULL){
	printf("not found!\n");
    }else{
	printf("found value:%d\n",p->value);
    }
    rc=sldelete(sl,value);
    if(rc!=0){
	printf("sl delete failed\n");
    }else{
	printf("sl successfuly delete\n");
	sldisplay(sl);
    }

    scanf("%d",&value);
    rc=double_delete(dl,value);
    if(rc!=0){
	printf("dl delete failed\n");
    }else{
	printf("dl successfuly delete\n");
	doubledisplay(dl);
    }
    return 0;
}









