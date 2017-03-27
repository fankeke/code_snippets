#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"
#include "sds.h"
#include "anet.h"

#define REDIS_CMD_INLINE	1
#define REDIS_CMD_BULK		2
#define REDIS_CMD_RETCODEREPLY	8
#define REDIS_CMD_BULKREPLY	16

struct config{
    char*hostip;
    int  hostport;
}config;

typedef struct{
    char *name;
    int   arity;
    int	  flags;
}redisClientCommand;

static redisClientCommand CommandTable[]={
    {	"get",2,REDIS_CMD_INLINE|REDIS_CMD_BULKREPLY},
    {	"set",3,REDIS_CMD_BULK|REDIS_CMD_RETCODEREPLY},
    {	NULL,0,0}
};

static redisClientCommand*lookupCommand(char*name)
{
    redisClientCommand*rc=&CommandTable[0];
    while(rc->name!=NULL){
	if(strncasecmp(rc->name,name,strlen(name))==0) return rc;
	rc++;
    }
    return NULL;
}

static int clientConnect(void)
{
    char err[ANET_ERR_LEN];
    int fd;

    fd=anetTcpConnect(err,config.hostip,config.hostport);
    if(fd==ANET_ERR){
	fprintf(stderr,"connect:%s\n",err);
	return -1;
    }
    anetEnableTcpNoDelay(NULL,fd);
    return fd;
}
static sds cliReadLine(int fd)
{
    sds line=sdsempty();
    while(1){
	char c;
	ssize_t ret;
	ret=read(fd,&c,1);
	if(ret==-1){
	    sdsfree(line);
	    return NULL;
	}else if((ret==0)||(c=='\n')){
	    break;
	}else{
	    line=sdscatlen(line,&c,1);
	}
    }
    return sdstrim(line,"\r\n");
}

static int cliReadInlineReply(int fd,int type)
{
    sds reply=cliReadLine(fd);
    if(reply==NULL) return 1;
    printf("%s\n",reply);
    return 0;
}

static int cliReadBulkReply(int fd,int multibulk)
{
    sds replylen=cliReadLine(fd);
    char*reply,crlf[2];
    int bulklen,error=0;

    if(replylen==NULL) return 1;
    if(strcmp(replylen,"nil")==0){
	sdsfree(replylen);
	printf("(nil)\n");
	return 0;
    }

    bulklen=atoi(replylen);
    if(multibulk&&bulklen==-1){
	sdsfree(replylen);
	printf("(nil)\n");
	return 0;
    }

    reply=malloc(bulklen);
    anetRead(fd,reply,bulklen);
    anetRead(fd,crlf,2);

    if(bulklen&&fwrite(reply,bulklen,1,stdout)==0){
	free(reply);
	return 1;
    }
    printf("\n");
    return error;


}

static int cliSendCommand(int argc,char**argv)
{
    int fd,j,retval=0;
    sds cmd=sdsempty();
    
    redisClientCommand*rc=lookupCommand(argv[0]);
    if(!rc){
	fprintf(stderr,"unkonw Command:%s\n",argv[0]);
	return 1;
    }

    if(rc->arity>0&&rc->arity!=argc){
	fprintf(stderr,"Wrong name of arguments for %s\n",rc->name);
	return 1;
    }

    if((fd=clientConnect())==-1) return 1;


    for(j=0;j<argc;j++){
	if(j!=0) cmd=sdscat(cmd," ");
	if(j==argc-1&&rc->flags&REDIS_CMD_BULK){
	    cmd=sdscatprintf(cmd,"%d",sdslen(argv[j]));
	}else{
	    cmd=sdscatlen(cmd,argv[j],sdslen(argv[j]));
	}
    }

    cmd=sdscat(cmd,"\r\n");

    if(rc->flags&REDIS_CMD_BULK){
	cmd=sdscatlen(cmd,argv[argc-1],sdslen(argv[argc-1]));
	cmd=sdscat(cmd,"\r\n");
    }

    anetWrite(fd,cmd,sdslen(cmd));

    if(rc->flags&REDIS_CMD_RETCODEREPLY){
	retval=cliReadInlineReply(fd,REDIS_CMD_RETCODEREPLY);
    }else if(rc->flags&REDIS_CMD_BULKREPLY){
	retval=cliReadBulkReply(fd,0);
    }

    if(retval){
	close(fd);
	return retval;
    }
    close(fd);
    return 0;
}







int main(int argc,char**argv)
{
    char **argvcopy;
    int j;

    config.hostip="192.168.191.3";
    config.hostport=6379;

    argc-=1;
    argv+=1;


    if(argc<1){
	fprintf(stderr,"usage:set msg hello\n");
	exit(1);
    }

    argvcopy=(char**)malloc(sizeof(char*)*(argc+1));

    for(j=0;j<argc;j++)
	argvcopy[j]=sdsnew(argv[j]);

    return cliSendCommand(argc,argvcopy);
}


