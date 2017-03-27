#include "ae.h"
#include "ae_epoll.h"
#include "version.h"

static void aeGetTime(long *sec,long*ms)
{
    struct timeval	tv;
    gettimeofday(&tv,NULL);
    *sec=tv.tv_sec;
    *ms=tv.tv_usec/1000;
}
static void  aeAddMillisecondsToNow(long long milliseconds,long*sec,long*ms)
{
    long		cur_sec,cur_ms,when_sec,when_ms;

    aeGetTime(&cur_sec,&cur_ms);
    when_sec=cur_sec+milliseconds/1000;
    when_ms=cur_ms+milliseconds%1000;

    if(when_ms>=1000){
	when_sec++;
	when_ms-=1000;
    }
    *sec=when_sec;
    *ms=when_ms;
}

static aeTimeEvent*aeSearchNearestTimer(aeEventLoop*eventLoop)
{
    aeTimeEvent *te=eventLoop->timeEventHead;
    aeTimeEvent *nearest=NULL;

    while(te){
	if(!nearest||te->when_sec<nearest->when_sec||
		(te->when_sec==nearest->when_sec&&
		 te->when_ms<nearest->when_ms))
	    nearest=te;
	te=te->next;
    }
    return nearest;
}

static int processTimeEvents(aeEventLoop*eventLoop)
{
    int 	processed=0;
    aeTimeEvent *te;
    long long maxId;
    time_t	now=time(NULL);

//    if(now<eventLoop->lastTime)

    eventLoop->lastTime=now;

    te=eventLoop->timeEventHead;
    maxId=eventLoop->timeEventNextId-1;

    while(te){
	long now_sec,now_ms;
	long long id;

	if(te->id>maxId){
	    te=te->next;
	    continue;
	}

	aeGetTime(&now_sec,&now_ms);
	if(now_sec>te->when_sec||
		(now_sec==te->when_sec&&now_ms>=te->when_ms))
	{
	    int retval;

	    id=te->id;
	    retval=te->timeProc(eventLoop,id,te->clientData);
	    processed++;

	    //the retval is timeevent interval time
	    if(retval!=AE_NOMORE){
		aeAddMillisecondsToNow(retval,&te->when_sec,&te->when_ms);
	    }else{
		aeDeleteTimeEvent(eventLoop,id);
	    }

	    te=eventLoop->timeEventHead;

	}else{
	    te=te->next;
	}
    }
    return processed;
}




aeEventLoop*aeCreateEventLoop(int setsize)
{
    aeEventLoop		*eventLoop;
    int			 i;

    if((eventLoop=malloc(sizeof(*eventLoop)))==NULL)
	goto err;
    eventLoop->events=malloc(sizeof(aeFileEvent)*setsize);
    eventLoop->fired=malloc(sizeof(aeFiredEvent)*setsize);
    if(eventLoop->events==NULL||eventLoop->fired==NULL)
	goto err;
    eventLoop->setsize=setsize;
    eventLoop->lastTime=time(NULL);
    eventLoop->timeEventHead=NULL;
    eventLoop->timeEventNextId=0;
    eventLoop->stop=0;
    eventLoop->maxfd=-1;
    eventLoop->beforesleep=NULL;

    if(aeApiCreate(eventLoop)==-1)
	goto err;

    for(i=0;i<setsize;i++)
	eventLoop->events[i].mask=AE_NONE;

    return eventLoop;
    
err:
    if(eventLoop){
	free(eventLoop->events);
	free(eventLoop->fired);
	free(eventLoop);
    }
    return NULL;
}

void aeDeleteEventLoop(aeEventLoop*eventLoop)
{
    aeApiFree(eventLoop);
    free(eventLoop->events);
    free(eventLoop->fired);
    free(eventLoop);
}











long long aeCreateTimeEvent(aeEventLoop*eventLoop,long long milliseconds,
	aeTimeProc*proc,void*clientData,aeEventFinalizerProc*finalizerProc)
{
    long long id=eventLoop->timeEventNextId++;
    aeTimeEvent	*te;

    te=malloc(sizeof(*te));
    if(te==NULL)
	return AE_ERR;
    te->id=id;

    //set te->when_sec,te->when_ms
    //te->when_sec and when_ms is time when this time event should be handler
    aeAddMillisecondsToNow(milliseconds,&te->when_sec,&te->when_ms);

    te->timeProc=proc;
    te->finalizerProc=finalizerProc;
    te->clientData=clientData;
    te->next=eventLoop->timeEventHead;
    eventLoop->timeEventHead=te;
    return id;
}

int aeDeleteTimeEvent(aeEventLoop*eventLoop,long long id)
{
    aeTimeEvent	*te,*prev=NULL;
    te=eventLoop->timeEventHead;
    while(te){
	if(te->id==id){
	    if(prev==NULL){
		eventLoop->timeEventHead=te->next;
	    }else{
		prev->next=te->next;
	    }
	    if(te->finalizerProc){
		te->finalizerProc(eventLoop,te->clientData);
	    }
	    free(te);
	    return AE_OK;
	}
	prev=te;
	te=te->next;
    }
    return AE_ERR;
}



