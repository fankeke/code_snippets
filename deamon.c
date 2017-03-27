/*
 * a simple code for deamon a process
 * more info couold refenrece to :
 *  http://www.cnblogs.com/mickole/p/3188321.html
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>


#define oops(msg) {perror(msg);exit(0);}

void create_deamon(void);

int main(void)
{
    time_t t;
    int fd;
    create_deamon();

    while(1){
	fd=open("deamon.log",O_WRONLY|O_CREAT|O_APPEND,0644);
	if(fd<0){
	    oops("open deamon.log");
	}
	t=time(0);
	char*buf=asctime(localtime(&t));
	write(fd,buf,strlen(buf));
	close(fd);
	sleep(5);
    }
}

void create_deamon(void)
{
    pid_t pid;
    pid=fork();
    if(pid<0) 
	oops("fork");
    if(pid>0)
	exit(0);

    //now only one process
    if(setsid()<0)
	oops("setsid");

    //chdir("/"); //whatever you want to change to ,or not
    int i;
    for(i=0;i<3;i++){
	close(i);
	/*
	fd=open("/dev/null",O_RDWR);
	fd=dup(0);
	fd=dup(0);
	*/
    }
    umask(0);
    return ;
}
    


