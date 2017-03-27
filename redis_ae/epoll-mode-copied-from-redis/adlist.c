#include <stdlib.h>
#include "adlist.h"

list*listCreate(void)
{
    list	*list;

    if((list=malloc(sizeof(*list)))==NULL)
	return NULL;
    list->head=list->tail=NULL;
    list->len=0;
    list->dup=NULL;
    list->free=NULL;
    list->match=NULL;

    return list;
}


list*listAddNodeTail(list*list,void*value)
{
    listNode	*node;
    if((node=malloc(sizeof(*node)))==NULL)
	return NULL;
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

	
listNode*listSearchKey(list*list,void*key)
{
    listIter*iter;
    listNode *node;

    iter=listGetIterator(list,AL_START_HEAD);
    while((node=listNext(iter))!=NULL){
	if(list->match){
	    ;
	}else{
	    if(key==node->value){
		listReleaseIterator(iter);
		return node;
	    }
	}
    }

    listReleaseIterator(iter);
    return NULL;
}


void listDelNode(list*list,listNode*node)
{
    if(node->prev){
	node->prev->next=node->next;
    }else{
	list->head=node->next;
    }

    if(node->next){
	node->next->prev=node->prev;
    }else{
	list->tail=node->prev;
    }

    if(list->free) list->free(node->value);

    free(node);
    list->len--;
}



listIter*listGetIterator(list*list,int direction)
{
    listIter *iter=malloc(sizeof(*iter));
    if(direction==AL_START_HEAD)
	iter->next=list->head;
    else
	iter->next=list->tail;
    iter->direction=direction;

    return iter;
}
listNode*listNext(listIter*iter)
{
    listNode	*node=iter->next;
    if(node!=NULL){
	if(iter->direction==AL_START_HEAD)
	    iter->next=node->next;
	else
	    iter->next=node->prev;
    }
    return node;
}
void listReleaseIterator(listIter*iter)
{
    free(iter);
}
