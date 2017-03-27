#include "anet.h"


#define ANET_CONNECT_NONE	0
#define ANET_CONNECT_NONBLOCK	1

static void anetSetError(char*err,const char*fmt,...)
{
    va_list	ap;
    if(!err) return;
    va_start(ap,fmt);
    vsnprintf(err,ANET_ERR_LEN,fmt,ap);
    va_end(ap);
}

static int anetTcpGenericConnect(char*err,char*ip,int port,int flags)
{
    int s,on=1;
    struct sockaddr_in sa;
    
    if((s=socket(AF_INET,SOCK_STREAM,0))==-1){
	anetSetError(err,"create socket:%s\n",strerror(errno));
	return ANET_ERR;
    }
    setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

    sa.sin_family=AF_INET;
    sa.sin_port=htons(port);
    if(inet_aton(ip,&sa.sin_addr)==0){
	struct hostent*he;
	he=gethostbyname(ip);
	if(he==NULL){
	    anetSetError(err,"Can't resolver:%s\n",ip);
	    close(s);
	    return ANET_ERR;
	}
	memcpy(&sa.sin_addr,he->h_addr,sizeof(struct in_addr));
    }
    if(flags&ANET_CONNECT_NONBLOCK){
	if(anetNonBlock(err,s)!=ANET_OK)
	    return ANET_ERR;
    }

    if(connect(s,(struct sockaddr*)&sa,sizeof(sa))==-1){
	if(errno==EINPROGRESS&&flags&ANET_CONNECT_NONBLOCK)
	    return s;//让它继续完成connect
	anetSetError(err,"Connect:%s\n",strerror(errno));
	close(s);
	return ANET_ERR;
    }

    return s;
}

int anetTcpConnect(char*err,char*ip,int port)
{
    return anetTcpGenericConnect(err,ip,port,ANET_CONNECT_NONE);
}

int anetSetBlock(char*err,int fd,int flag)
{
    int flags;

    if((flags=fcntl(fd,F_GETFL))==-1){
	anetSetError(err,"fcntl:%s\n",strerror(errno));
	return ANET_ERR;
    }
    if(flag){
	flags|=O_NONBLOCK;
    }else{
	flags|=~O_NONBLOCK;
    }

    if((fcntl(fd,F_SETFL,flags))==-1){
	anetSetError(err,"fcntl:%s\n",strerror(errno));
	return ANET_ERR;
    }
    return ANET_OK;
}

int anetSetTcpNoDelay(char*err,int fd,int flag)
{
    if(setsockopt(fd,IPPROTO_TCP,TCP_NODELAY,&flag,sizeof(flag))==-1){
	anetSetError(err,"setsockopt TCP_NODELAY:%s\n",strerror(errno));
	return ANET_ERR;
    }
    return ANET_OK;
}

int anetBlock(char*err,int fd)
{
    return anetSetBlock(err,fd,0);
}
int anetNonBlock(char*err,int fd)
{
    return anetSetBlock(err,fd,1);
}

int anetEnableTcpNoDelay(char*err,int fd)
{
    return anetSetTcpNoDelay(err,fd,0);
}
int anetDisableTcpNoDelay(char*err,int fd)
{
    return anetSetTcpNoDelay(err,fd,1);
}

//下面的读和写都是阻塞的操作，因为只是作用在client上面，
//server的R W在cmd->proc中具体实现

int anetWrite(int fd,void*buf,int count)
{
    int nwritten,totlen=0;
    while(totlen!=count){
	nwritten=write(fd,buf,count-totlen);
	if(nwritten==0) break;
	if(nwritten==-1) return -1;
	totlen+=nwritten;
	buf+=nwritten;
    }
    return totlen;
}

int anetRead(int fd,void*buf,int count)
{
    int nread,totlen=0;
    while(totlen!=count){
	nread=read(fd,buf,count-totlen);
	if(nread==0) break;
	if(nread==-1) return -1;
	totlen+=nread;
	buf+=nread;
    }
    return totlen;
}





