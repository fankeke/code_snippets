#ifndef __ADLIST_H__
#define __ADLIST_H__

typedef struct listNode listNode;
struct listNode{
    listNode	*prev;
    listNode	*next;
    void	*value;
};

typedef struct list list;
struct list{
    char	 name[10];
    listNode	*head;
    listNode	*tail;
    void *(*dup)(void*ptr);
    void *(*free)(void*ptr);
    int	 *(*match)(void*ptr,void*key);
    unsigned long len;
};

//对list的迭代器定义,主要为了遍历list方便
typedef struct {
    listNode	*next;
    int		 direction;
}listIter;


#define listLength(l) ((l)->len)



void listpretraval(list*list);
void listposttraval(list*list);
list*listCreate(void);
void listRelease(list*list);
list*listAddNodeHead(list*list,void*value);
list*listAddNodeTail(list*list,void*value);

list*listInsertNode(list*list,listNode*old_node,void*value,int after);
void listDelNode(list*list,listNode*node);


listIter* listGetIterator(list*list,int direction);
void listReleaseIterator(listIter*iter);
void listRewind(list*list,listIter*li);//更改某个迭代器使其与list相关
void listRewindTail(list*list,listIter*li);//更改某个迭代器使其与list相关
listNode*listNext(listIter*iter);


list*listDup(list*orig);//复制作一个和orig一样的list，返回复制后的list
listNode*listSearchKey(list*list,void*key);//通过key来找到node
//默认的比较规则是key和node->value相等，如果list有match函数，则执行match函数来匹配


listNode*listIndex(list*list,long index);//返回第index个节点

void listRotate(list*list);

#endif
