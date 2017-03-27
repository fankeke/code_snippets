#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define  MAXLINE	1024
void child(int fd);
int main(int argc,char**argv)
{
    int					listenfd,connfd;
    struct sockaddr_in			server_addr,client_addr;
    pid_t				pid;
    socklen_t				addrlen=sizeof(struct sockaddr);

    listenfd=socket(AF_INET,SOCK_STREAM,0);
    
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(8080);
    server_addr.sin_addr.s_addr=INADDR_ANY;

    if(bind(listenfd,(struct sockaddr*)&server_addr,addrlen)<0){
	perror("bind");
	exit(0);
    }

    listen(listenfd,5);

    for(;;){
	connfd=accept(listenfd,(struct sockaddr*)&client_addr,&addrlen);
	printf("peer info:%s:%d\n",inet_ntoa(client_addr.sin_addr),
		ntohs(client_addr.sin_port));
	pid=fork();
	if(pid==0){
	    close(listenfd);
	    child(connfd);
	    close(connfd);
	    exit(0);
	}
	close(connfd);
    }
    exit(0);
}

void child(int fd)
{
    ssize_t			n;
    char			buff[MAXLINE];
    int				filefd;

    n=read(fd,buff,sizeof(buff));
    if(n<0){
	perror("read");
	return;
    }
    buff[n]='\0';
    printf("filename:%s\n",buff);


    filefd=open(buff,O_RDONLY);
    if(filefd<0){
	snprintf(buff+n,sizeof(buff)-n,":can no open %s\n",strerror(errno));
	n=strlen(buff);
	write(fd,buff,n);
	perror("open");
	return;
    }else{

	while((n=read(filefd,buff,sizeof(buff)))>0){
	    write(fd,buff,n);
	    bzero(buff,sizeof(buff));
	}
    }
    close(filefd);
    return;
}




    



