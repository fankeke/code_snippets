#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "util.h"
#include "policy.h"
#include "ae.h"
#include "anet.h"

#define CLIENT_CLOSE_AFTER_SENT 0x01
#define MAX_WRITE_PER_EVENT 1024*1024*1024

Policy *policy;
aeEventLoop*el;
static char anet_error[1024];

typedef struct proxy_client{
    int fd;
    int flags;
    struct proxy_client*remote;
    BufferList*blist;
    void(*OnError)(struct proxy_client*c);
    void(*OnRemoteDown)(struct proxy_client*c);

}proxy_client;

void ParseArgs(int argc,char**argv)
{
    int i,j;
    const char*logfile="stderr";
    //int loglevel=kInfo;
    int loglevel=kDebug;

    InitLogger(loglevel,NULL); //set log_file and log_level

    for(i=1;i<argc;i++){
	policy=ParsePolicy(argv[i]);
    }
    InitLogger(loglevel,logfile);
    if(policy==NULL){
	LogFatal("policy not valid");
    }
}

void SignalHandler(int signo)
{
    if(signo==SIGINT||signo==SIGTERM){
	el->stop=1;
    }
}

void RemoteDown(proxy_client*r)
{
    r->remote->OnRemoteDown(r->remote);
}

//bug:could merger into one fun
void FreeRemote(proxy_client*r)
{
    LogDebug("free remote");
    aeDeleteFileEvent(el,r->fd,AE_READABLE);
    aeDeleteFileEvent(el,r->fd,AE_WRITABLE);
    close(r->fd);
    FreeBufferList(r->blist);
    free(r);
}

void FreeClient(proxy_client*c)
{
    if(c==NULL) return;
    LogDebug("free client %d",c->fd);
    aeDeleteFileEvent(el,c->fd,AE_READABLE);
    aeDeleteFileEvent(el,c->fd,AE_WRITABLE);
    close(c->fd);
    FreeRemote(c->remote);
    FreeBufferList(c->blist);
    free(c);
}

void CloseAfterSent(proxy_client*c)
{
    int len;
    if(BufferListGetData(c->blist,&len)==NULL){
	FreeClient(c);
    }else{
	c->flags|=CLIENT_CLOSE_AFTER_SENT;
    }
}

void SendOutcome(aeEventLoop*el,int fd,void*privdata,int mask)
{
    LogDebug("sendoutcome");
    proxy_client*c=(proxy_client*)privdata;
    int len,nwritten=0,totwritten=0;
    char*buf;

    buf=BufferListGetData(c->blist,&len);
    if(buf==NULL){
	LogDebug("delete write event ,cause no data could sendout");
	aeDeleteFileEvent(el,fd,AE_WRITABLE);//why delete writeable event??
    }

    while(1){

	buf=BufferListGetData(c->blist,&len);
	if(buf==NULL){
	    //no data to send
	    if(c->flags&CLIENT_CLOSE_AFTER_SENT){
		FreeClient(c);
		return;
	    }
	    break;
	}
	nwritten=send(fd,buf,len,MSG_DONTWAIT);
	if(nwritten<=0) break;

	totwritten+=nwritten;
	LogDebug("Write and pop data %p %d",c->blist,nwritten);
	BufferListPop(c->blist,nwritten);
	if(totwritten>MAX_WRITE_PER_EVENT) break;
    }

    LogDebug("totwritten %d",totwritten);

    if(nwritten==-1){
	if(errno==EAGAIN){
	    nwritten=0;
	}else{
	    LogDebug("Write error %s",strerror(errno));
	    c->OnError(c);
	    return;
	}
    }
}

int SetWriteEvent(proxy_client*c)
{
    if(aeCreateFileEvent(el,c->fd,AE_WRITABLE,SendOutcome,c)==AE_ERR){
	LogError("Set write event failed");
	return -1;
    }
    return 0;
}

