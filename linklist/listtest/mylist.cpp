#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>

using namespace std;


int testpernum=40000;
int mylistsize=0;

struct element{
    int val;
};

typedef struct {
    struct element*p;
    void*prev;
    void*next;
}selflistelement;

selflistelement*mylisthead=NULL;
selflistelement*mylisttail=NULL;

list<struct element*>stllist;


void printusage(int argc,char**argv)
{
    printf("usage:%s\t[1|2][oneroundpressum\t1meas STL,2meas simple list oneroundpersum means in 3 seconds how many elements add/del in list\n",argv[0]);
}
void mylistadd(struct element*e)
{
    selflistelement*ele=(selflistelement*)malloc(sizeof(*ele));
    ele->p=e;
    mylistsize+=1;
    if(mylisthead==NULL){
	mylisthead=mylisttail=ele;
	ele->prev=ele->next=NULL;
	return;
    }
    ele->next=mylisthead;
    mylisthead->prev=ele;
    ele->prev=NULL;
    mylisthead=ele;
    return;
}

void testadd1()
{
    int i;
    for(i=0;i<testpernum;i++){
	struct element*e=(struct element*)malloc(sizeof(*e));
	stllist.push_front(e);
    }
}
void testdel1()
{
    //while(stllist.size()>0){
    while(!stllist.empty()){
	struct element*e=stllist.back();
	stllist.pop_back();
	free(e);
	e=NULL;
    }
}

void testadd2()
{
    int i;
    for(i=0;i<testpernum;i++){
	struct element*e=(struct element*)malloc(sizeof(*e));
	mylistadd(e);
    }
}
selflistelement*testget2()
{
    if(mylisttail==NULL)
	return NULL;
    mylistsize-=1;
    selflistelement*ele=mylisttail;
    if(mylisttail->prev==NULL){
	mylisttail=mylisthead=NULL;
    }else{
	mylisttail=(selflistelement*)mylisttail->prev;
	mylisttail->next=NULL;
    }
    return ele;
}

void testdel2()
{
    do{
	selflistelement*ele=mylisttail;
	if(ele==NULL)
	    break;
	ele=testget2();
	free(ele->p);
	free(ele);
	ele=NULL;
    }while(1);
}

int main(int argc,char**argv)
{
    if(argc<2){
	printusage(argc,argv);
	return 0;
    }

    int type=atoi(argv[1]);
    if(type!=1&&type!=2){
	printusage(argc,argv);
	return 0;
    }
    if(argc>2)
	testpernum=atoi(argv[2]);

    printf("every 3 secons add/del elements num is %d\n",testpernum);

    struct timeval time1,time2;
    gettimeofday(&time1,NULL);
    unsigned long interval;

    while(1){
	gettimeofday(&time1,NULL);
	if(type==1){
	    testadd1();
	    printf("stllist has %d element\n",stllist.size());
	}else{
	    testadd2();
	    printf("mylist has :%d elements\n",mylistsize);
	}
	gettimeofday(&time2,NULL);
	interval=1000000*(time2.tv_sec-time1.tv_sec)+
	    time2.tv_usec-time1.tv_usec;
	if(interval<3000000){
	    printf("after add sleep %d usec\n",3000000-interval);
	    usleep(3000000-interval);
	}else{
	    printf("add cost time too much %d \n",interval);
	}
	///////////////////////////////////////////////////
	gettimeofday(&time1,NULL);
	if(type==1){
	    testdel1();
	    printf("stllist has:%d elements\n",stllist.size());
	}else{
	    testdel2();
	    printf("mylist has :%d elements\n",mylistsize);
	}
	gettimeofday(&time2,NULL);
	interval=1000000*(time2.tv_sec-time1.tv_sec)+
	    time2.tv_usec-time1.tv_usec;
	if(interval<3000000){
	    printf("after del sleep %d usec\n",3000000-interval);
	    usleep(3000000-interval);
	}else{
	    printf("del cost time too much %d \n",interval);
	}

    }
}





