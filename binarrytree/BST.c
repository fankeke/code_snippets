
// 因为一直看到红黑�🌲，然而奈何其re－balance算法看的云里雾里，，故而学习BST，BST还是比较简单的
// rbtree就是在BST基础上面，多了自平衡（re-banlance） 但是没有AVL的平衡要求高，故而平衡时间短，可以达到AVL的性能还可以时间少，所以工程上
//大都用rbtree


//reference:                  http://blog.csdn.net/yankai0219/article/details/8273542

／／这里的BST也是取自rbtree的部分，简单把color成员去掉即可。参考 ngx_rbtree.h ngx_rbtree.c

// bst.h

#ifndef _BST_H_
#define _BST_H_

typedef struct bstree_node_s bstree_node_t;
typedef struct bstree_s bstree_t;

struct bstree_node_s{
    bstree_node_t*left;
    bstree_node_t*right;
    bstree_node_t*parent;
    int  key;
};

typedef void(*bstree_node_insert_pt)(bstree_node_t*root,bstree_node_t*node,
	bstree_node_t*sentinel);


struct bstree_s{
    bstree_node_t 		*root;
    bstree_node_t 		*sentinel;
    bstree_node_insert_pt	insert;
};

bstree_t*bstree_create(bstree_node_t*sentinel,bstree_node_insert_pt i);
void bstree_insert(bstree_t*tree,bstree_node_t*node);
void bstree_delete(bstree_t*tree,bstree_node_t*node);
bstree_node_t*bstree_min(bstree_node_t*node,bstree_node_t*sentinel);
bstree_node_t*bstree_lookup(bstree_t*tree,int key);


void 
bstree_insert_value(bstree_node_t*root,bstree_node_t*node,bstree_node_t*sentinel);



#endif





//bst.h

#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bstree_node_t*bstree_lookup(bstree_t*tree,int key)
{
    bstree_node_t		*p=tree->root;
    while(p!=tree->sentinel){
	if(p->key==key) return p;
	p=(p->key<key)?p->right:p->left;
    }
    return p;
}

void bstree_insert_value(bstree_node_t*root,
	bstree_node_t*node,bstree_node_t*sentinel)
{
    bstree_node_t		**p;
    for(;;){
     	p=(node->key<root->key)?&root->left:&root->right;
	if(*p==sentinel) break;
	root=*p;
    }
    node->parent=root;
    *p=node;
    node->left=node->right=sentinel;
}

void bstree_insert(bstree_t*tree,bstree_node_t*node)
{
    bstree_node_t			**p;
    p=&tree->root;
    if(*p==tree->sentinel){
	*p=node;
	node->parent=node->left=node->right=tree->sentinel;
	//root->parent point to sentinel;
	return;
    }
    tree->insert(*p,node,tree->sentinel);
}

void bstree_delete(bstree_t*tree,bstree_node_t*node)
{
    //subst is the real node to be delete,temp is node to replace subst
    bstree_node_t		*sentinel,*subst,*temp;

    sentinel=tree->sentinel;

    if(node->left==sentinel){
	subst=node;
	temp=node->right;
    }else if(node->right==sentinel){
	subst=node;
	temp=node->left;
    }else{
	subst=bstree_min(node->right,sentinel);
	temp=subst->right;
    }

    temp->parent=subst->parent;

    //so delete root is simple 
    if(subst->parent==sentinel)
	tree->root=temp;

    else if(subst->parent->left==subst)
	subst->parent->left=temp;
    else
	subst->parent->right=temp;

    if(subst!=node){
	node->key=subst->key;
    }

    free(subst);
}

bstree_t*bstree_create(bstree_node_t*sentinel,bstree_node_insert_pt i)
{
    bstree_t	*tree;

    if((tree=malloc(sizeof(*tree)))==NULL)
	return NULL;
    tree->root=sentinel;
    tree->sentinel=sentinel;
    tree->insert=i;
    return tree;
}
bstree_node_t*bstree_min(bstree_node_t*node,bstree_node_t*sentinel)
{
    if(node==sentinel) return sentinel;

    while(node->left!=sentinel){
	node=node->left;
    }
    return node;
}
static void
inordertravel(bstree_node_t*root,bstree_node_t*sentinel)
{
    if(root!=sentinel){
	inordertravel(root->left,sentinel);
	printf("%d ",root->key);
	inordertravel(root->right,sentinel);
    }
}

static void
inorder(bstree_t*tree)
{
    inordertravel(tree->root,tree->sentinel);
    printf("\n");
}

int main(int argc,char**argv)
{

    int i,n=atoi(argv[1]);
    bstree_node_t	*sentinel,*node;
    if((sentinel=malloc(sizeof(*sentinel)))==NULL)
	return 1;
    bstree_t *tree=bstree_create(sentinel,bstree_insert_value);
    srand(time(NULL));

    //create node
    for(i=0;i<n;i++){
	node=malloc(sizeof(*node));
	node->left=node->right=NULL;
	node->key=random()%100;
	printf("%d ",node->key);
	bstree_insert(tree,node);
    }
    printf("\n");

    //travel
    inorder(tree);

    //lookup
    for(i=0;i<n;i++){
	int key;
	scanf("%d",&key);
	bstree_node_t*temp=bstree_lookup(tree,key);
	if(temp==tree->sentinel){
	    printf("not found ,sorry!\n");
	}else{
	    printf("found node: %p key %d\n",temp,temp->key);
	    bstree_delete(tree,temp);
	}
    }


    for(i=0;i<n;i++){
	bstree_node_t*p=bstree_min(tree->root,tree->sentinel);
	if(p==tree->sentinel) break;
	printf("current min node is %d\n",p->key);
	bstree_delete(tree,p);
    }
    return 0;

}
    

// Makefile

cc=gcc
link=$(cc)
cflags=-Wall -g -O0

incs=-I ./
deps=

target=bst

obj=bst.o

all:$(target)

$(target):$(obj)
	$(link) -o $(target) $(obj)


bst.o: bst.h bst.c $(deps)
	$(cc) -c $(cflags) $(incs) -o bst.o bst.c

clean:
	rm -rf $(target) $(obj)
	
	
	
