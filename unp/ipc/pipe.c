#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAXLINE		1024
void server(int,int);
void client(int,int);

int main(int argc,char**argv)
{
    int			pipe1[2],pipe2[2];
    pid_t		pid;

    pipe(pipe1);
    pipe(pipe2);

    pid=fork();
    if(pid<0){
	perror("fork");
	exit(0);
    }
    if(pid==0){
	close(pipe1[0]);
	close(pipe2[1]);

	server(pipe2[0],pipe1[1]);

	exit(0);
    }
    close(pipe1[1]);
    close(pipe2[0]);
    client(pipe1[0],pipe2[1]);
    exit(0);
}

void client(int readfd,int writefd)
{
    char		buff[MAXLINE];
    size_t		len;
    ssize_t		n;

    printf("input filename:");
    fgets(buff,MAXLINE,stdin);
    len=strlen(buff);
    if(buff[len-1]=='\n'){
	len--;
    }

    write(writefd,buff,len);

    while((n=read(readfd,buff,MAXLINE))>0){
	write(STDOUT_FILENO,buff,n);
	bzero(buff,sizeof(buff));
    }
    
}
void server(int readfd,int writefd)
{
    int			fd;
    char		buff[MAXLINE];
    ssize_t		n;

    if((n=read(readfd,buff,MAXLINE))<0){
	perror("read");
	exit(0);
    }
    buff[n]='\0';
    fd=open(buff,O_RDONLY);
    if(fd<0){
	snprintf(buff+n,sizeof(buff)-n,":can no open:%s",strerror(errno));
	n=strlen(buff);
	write(writefd,buff,n);
    }else{
	while((n=read(fd,buff,MAXLINE))>0){
	    write(writefd,buff,n);
	}
	close(fd);
    }
}
    