//we don't need to malloc aeFileEvent,because already exits in eventLoop->events[]
int aeCreateFileEvent(aeEventLoop*eventLoop,int fd,int mask,aeFileProc*proc,void*clientData)
{
    if(fd>=eventLoop->setsize){
	errno=ERANGE;
	return AE_ERR;
    }

    aeFileEvent*fe=&eventLoop->events[fd];

    if(aeApiAddEvent(eventLoop,fd,mask)==-1){
	return AE_ERR;
    }

    fe->mask|=mask;
    if(mask&AE_READABLE) fe->rfileProc=proc;
    if(mask&AE_WRITEABLE) fe->wfileProc=proc;

    fe->clientData=clientData;

    if(fd>eventLoop->maxfd)
	eventLoop->maxfd=fd;
    return AE_OK;
}

void aeDeleteFileEvent(aeEventLoop*eventLoop,int fd,int mask)
{
    if(fd>=eventLoop->setsize) return;
    aeFileEvent *fe=&eventLoop->events[fd];

    if(fe->mask==AE_NONE) return;

    aeApiDelEvent(eventLoop,fd,mask);
    fe->mask=fe->mask&(~mask);

    //in eventLoop->events[],fd indict the index

    if(fd==eventLoop->maxfd&&fe->mask==AE_NONE){
	int j;
	for(j=eventLoop->maxfd-1;j>=0;j--){
	    if(eventLoop->events[j].mask!=AE_NONE) break;
	}
	eventLoop->maxfd=j;
    }
}


void aeSetBeforeSleepProc(aeEventLoop*eventLoop,aeBeforeSleepProc*beforesleep)
{
    eventLoop->beforesleep=beforesleep;
}


int aeProcessEvents(aeEventLoop*eventLoop,int flags)
{
    int processed=0,numevents;

    if(!(flags&AE_TIME_EVENTS)&&!(flags&AE_FILE_EVENTS))
	return 0;
    
    if(eventLoop->maxfd!=-1||
	    ((flags&AE_TIME_EVENTS)&&!(flags&AE_DONT_WAIT)))
    {
	int		j;
	aeTimeEvent	*shortest=NULL;
	struct timeval	tv,*tvp;

	if(flags&AE_TIME_EVENTS&&(!(flags&AE_DONT_WAIT)));
	    shortest=aeSearchNearestTimer(eventLoop);
	if(shortest){
	    long 	now_sec,now_ms;
	    aeGetTime(&now_sec,&now_ms);
	    tvp=&tv;
	    tvp->tv_sec=shortest->when_sec-now_sec;
	    if(shortest->when_ms<now_ms){
		tvp->tv_usec=((shortest->when_ms+1000)-now_ms)*1000;
		tvp->tv_sec--;
	    }else{
		tvp->tv_usec=(shortest->when_ms-now_ms)*1000;
	    }

	    if(tvp->tv_sec<0) tvp->tv_sec=0;
	    if(tvp->tv_usec<0) tvp->tv_usec=0;
	}else{
	    //have no NearestTimeEvent;
	    if(flags&AE_DONT_WAIT){
		tv.tv_sec=tv.tv_usec=0;
		tvp=&tv;
	    }else{
		tvp=NULL;/*wait forever*/
	    }
	}

	numevents=aeApiPoll(eventLoop,tvp);
	for(j=0;j<numevents;j++){
	    aeFileEvent *fe=&eventLoop->events[eventLoop->fired[j].fd];
	    int mask=eventLoop->fired[j].mask;
	    int fd=eventLoop->fired[j].fd;
	    int rfired=0;

	    //so we & three of them because 
	    //we want to be sure that the event type triigered(mask)  is 
	    //actually we added ( fe->mask)
	    //
	    //may be an alreday processed event removed an elemnet that fired and 
	    //we still didn't processed, so we check if the event is still valid
	    if(fe->mask&mask&AE_READABLE){
		rfired=1;
		fe->rfileProc(eventLoop,fd,fe->clientData,mask);
	    }
	    if(fe->mask&mask&AE_WRITEABLE){
		if(!rfired||fe->wfileProc!=fe->rfileProc)
		    fe->wfileProc(eventLoop,fd,fe->clientData,mask);
	    }
	    processed++;
	}
    }
    if(flags&AE_TIME_EVENTS)
	processed+=processTimeEvents(eventLoop);
    return processed;;
}


void aeMain(aeEventLoop*eventLoop)
{
    eventLoop->stop=0;
    while(!eventLoop->stop){
	if(eventLoop->beforesleep!=NULL)
	    eventLoop->beforesleep(eventLoop);
	aeProcessEvents(eventLoop,AE_ALL_EVENTS);
    }
}















