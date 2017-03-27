#ifndef _ADLIST_H_INCLDED_H_
#define _ADLIST_H_INCLDED_H_


#define AL_START_HEAD	0
#define AL_START_TAIL	1

typedef struct listNode_s listNode;
typedef struct list_s 	list;
typedef struct listIter_s listIter;


struct listIter_s {
    listNode	*next;
    int		direction;
};

struct listNode_s{
    listNode	*prev;
    listNode	*next;
    void	*value;
};

struct list_s{
    listNode	*head;
    listNode	*tail;

    void*(*dup)(void*ptr);
    void(*free)(void*ptr);
    int (*match)(void*ptr,void*key);
    unsigned long len;
};
#define listLength(l)  ((l)->len)


listIter*listGetIterator(list*list,int direction);
listNode*listNext(listIter*iter);
void listReleaseIterator(listIter*iter);

list*listCreate(void);

list*listAddNodeTail(list*list,void*value);

listNode*listSearchKey(list*list,void*key);

void listDelNode(list*list,listNode*node);

#endif
