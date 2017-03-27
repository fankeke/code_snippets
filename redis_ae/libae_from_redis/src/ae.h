#ifndef __AE_H__
#define __AE_H__




#define AE_OK		0
#define AE_ERR		-1

#define AE_NONE		0
#define AE_READABLE	1
#define AE_WRITABLE	2

#define AE_FILE_EVENTS	1
#define AE_TIME_EVENTS	2
#define AE_ALL_EVENTS (AE_FILE_EVENTS|AE_TIME_EVENTS)
#define AE_DONT_WAIT	4

#define AE_NOMORE	-1

struct aeEventLoop;

typedef void aeFileProc(struct aeEventLoop*eventLoop,int fd,void*clientData,int mask);


typedef struct aeFileEvent{
    int	mask;
    aeFileProc*rfileProc;
    aeFileProc*wfileProc;
    void*clientData;
}aeFileEvent;
typedef struct aeFiredEvent{
    int fd;
    int mask;
}aeFiredEvent;

typedef struct aeEventLoop{
    int maxfd;
    int setsize;

    aeFileEvent		*events;
    aeFiredEvent	*fired;

    int		stop;
    void	*apidata;

    /*TODO aeBeforeSleepProc*beforesleep*/
}aeEventLoop;


aeEventLoop*aeCreateEventLoop(int setsize);

#endif
