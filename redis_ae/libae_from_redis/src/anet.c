#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

#include "anet.h"


static void anetSetError(char*err,const char*fmt,...)
{
    va_list ap;
    if(!err) return;
    va_start(ap,fmt);
    vsnprintf(err,ANET_ERR_LEN,fmt,ap);
    va_end(ap);
}

static int anetSetReuseAddr(char*err,int fd)
{
    int yes=1;
    if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes))==-1){
	anetSetError(err,"setsockopt SO_REUSEADDR:%s",strerror(errno));
	return ANET_ERR;
    }
    return ANET_OK;
}


static int anetListen(char*err,int s,struct sockaddr*sa,socklen_t len,int backlog)
{
    if(bind(s,sa,len)==-1){
	anetSetError(err,"bind:%s",strerror(errno));
	close(s);
	return ANET_ERR;
    }
    if(listen(s,backlog)==-1){
	anetSetError(err,"listen:%s",strerror(errno));
	close(s);
	return ANET_ERR;
    }
    return ANET_OK;
}

static int _anetTcpServer(char*err,int port,char*bindaddr,int af,int backlog)
{
    int s,rv;
    if((s=socket(AF_INET,SOCK_STREAM,0))<0){
	anetSetError(err,"socket:%s\n",strerror(errno));
	return ANET_ERR;
    }

    if((anetSetReuseAddr(err,s))<0){
	return ANET_ERR;
    }
    struct sockaddr_in  server_addr;
    socklen_t 		server_len;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(port);
    server_addr.sin_addr.s_addr=inet_addr(bindaddr);
    server_len=sizeof(server_addr);
    if((anetListen(err,s,(struct sockaddr*)&server_addr,server_len,backlog))<0){
	return ANET_ERR;
    }
    return s;

    /*
    char _port[6];
    struct addrinfo hints,*servinfo,*p;

    snprintf(_port,6,"%d",port);
    memset(&hints,0,sizeof(hints));
    hints.ai_family=af;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;

    if((rv=getaddrinfo(bindaddr,_port,&hints,&servinfo))!=0){
	anetSetError(err,"%s",strerror(rv));
	return ANET_ERR;
    }

    for (p=servinfo;p;p=p->ai_next) {
	if((s=socket(p->ai_family,p->ai_socktype,p->ai_protocol))==-1)
	    continue;
	if(anetSetReuseAddr(err,s)==ANET_ERR) goto error;
	if(anetListen(err,s,p->ai_addr,p->ai_addrlen,backlog)==ANET_ERR) goto error;
	goto end;
    }
    if(p==NULL){
	anetSetError(err,"unable to bind socket");
	goto error;
    }
error:
    s=ANET_ERR;
end:
    freeaddrinfo(servinfo);
    return s;
    */

}

int anetTcpServer(char*err,int port,char*bindaddr,int backlog)
{
    return _anetTcpServer(err,port,bindaddr,AF_INET,backlog);
}
