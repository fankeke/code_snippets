#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

#define oops(msg){perror(msg);exit(0);}


void printusage()
{
    printf("usage:client,arg1,arg2\n");
}

void client_handler(int fd,int arg1,int arg2)
{
    int orig=arg1+arg2;
    arg1=htons(arg1);
    arg2=htons(arg2);
    send(fd,&arg1,sizeof(int),0);
    send(fd,&arg2,sizeof(int),0);
    int res;
    recv(fd,&res,sizeof(int),0);
    res=ntohs(res);
    if(orig!=res){
	fprintf(stderr,"wrong ans,should %d,but %d\n",orig,res);
    }else
	printf("ans:%d\n",res);
}
int main(int argc,char**argv)
{
    if(argc<3){
	printusage();
	oops("usage");
    }
    int arg1=atoi(argv[1]),arg2=atoi(argv[2]);
    int clientfd;
    struct sockaddr_in server;
    server.sin_family=AF_INET;
    server.sin_port=htons(8000);
    server.sin_addr.s_addr=inet_addr("192.168.1.104");

    if((clientfd=socket(AF_INET,SOCK_STREAM,0))<0){
	oops("socket");
    }
    if((connect(clientfd,(struct sockaddr*)&server,sizeof(server)))<0){
	oops("connect");
    }
    client_handler(clientfd,arg1,arg2);
    close(clientfd);
}
