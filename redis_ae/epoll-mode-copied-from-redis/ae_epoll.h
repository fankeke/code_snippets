#ifndef _AE_EPOLL_H_INCLUDED_H_
#define _AE_EPOLL_H_INCLUDED_H_

#include <sys/epoll.h>
#include "ae.h"

typedef struct  aeApiState_s aeApiState;

struct aeApiState_s {
    int epfd;
    struct epoll_event*events;
};

int aeApiCreate(aeEventLoop*eventLoop);
void aeApiFree(aeEventLoop*eventLoop);

int aeApiAddEvent(aeEventLoop*eventLoop,int fd,int mask);
void aeApiDelEvent(aeEventLoop*eventLoop,int fd,int delmask);

int aeApiPoll(aeEventLoop*eventLoop,struct timeval*tvp);

#endif
