
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct list_node_s list_node;

//the node is just a array ,the real item is stored in the node's array
struct list_node_s{
    void		*elts;
    int			 nelts;
    list_node		*next;
};

typedef struct {
    list_node		*last;
    list_node		 start;
    size_t		 size;
    int			 nalloc;
}list;

   
list*list_create(int n,size_t size);
void*list_push(list*list);


//create a list ,it's node's nalloc is n,and node's element size si size
list*list_create(int n,size_t size)
{
    list			*list;
    list_node			*node;
    if((list=malloc(sizeof(*list)))==NULL)
	return NULL;

    node=&list->start;
    node->elts=(char*)malloc(n*size);
    if(node->elts==NULL){
	free(list);
	return NULL;
    }
    node->nelts=0;
    node->next=NULL;

    list->last=node;
    list->size=size;
    list->nalloc=n;

    return list;
}

void*list_push(list*list)
{
    list_node			*last;
    void			*elts;

    last=list->last;
    if(last->nelts==list->nalloc){
	if((last=malloc(sizeof(*last)))==NULL)
	    return NULL;
	if((last->elts=malloc(list->nalloc*list->size))==NULL)
	    return NULL;
	list->last->next=last;
	list->last=last;
    }
    elts=(char*)last->elts+list->size*last->nelts;
    last->nelts++;
    return elts;
}

void list_print(list*list)
{
    list_node			*node;
    int				*elts,i;

    node=&(list->start);
    elts=node->elts;

    for(i=0;/*void*/;i++){
	if(i>=node->nelts){
	    if(node->next==NULL)
		break;
	    node=node->next;
	    elts=node->elts;
	    i=0;
	}
	printf("%d ",elts[i]);
    }
    printf("\n");
}

int main(int argc,char**argv)
{
    list	*list;
    int		 i,n=atoi(argv[1]);
    int		*elts;
    srand(time(NULL));
    if((list=list_create(4,sizeof(int)))==NULL){
	fprintf(stderr,"%s","create\n");
	exit(0);
    }
    for(i=0;i<n;i++){
	if((elts=list_push(list))==NULL){
	    fprintf(stderr,"%s\n","push");
	    exit(0);
	}
	*elts=random()%100;
    }

    list_print(list);
    return 0;
}


/*

 list               list_node 
－－－－|           |------|     
  start |           | elts*----->|-----|
  last  |           |      |     |     |
  size  |           --------     |     |
  nalloc|                        |     |
－－－－|                        ------

*/


















