#ifndef _NETWORKING_H_INCLUDED_H_
#define _NETWORKING_H_INCLUDED_H_

#include "ae.h"
#include "redis.h"


void acceptTcpHandler(aeEventLoop*el,int fd,void*privdata,int mask);

redisClient* createClient(int fd);
void	 freeClient(redisClient*c);


void readQueryFromClient(aeEventLoop*el,int fd,void*privdata,int mask);

#endif
