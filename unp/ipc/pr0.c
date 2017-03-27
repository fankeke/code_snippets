#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define oops(msg) {perror(msg);exit(0);}
int main(int argc,char**argv)
{
    int			fd;
    char		buffer[1024];

    for(;;){

	fd=open("hello.txt",O_RDWR|O_APPEND|O_CREAT,0664);
	//mode 一定前面加0。不能写成664.
	//另外，写入文件，需要在flags中加入O_APPEND,防止覆盖存在的内容。
	//还有，如果不加O_RDWR，那么写进去。
	//最后，如果出现O_CREAT，一定要写后面的mode。如果没有O_CREAT,则
	//mode可以忽略
	if(fd<0){
	    oops("open");
	}
	snprintf(buffer,1024,"%s","hello,world");
	if((write(fd,buffer,strlen(buffer)))<0)
	    oops("write");
	close(fd);
	bzero(buffer,1024);
	sleep(1);
    }
}
