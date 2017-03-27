#include "anet.h"
#include "version.h"

static void anetSetError(char*err,const char*fmt,...)
{
    va_list		ap;
    if(!err) return;
    va_start(ap,fmt);
    vsnprintf(err,ANET_ERR_LEN,fmt,ap);
    va_end(ap);
}
static int anetSetReuseAddr(char*err,int fd)
{
    int yes=1;
    if(setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes))==-1){
	anetSetError(err,"setsockopt SO_REUSEADDR:%s\n",strerror(errno));
	return ANET_ERR;
    }
    return ANET_OK;
}
static int anetSetBlock(char*err,int fd,int non_block)
{
    int flags;

    if((flags=fcntl(fd,F_GETFL))==-1){
	anetSetError(err,"fcntl(F_GETFL):%s",strerror(errno));
	return ANET_ERR;
    }
    if(non_block)
	flags|=O_NONBLOCK;
    else
	flags|=~O_NONBLOCK;
    if((flags=fcntl(fd,F_SETFL))==-1){
	anetSetError(err,"fcntl(F_SETL,O_NONBLOCK):%s",strerror(errno));
	return ANET_ERR;
    }
    return ANET_OK;
}

static int anetSetTcpNoDelay(char*err,int fd,int val)
{
    if(setsockopt(fd,IPPROTO_TCP,TCP_NODELAY,&val,sizeof(val))==-1){
	anetSetError(err,"setsockopt tcp_nodelay :%s",strerror(errno));
	return ANET_ERR;
    }

    return ANET_OK;
}

static int anetListen(char*err,int s,struct sockaddr*sa,socklen_t len,int backlog)
{
    if(bind(s,sa,len)==-1){
	anetSetError(err,"bind:%s\n",strerror(errno));
	close(s);
	return ANET_ERR;
    }

    if(listen(s,backlog)==-1){
	anetSetError(err,"listen:%s\n",strerror(errno));
	close(s);
	return ANET_ERR;
    }
    return ANET_OK;
}

static int _anetTcpServer(char*err,int port,char*bindaddr,int af,int backlog)
{
    int			s,rv;
    char		_port[6];/*strlen("65535")*/
    struct addrinfo hints,*servinfo,*p;

    snprintf(_port,6,"%d",port);
    memset(&hints,0,sizeof(hints));
    hints.ai_family=af;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;

    if((rv=getaddrinfo(bindaddr,_port,&hints,&servinfo))!=0){
	anetSetError(err,"%s",gai_strerror(rv));
	return ANET_ERR;
    }

    for(p=servinfo;p!=NULL;p=p->ai_next){
	if((s=socket(p->ai_family,p->ai_socktype,p->ai_protocol))==-1){
	    continue;
	}
	if(anetSetReuseAddr(err,s)==ANET_ERR) 
	    goto error;
	if(anetListen(err,s,p->ai_addr,p->ai_addrlen,backlog)==ANET_ERR)
	    goto error;
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
}
static int anetGenericAccept(char*err,int s,struct sockaddr*sa,socklen_t*len)
{
    int				fd;
    while(1){
	fd=accept(s,sa,len);
	if(fd==-1){
	    if(errno==EINTR)
		continue;
	    else{
		anetSetError(err,"accept:%s\n",strerror(errno));
		return ANET_ERR;
	    }
	}
	break;
    }
    return fd;
}



int anetTcpServer(char*err,int port,char*bindaddr,int backlog)
{
    return _anetTcpServer(err,port,bindaddr,AF_INET,backlog);
}

int anetNonBlock(char*err,int fd)
{
    return anetSetBlock(err,fd,1);
}

int anetEnableTcpNoDelay(char*err,int fd)
{
    return anetSetTcpNoDelay(err,fd,0);
}

int anetTcpAccept(char*err,int s,char*ip,size_t ip_len,int*port)
{
    int					fd;
    struct sockaddr_storage		sa;
    socklen_t				salen=sizeof(sa);

    if((fd=anetGenericAccept(err,s,(struct sockaddr*)&sa,&salen))==-1)
	return ANET_ERR;

    if(sa.ss_family==AF_INET){
	struct sockaddr_in *s=(struct sockaddr_in*)&sa;
	if(ip) inet_ntop(AF_INET,(void*)&(s->sin_addr),ip,ip_len);
	if(port)*port=ntohs(s->sin_port);
    }else{
	;
    }
    return fd;

}











