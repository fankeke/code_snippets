#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define oops(msg) {perror(msg);exit(0);}
int main(int argc,char**argv)
{
    int			fd,n;
    char		buffer[1024];

    for(;;){
	fd=open("hello.txt",O_RDONLY);
	if(fd<0){
	    oops("open");
	}
	if((n=read(fd,buffer,sizeof(buffer)))<0)
	    oops("read");
	buffer[n]=0;
	printf("buffer :%s\n",buffer);
	close(fd);
	bzero(buffer,1024);
	sleep(1);
    }
}
