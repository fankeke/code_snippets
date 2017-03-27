#ifndef _ANET_H_
#define _ANET_H_


#define ANET_OK		0
#define ANET_ERR 	-1
#define ANET_ERR_LEN	256



int anetTcpServer(char*err,int port,char*bindaddr,int backlog);


#endif
