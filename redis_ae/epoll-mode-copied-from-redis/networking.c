#include "networking.h"
#include "redis.h"
#include "anet.h"
#include "ae.h"
#include "version.h"

#define MAX_ACCEPTS_PER_CALL	1000

static void acceptCommonHandler(int fd,int flags)
{
    //so every connection represented as redisClient
    //the struct redisClient's role is same as nginx's ngx_connection_t
    redisClient			*c;
    if((c=createClient(fd))==NULL){
	fprintf(stderr,"Error registering fd event for the new client:%s(fd=%d)",
		strerror(errno),fd);
	close(fd);
	return;
    }

    if(listLength(server.clients)>server.maxclients){
	char *err="-ERR max numbe of clients reached\r\n";
	if(write(c->fd,err,strlen(err)==-1)){
	    /*nothing to do ,Just to avoid the warning ...*/
	}
	freeClient(c);
	return;
    }

    server.stat_numconnections++;
    c->flags|=flags;
}



void acceptTcpHandler(aeEventLoop*el,int fd,void*privdata,int mask)
{
    int			cport,cfd,max=MAX_ACCEPTS_PER_CALL;

    char		cip[REDIS_IP_STR_LEN];

    /*eveny call ,we create connection as much as possible (1000)*/
    while(max--){
	cfd=anetTcpAccept(server.neterr,fd,cip,sizeof(cip),&cport);
	if(cfd==ANET_ERR){ /* wrong or BLOCK  ,both should return*/
	    if(errno!=EWOULDBLOCK){
		fprintf(stderr,"Accepting client connection:%s\n",server.neterr);
	    }
	    return;
	}
	fprintf(stderr,"Accept:%s:%d",cip,cport);
	//so we should handle this connection .....
	acceptCommonHandler(cfd,0);
    }
}

redisClient* createClient(int fd)
{
    redisClient	*c=malloc(sizeof(*c));

    if(fd!=-1){
	anetNonBlock(NULL,fd);
	anetEnableTcpNoDelay(NULL,fd);
	if(server.tcpkeepalive){
	    ;
	    //anetKeepAlive(NULL,fd,server.tcpkeepalive);
	}

	//add this fd to epoll, and the file_event's privdata points to c
	//set the callback fun with readQueryFromClient
	if(aeCreateFileEvent(server.el,fd,AE_READABLE,
		    readQueryFromClient,c)==AE_ERR){
	    close(fd);
	    free(c);
	    return NULL;
	}
    }

    c->id=server.next_client_id++;
    c->fd=fd;
    c->flags=0;
    c->bufpos=0;

    // all clients is oganized in list patten
    if(fd!=-1)
	listAddNodeTail(server.clients,c);
    return c;
}

void freeClient(redisClient*c)
{
    listNode 	*ln;
    //remove from event
    if(c->fd!=-1){
	aeDeleteFileEvent(server.el,c->fd,AE_READABLE);
	aeDeleteFileEvent(server.el,c->fd,AE_WRITEABLE);
	close(c->fd);
    }

    //remove from list
    if(c->fd!=-1){
	ln=listSearchKey(server.clients,c);
	listDelNode(server.clients,ln);
    }

    free(c);
}

void readQueryFromClient(aeEventLoop*el,int fd,void*privdata,int mask)
{
    return;
}
