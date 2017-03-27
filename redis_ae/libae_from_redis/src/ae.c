#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "ae.h"
#include "ae_epoll.c"



aeEventLoop*aeCreateEventLoop(int setsize)
{
    aeEventLoop*eventLoop;
    int i;

    if((eventLoop=malloc(sizeof*eventLoop))==NULL) goto err;
    eventLoop->events=malloc(sizeof(aeFileEvent)*setsize);
    eventLoop->fired=malloc(sizeof(aeFiredEvent)*setsize);
    if(eventLoop->events==NULL||eventLoop->fired==NULL) goto err;
    eventLoop->setsize=setsize;
    eventLoop->stop=0;
    eventLoop->maxfd=-1;

    if(aeApiCreate(eventLoop)<0) goto err;

    for(i=0;i<setsize;i++){
	eventLoop->events[i].mask=AE_NONE;
    }
    return eventLoop;

err:
    if(eventLoop){
	free(eventLoop->events);
	free(eventLoop->fired);
	free(eventLoop);
    }
    return NULL;
}

static int aeProcessEvents(aeEventLoop*eventLoop,int flags)
{
}


int aeCreateFileEvent(aeEventLoop*eventLoop,int fd,int mask,
	aeFileProc*proc,void*clientData)
{
}
void aeMain(aeEventLoop*eventLoop)
{
    eventLoop->stop=0;
    while(!eventLoop->stop){
	aeProcessEvents(eventLoop,AE_ALL_EVENTS);
    }
}