void ReadIncome(aeEventLoop*el,int fd,void*privdata,int mask)
{
    
    LogDebug("read in come");
    proxy_client*c=(proxy_client*)privdata;
    proxy_client*r=c->remote;
    //read things store in peer buffer

    char*buf;
    int len,nread=0;

    while(1){
	buf=BufferListGetSpace(r->blist,&len);
	if(buf==NULL) break;
	nread=recv(fd,buf,len,0);
	if(nread==-1){
	    if(errno==EAGAIN){
		nread=0;
	    }else{
		goto ERROR;
	    }
	}else if(nread==0){
	    LogInfo("connection close");
	    goto ERROR;
	}
	if(nread){
	    BufferListPush(r->blist,nread);
	    SetWriteEvent(r); //write to ...
	    LogDebug("set write");
	}else{
	    break;
	}
    }
    return;
ERROR:
    c->OnError(c);
}

proxy_client*AllocRemote(proxy_client*c)
{
    proxy_client*r=malloc(sizeof(proxy_client));
    r->flags=0;
    int fd=anetTcpNonBlockConnect(anet_error,policy->hosts[0].addr,policy->hosts[0].port);
    if(r==NULL||fd==-1) return NULL;
    LogDebug("connect remote fd:%d",fd);
    anetNonBlock(NULL,fd);
    anetTcpNoDelay(NULL,fd);
    r->fd=fd;
    r->remote=c;
    r->OnError=RemoteDown;
    r->blist=AllocBufferList(3);
    if(aeCreateFileEvent(el,r->fd,AE_READABLE,ReadIncome,r)==AE_ERR){
	//bug:free remote
	close(fd);
	return NULL;
    }
    LogDebug("new peer:%d---%d",r->fd,c->fd);
    return r;
}

proxy_client*AllocClient(int fd)
{
    proxy_client*c=malloc(sizeof(proxy_client));
    if(c==NULL) return NULL;
    c->flags=0;

    anetNonBlock(NULL,fd);
    anetTcpNoDelay(NULL,fd);

    c->fd=fd;
    c->blist=AllocBufferList(3);
    c->remote=AllocRemote(c);
    c->OnError=FreeClient;
    c->OnRemoteDown=CloseAfterSent;

    if(c->remote==NULL){
	//bug:c->blist should free
	close(fd);
	free(c);
	return NULL;
    }

    LogDebug("New client fd:%d remotefd:%d",c->fd,c->remote->fd);
    return c;
}

void Proxy_AcceptTcpHandler(aeEventLoop*el,int fd,void*privdata,int mask)
{
    int cport,cfd;
    char cip[128];
    cfd=anetTcpAccept(anet_error,fd,cip,&cport);
    if(cfd==AE_ERR){
	LogError("accept client connection failed:%s",anet_error);
	return;
    }
    LogInfo("Accept client from:%s:%d\n",cip,cport);

    proxy_client*c=AllocClient(cfd);

    if(c==NULL||aeCreateFileEvent(el,cfd,AE_READABLE,ReadIncome,c)==AE_ERR){
	LogError("Create event failed");
	FreeClient(c);
    }
}

int main(int argc,char**argv)
{
    int i,listen_fd;
    struct sigaction sig_action;

    ParseArgs(argc,argv);

    //daemonize();

    sig_action.sa_handler=SignalHandler;
    sig_action.sa_flags=SA_RESTART;
    sigemptyset(&sig_action.sa_mask);
    sigaction(SIGINT,&sig_action,NULL);
    sigaction(SIGTERM,&sig_action,NULL);
    sigaction(SIGPIPE,&sig_action,NULL);


    if((policy->listen.addr==NULL)||!strcmp(policy->listen.addr,"any")){
	free(policy->listen.addr);
	policy->listen.addr=strdup("0.0.0.0");
    }else if (!strcmp(policy->listen.addr,"localhost")){
	free(policy->listen.addr);
	policy->listen.addr=strdup("127.0.0.1");
    }

    listen_fd=anetTcpServer(anet_error,policy->listen.port,policy->listen.addr);
    el=aeCreateEventLoop(1024);

    if(listen_fd<0||aeCreateFileEvent(el,listen_fd,AE_READABLE,Proxy_AcceptTcpHandler,NULL)==AE_ERR){
	LogFatal("listen failed:%s",strerror(errno));
    }

    LogInfo("listening on:%s:%d",(policy->listen.addr?policy->listen.addr:"any"),policy->listen.port);
    for(i=0;i<policy->nhost;i++){
	LogInfo("proxy to %s:%d",policy->hosts[i].addr,policy->hosts[i].port);
    }

    aeMain(el);
    aeDeleteEventLoop(el);
    FreePolicy(policy);
    return 0;
}




