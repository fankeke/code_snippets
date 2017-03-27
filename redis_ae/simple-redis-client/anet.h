#ifndef _ANET_H_INCLUDE_H_
#define _ANET_H_INCLUDE_H_

#include "utils.h"

#define ANET_OK		0
#define ANET_ERR	-1

#define ANET_ERR_LEN	256


int anetTcpConnect(char*err,char*ip,int port);


int anetSetBlock(char*err,int fd,int flag);
int anetBlock(char*err,int fd);
int anetNonBlock(char*err,int fd);

int anetSetTcpNoDelay(char*err,int fd,int flag);
int anetEnableTcpNoDelay(char*err,int fd);
int anetDisableTcpNoDelay(char*err,int fd);


int anetWrite(int fd,void*buf,int count);
int anetRead(int fd,void*buf,int count);

#endif
