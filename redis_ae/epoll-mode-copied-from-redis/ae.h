#ifndef _AE_H_INCLUDED_H_
#define _AE_H_INCLUDED_H_

#include "version.h"

#define AE_OK		0
#define AE_ERR		-1

#define AE_NONE		0
#define AE_READABLE	1
#define AE_WRITEABLE	2

#define AE_FILE_EVENTS	1
#define AE_TIME_EVENTS	2
#define AE_ALL_EVENTS (AE_FILE_EVENTS|AE_TIME_EVENTS)
#define AE_DONT_WAIT	4

#define AE_NOMORE	-1



typedef struct aeEventLoop_s aeEventLoop;
typedef struct aeFileEvent_s aeFileEvent;
typedef struct aeFiredEvent_s aeFiredEvent;
typedef struct aeTimeEvent_s aeTimeEvent;

typedef void (aeFileProc)(aeEventLoop*eventLoop,int fd,void*clientData,int mask);
typedef void (aeBeforeSleepProc)(aeEventLoop*eventLoop);
typedef int (aeTimeProc)(aeEventLoop*eventLoop,long long id,void*clientData);
typedef void (aeEventFinalizerProc)(aeEventLoop*eventLoop,void*clientData);

struct aeFileEvent_s {
    int		 mask;
    aeFileProc	*rfileProc;
    aeFileProc	*wfileProc;
    void	*clientData;
};
struct aeTimeEvent_s{
    long long 			id;
    long 			when_sec;
    long 			when_ms;
    aeTimeProc			*timeProc;
    aeEventFinalizerProc	*finalizerProc;
    void			*clientData;
    aeTimeEvent			*next;
};


struct aeFiredEvent_s{
    int		 fd;
    int		 mask;
};

struct aeEventLoop_s {
    int 	 	maxfd;
    int		 	setsize;
    long long 	 	timeEventNextId;
    time_t	 	lastTime;
    aeFileEvent		*events;
    aeFiredEvent 	*fired;
    aeTimeEvent		*timeEventHead;
    int			stop;
    void		*apidata;
    aeBeforeSleepProc	*beforesleep;
};


aeEventLoop*aeCreateEventLoop(int setsize);
void aeDeleteEventLoop(aeEventLoop*eventLoop);

long long aeCreateTimeEvent(aeEventLoop*eventLoop,long long milliseconds,
	aeTimeProc*proc,void*clientData,aeEventFinalizerProc*finalizerProc);

int aeDeleteTimeEvent(aeEventLoop*eventLoop,long long id);

int aeCreateFileEvent(aeEventLoop*eventLoop,int fd,int mask,aeFileProc*proc,void*clientData);
void aeDeleteFileEvent(aeEventLoop*eventLoop,int fd,int mask);



void aeSetBeforeSleepProc(aeEventLoop*eventLoop,aeBeforeSleepProc*beforesleep);

void aeMain(aeEventLoop*eventLoop);

#endif













