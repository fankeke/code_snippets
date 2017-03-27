#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <errno.h>

#define oops(msg){perror(msg);exit(0);}




void client_handler(int fd)
{
    int arg1,arg2;

    recv(fd,&arg1,sizeof(int),0);
    recv(fd,&arg2,sizeof(int),0);
    
    arg1=ntohs(arg1);
    arg2=ntohs(arg2);

    int res=arg1+arg2;
    res=htons(res);
    send(fd,&res,sizeof(int),0);

    close(fd);
}
int main(int argc,char**argv)
{

    int listenfd;
    struct sockaddr_in server;
    server.sin_family=AF_INET;
    server.sin_port=htons(8000);
    server.sin_addr.s_addr=inet_addr("192.168.1.104");

    if((listenfd=socket(AF_INET,SOCK_STREAM,0))<0){
	oops("socket");
    }

    if((bind(listenfd,(struct sockaddr*)&server,sizeof(server)))<0){
	oops("bind");
    }

    if((listen(listenfd,5))<0){
	oops("listen");
    }
    struct sockaddr_in clientaddr;
    int clientfd;
    socklen_t clientlen;

    while(1){
	clientlen=sizeof(clientaddr);
	if((clientfd=accept(listenfd,(struct sockaddr*)&clientaddr,&clientlen))<0){
	    printf("accpet:%s\n",strerror(errno));
	    continue;
	}
	client_handler(clientfd);
    }
}





