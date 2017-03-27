#ifndef _ANET_H_INCLUDED_H_
#define _ANET_H_INCLUDED_H_

#define ANET_OK		0
#define ANET_ERR	-1
#define	ANET_ERR_LEN	256

#include "version.h"


int anetTcpServer(char*err,int port,char*bindaddr,int backlog);
int anetNonBlock(char*err,int fd);
int anetTcpAccept(char*err,int s,char*ip,size_t ip_len,int*port);
int anetEnableTcpNoDelay(char*err,int fd);

#endif
