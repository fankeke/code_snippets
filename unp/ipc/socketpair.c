#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#define P_DATA	"Hi,I am parent"
#define C_DATA	"Hi,I am child"
void child(int fd);
void parent(int fd);


int main(int argc,char**argv)
{
    int			fd[2];
    pid_t		pid;
    socketpair(AF_UNIX,SOCK_STREAM,0,fd);
//    pipe(fd);      //实验了下，发现我系统上面的pipe不是全双共的。。

    pid=fork();

    if(pid==0){
	close(fd[0]);
	child(fd[1]);
	close(fd[1]);
    }else{
	close(fd[1]);
	parent(fd[0]);
	close(fd[0]);
    }
}
void child(int fd)
{
    char		buff[1024];
    write(fd,C_DATA,sizeof(C_DATA));
    read(fd,buff,sizeof(buff));
    printf("child read :%s\n",buff);
}
void parent(int fd)
{
    char		buff[1024];
    read(fd,buff,sizeof(buff));
    printf("parent read;%s\n",buff);
    write(fd,P_DATA,sizeof(P_DATA));
}

