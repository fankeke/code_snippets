#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAXLINE		1024
void recv_fun(int);
int main(int argc,char**argv)
{
    int				connfd;
    struct sockaddr_in		server_addr;

    connfd=socket(AF_INET,SOCK_STREAM,0);

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(8080);
    server_addr.sin_addr.s_addr=inet_addr("192.168.1.102");

    
    if(connect(connfd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
	perror("connect");
	exit(0);
    }

    recv_fun(connfd);
    close(connfd);
}



void recv_fun(int fd)
{
    char			buff[MAXLINE];
    size_t			len;
    ssize_t			n;
    int				filefd;

    printf("input filename:");
    fgets(buff,MAXLINE,stdin);
    len=strlen(buff);
    if(buff[len-1]='\n'){
	buff[len-1]='\0';
	len--;
    }

    filefd=open(buff,O_RDWR|O_CREAT|O_APPEND,0664);
    write(fd,buff,len);

    while((n=read(fd,buff,sizeof(buff)))>0){
	write(filefd,buff,n);
	bzero(buff,sizeof(buff));
    }
}
