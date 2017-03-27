#include <stdio.h>
#include <stdlib.h>
#include <adlist.h>

void listpretraval(list*list)
{
    listIter	*iter;
    listNode	*node;

    if(list->len==0){
	printf("链表中没有任何元素，返回\n");
	return ;
    }
    printf("链表元素:");
    iter=listGetIterator(list,0);
    while((node=listNext(iter))!=NULL){
	printf("%d ",*(int*)(node->value));
    }
    printf("\n");
}
void listposttraval(list*list)
{ 
    listIter	*iter;
    listNode	*node;

    iter=listGetIterator(list,1);
    while((node=listNext(iter))!=NULL){
	printf("%d ",*(int*)(node->value));
    }
    printf("\n");


 /*   listNode	*p=list->tail;
    unsigned long len=list->len;
    while(len--){
	printf("%d ",*(int*)(p->value));
	p=p->prev;
    }
    printf("\n");*/
}
list* listCreate(void)
{
    list	*list;

    if((list=malloc(sizeof(*list)))==NULL){
	return NULL;
    }
    list->head=list->tail=NULL;
    list->len=0;
    list->dup=NULL;
    list->free=NULL;
    list->match=NULL;
    return list;
}

void listRelease(list*list)
{
    unsigned long len;
    listNode *current,*next;
    current=list->head;
    len=list->len;

    while(len--){
	next=current->next;
	if(list->free) list->free(current->value);
	free(current);
	current=next;
    }
    free(list);
}
list*listAddNodeHead(list*list,void*value)
{
    //第一个node的prev为空
    
    listNode	*node;
    if((node=malloc(sizeof(*node)))==NULL){
	return NULL;
    }
    node->value=value;

    if(list->len==0){
	list->head=list->tail=node;
	node->prev=node->next=NULL;
    }else{
	node->prev=NULL;
	node->next=list->head;
	list->head->prev=node;
	list->head=node;
    }
    list->len++;
    return list;
}
list*listAddNodeTail(list*list,void*value)
{
    listNode	*node;
    if((node=malloc(sizeof(*node)))==NULL){
	return NULL;
    }

    node->value=value;
    if(list->len==0){
	list->head=list->tail=node;
	node->prev=node->next=NULL;
    }else{
	node->prev=list->tail;
	node->next=NULL;
	list->tail->next=node;
	list->tail=node;
    }
    list->len++;
    return list;
}
list*listInsertNode(list*list,listNode*old_node,void*value,int after)
{
    listNode	*node;
    if((node=malloc(sizeof(*node)))==NULL){
	return NULL;
    }

    node->value=value;
    if(after){//插入到old之后
	node->prev=old_node;
	node->next=old_node->next;
	if(list->tail==old_node){//old为最后一个listNode
	    list->tail=node;	//更新最后的node为插入的node
	}
    }else{//插入old之前
	node->prev=old_node->prev;
	node->next=old_node;
	if(list->head==old_node){
	    list->head=node;
	}
    }

    //完善最后的指针搭建,双向表最后完善不是当前节点的指针不易出错
    if(node->prev!=NULL){
	node->prev->next=node;
    }
    if(node->next!=NULL){
	node->next->prev=node;
    }
    list->len++;
    return list;
}

void listDelNode(list*list,listNode*node)
{
    if(node->prev){
	node->prev->next=node->next;
    }else{//删除的是首节点
	list->head=node->next;
    }

    if(node->next){
	node->next->prev=node->prev;
    }else{//删除最后节点
	list->tail=node->prev;
    }

    if(list->free) list->free(node->value);
    free(node);
    list->len--;
}

//一般迭代器是和某个list相关的，只有在需要遍历的
//时候需要使用iter，再不需要遍历的时候释放掉iter
listIter* listGetIterator(list*list,int direction)
{
    listIter *iter;
    if((iter=malloc(sizeof(*iter)))==NULL){
	return NULL;
    }
    if(direction==0)//遍历方向
	iter->next=list->head;
    else
	iter->next=list->tail;

    iter->direction=direction;
    return iter;
}

void listReleaseIterator(listIter*iter)
{
    free(iter);
}
//
//正方向改变
void listRewind(list*list,listIter*li)
{
    li->next=list->head;
    li->direction=0;
}

void listRewindTail(list*list,listIter*li)
{
    li->next=list->tail;
    li->direction=1;
}

//返回当前iter做指向的node，然后使iter指向下一个node
listNode*listNext(listIter*iter)
{
    listNode*current=iter->next;
    if(current!=NULL){
	if(iter->direction==0){
	    iter->next=current->next;
	}else{
	    iter->next=current->prev;
	}
    }
    return current;
}


list *listDup(list*orig)
{
    list	*copy;
    listIter	*iter;
    listNode	*node;

    if((copy=listCreate())==NULL){
	return NULL;
    }
    copy->dup=orig->dup;
    copy->free=orig->free;
    copy->match=orig->match;

    iter=listGetIterator(orig,0);//创建一个和orig相关的iter

    while((node=listNext(iter))!=NULL){
	void *value;
	if(copy->dup){//如果list存在copy函数,需要执行
	    value=copy->dup(node->value);
	    if(value==NULL){
		listRelease(copy);
		listReleaseIterator(iter);
		return NULL;
	    }
	}else{
	    value=node->value;
	}

	if(listAddNodeTail(copy,value)==NULL){
	    listRelease(copy);
	    listReleaseIterator(iter);
	    return NULL;
	}
    }
    listReleaseIterator(iter);
    return copy;
}

listNode*listSearchKey(list*list,void*key)
{
    listIter	*iter;
    listNode	*node;

    iter=listGetIterator(list,0);//创建正向iter
    while((node=listNext(iter))!=NULL){
	if(list->match){
	    if(list->match(node->value,key)){
		listReleaseIterator(iter);
		return node;
	    }
	}else{//默认比较规则是==
	    if(key==node->value){
		listReleaseIterator(iter);
		return node;
	    }
	}
    }
    listReleaseIterator(iter);
    return NULL;
}

listNode*listIndex(list*list,long index)
{
    //举例来说，index=0，返回第一个节点，index=1，返回第2个
    //index=-1，返回最后一个，index=-2，返回倒数第2个
    listNode	*n;
    if(index<0){//如果是负数，则从尾部开始数
	index=(-index)-1;
	n=list->tail;
	while(index--&&n) n=n->prev;
    }else{
	n=list->head;
	while(index--&&n) n=n->next;
    }
    return n;
}

//把list的尾节点当作首节点
void listRotate(list*list)
{
    listNode*tail=list->tail;
    if(listLength(list)<=1) return ;

    list->tail=tail->prev;
    list->tail->next=NULL;

    list->head->prev=tail;
    tail->prev=NULL;
    tail->next=list->head;
    list->head=tail;
}

