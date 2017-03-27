#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <adlist.h>
list	*listall;
void print_menu();
void create_list()
{
    int		 n,i;
    int		*elem;
    list	*ll;
    ll=listCreate();
    printf("给你的链表起个名字吧:(限于字母）");
    scanf("%s",ll->name);
    printf("输入初始化时候链表需要插入多少元素：");
    scanf("%d",&n);
    printf("依次输入插入的元素：");
    for(i=0;i<n;i++){
	elem=(int*)malloc(sizeof(int));
	scanf("%d",elem);
	ll=listAddNodeTail(ll,elem);
    }
    listall=listAddNodeTail(listall,ll);
    printf("创建完毕！\n");
}
void displayall_list()
{
    if(listall->len==0){
	printf("没有任何链表，返回\n");
	return ;
    }
    listIter	*iter=listGetIterator(listall,0);
    listNode	*node;
    while((node=listNext(iter))!=NULL){
	list	*ll=(list*)node->value;
	printf("链表%s:",ll->name);
	listpretraval(ll);
    }
    listReleaseIterator(iter);
}
static list* found_list(listNode**pnode)
{
    if(listall->len==0){
	printf("没有任何链表\n");
	return NULL;
    }
    char	name[10];
    listNode	*node;
    listIter	*iter;
    list	*ll;
    printf("输入链表的名字:");
    scanf("%s",name);

    iter=listGetIterator(listall,0);
    while((node=listNext(iter))!=NULL){
	ll=(list*)(node->value);
	if(strcmp(ll->name,name)==0){
	    *pnode=node;
	    return ll;
	}
    }
    listReleaseIterator(iter);
    return NULL;
}
void print_list()
{
    listNode	*node;
    list	*ll=found_list(&node);
    if(ll){
	listpretraval(ll);
    }else{
	printf("没有这个链表\n");
    }
}
void delete_list()
{
    listNode	*node;
    list	*ll=found_list(&node);
    if(ll){
	listDelNode(listall,node);
	listRelease(ll);
	printf("释放完毕\n");
    }else{
	printf("没有这个链表\n");
    }
}
void insert_list()
{
    listNode	*node;
    list	*ll=found_list(&node);
    int		*elem;

    if(ll){
	elem=(int*)malloc(sizeof(int));
	printf("输入要插入的元素:");
	scanf("%d",elem);
	if((listAddNodeTail(ll,elem))!=NULL){
	    printf("插入成功\n");
	}else{
	    printf("插入失败\n");
	}
    }else{
	printf("没有这个链表\n");
    }
}
void dup_list()
{
    listNode	*node;
    list	*orig=found_list(&node);
    list	*ll;
    if(orig){
	ll=listDup(orig);
	printf("输入复制过后的链表名称:");
	scanf("%s",(ll->name));
	listall=listAddNodeTail(listall,ll);
	printf("复制成功\n");
    }else{
	printf("没有这个链表\n");
    }
}

int main(int argc,char**argv)
{
    char 	command[10];
    listall=listCreate();
    while(1){
	fflush(stdout);
	print_menu();
	printf("input choice:");
	scanf("%s",command);
	switch(command[0]){
	    case '0':
		exit(0);
		break;
	    case '1':
		create_list();
		break;
	    case '2':
		delete_list();
		break;
	    case '3':
		print_list();
		break;
	    case '4':
		insert_list();
		break;
	    case '5':
		dup_list();
		break;
	    case '6':
		displayall_list();
		break;
/*	    default:
		error_handler();
		break;
		*/
	}
    }
    return 0;
}

void print_menu()
{
    printf("---------------------------------------------\n");
    printf("---------------------------------------------\n");
    printf("---------------------------------------------\n");
    printf("---------------------------------------------\n");
    printf("1,----------------创建链表------------------\n");
    printf("2,----------------删除链表------------------\n");
    printf("3,----------------打印链表------------------\n");
    printf("4,----------------插入数据------------------\n");
    printf("5,----------------复制链表------------------\n");
    printf("6,----------------显示所有链表--------------\n");
    printf("0,----------------退出程序-------------------\n");
    printf("---------------------------------------------\n");
    printf("---------------------------------------------\n");
}
    
    



