#include "redis.h"
#include "adlist.h"
#include "version.h"
#include "networking.h"

struct redisServer	server;

static void sigShutDownHandler(int sig)
{
    char	*msg;
    switch(sig){
	case SIGINT:
	    msg="Received SIGINT signal....";
	    break;
	case SIGTERM:
	    msg="Received SIGTERM signal....";
	    break;
	default:
	    msg="Received shutdown signal....";
	    break;
    };

    fprintf(stderr,"%s",msg);
    exit(0);
}

static void setupSignalHandlers()
{
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    act.sa_handler=sigShutDownHandler;
    sigaction(SIGTERM,&act,NULL);
    sigaction(SIGINT,&act,NULL);
    return;
}

void displayServer()
{
    printf("pid is %d\n",server.pid);
    printf("port is %d\n",server.port);
}

int listenToPort(int port,int*fds,int*count)
{
    int		j;
    if(server.bindaddr_count==0) server.bindaddr[0]=NULL;
    for(j=0;j<server.bindaddr_count||j==0;j++){
	if(server.bindaddr[j]==NULL){
	    fds[*count]=anetTcpServer(server.neterr,port,NULL,server.tcp_backlog);
	    if(fds[*count]!=ANET_ERR){
		anetNonBlock(NULL,fds[*count]);
		(*count)++;
	    }
	    if(*count) break;
	}

	/*if(fds[*count]==ANET_ERR){
	    redisLog(REDIS_WARNING,
		    "create serer tcp listening socket %s:%d:%s",
		    server.baindaddr[j]?server.binaddr[j]:"*",
		    port,server.neterr);
	    return REDIS_ERR;
	}
	anetNonBlock(NULL,fds[*count]);
	(*count)++;
	*/
    }
    return REDIS_OK;
}

int serverCron(aeEventLoop*eventLoop,long long id,void*clientData)
{
    return 100;
    return REDIS_OK;
}
void beforeSleep(aeEventLoop*loop)
{
}

void initServerConfig(void)
{
    server.maxclients=REDIS_MAX_CLIENTS;
    server.port=REDIS_SERVERPORT;
    server.tcp_backlog=REDIS_TCP_BACKLOG;
    server.bindaddr_count=0;
    server.ipfd_count=0;
    server.tcpkeepalive=0;
    server.next_client_id=0;
    server.stat_numconnections=0;

}

void initServer(void)
{
    int j;
    signal(SIGHUP,SIG_IGN);
    signal(SIGPIPE,SIG_IGN);
    setupSignalHandlers();

    server.pid=getpid();
    server.clients=listCreate();

    server.el=aeCreateEventLoop(server.maxclients);

    if(server.port!=0&&
	    listenToPort(server.port,server.ipfd,&server.ipfd_count)==REDIS_ERR)
	exit(1);

    if(server.ipfd_count==0){
	//redisLog(REDIS_WARNING,"Configured to not listen anywhere,exiting..");
	fprintf(stderr,"%s","Configured to not listen anywhere ,exiting...");
	exit(1);
    }

    //1 is milliseconds,serverCron is aeTimeProc NULL is ClientData,NULL is finlizerproc
    if(aeCreateTimeEvent(server.el,1,serverCron,NULL,NULL)==AE_ERR){
	fprintf(stderr,"%s","Can't creat te serverCron time event,exiting...");
	exit(1);
    }

    for(j=0;j<server.ipfd_count;j++){
	if(aeCreateFileEvent(server.el,server.ipfd[j],AE_READABLE,
		    acceptTcpHandler,NULL)==AE_ERR){
	    fprintf(stderr,"%s","tcp handler register failed\n");
	    exit(1);
	}
    }

}


int main(int argc,char**argv)
{
    initServerConfig();
    initServer();
    aeSetBeforeSleepProc(server.el,beforeSleep);
    aeMain(server.el);
    aeDeleteEventLoop(server.el);

    return 0;


    displayServer();
    for(;;){
    }
    exit(0);
}



