# simpe-program
some simple program for my own use
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

#define	BUF_SIZ		4096

int main(int ac,char**av)
{
    ssize_t			n_read,n_write;
    int				fd_in,fd_out;
    char			*buf;

    //read write的返回值都是ssize_t 不是size_t 
    if(ac<3){
	printf("usage:orig,dst\n");
	exit(0);
    }
    buf=malloc(BUF_SIZ+1);//以后都按这种+1的方式申请内存，以免堆栈溢出
    if(buf==NULL){
	exit(0);
    }
    fd_in=open(av[1],O_RDONLY);
    if(fd_in<0){
	perror("open in");
	exit(0);
    }
    fd_out=open(av[2],O_RDWR|O_CREAT,S_IRUSR|S_IWUSR|S_IXUSR);
    if(fd_out<0){
	perror("open out");
	exit(0);
    }
    while((n_read=read(fd_in,buf,BUF_SIZ))>0){
	n_write=write(fd_out,buf,n_read);
	//write返回值如果和其第三个参数不相等，那么判断出错
	if(n_write!=n_read){
	    perror("write");
	    exit(0);
	}
    }
    if(n_read==0){
	printf("finsih\n");
    }else if(n_read<0){
	perror("read");
	exit(0);
    }
    return 0;
}

