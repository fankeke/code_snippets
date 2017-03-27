#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>



#define oops(msg)	{perror(msg);exit(0);}
#define BUF_SIZE	1023
#define	EPOLL_SIZE	2

int setnonblocking(s)
{
    int nb=1;
    return ioctl(s,FIONBIO,&nb);
}

int main(int argc,char**argv)
{
    int				sockfd,epfd,usr_fd,event_fd;
    int				events,i,n;
    uint32_t			revents;
    char			buf[BUF_SIZE+1];
    struct epoll_event		event_list[EPOLL_SIZE],ee;
    struct sockaddr_in		serv_addr;

    bzero(buf,BUF_SIZE+1);

    if(argc<2){
	oops("usage:port");
    }

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
	oops("socket()");
    }
    usr_fd=fileno(stdin);
    if(usr_fd<0){
	oops("fileno()");
    }
    epfd=epoll_create(EPOLL_SIZE);
    if(epfd<0){
	oops("epoll_create()");
    }

    ee.data.fd=sockfd;
    ee.events=EPOLLIN|EPOLLOUT;
    if(epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ee)<0){
	oops("epoll_ctl");
    }

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr=inet_addr("192.168.1.101");
    
    if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
	if(errno!=EAGAIN){
	    oops("connect");
	}
    }
   
    if(setnonblocking(sockfd)<0){
	oops("setnonblocking()");
    }
    if(setnonblocking(usr_fd)<0){
	oops("setnonblocking()");
    }

    for(;;){
	events=epoll_wait(epfd,event_list,EPOLL_SIZE,-1);
	if(events<0){
	    oops("epoll_wait()");
	}
	for(i=0;i<events;i++){
	    event_fd=event_list[i].data.fd;
	    revents=event_list[i].events;

	    if(event_fd==sockfd){
		if(revents&EPOLLIN){
		    n=read(sockfd,buf,BUF_SIZE);
		    if(n<0&&errno==EAGAIN){
			continue;
		    }else if(n==0){
			printf("server closed\n");
			close(sockfd);
			close(epfd);
			return;
		    }
		    buf[n]=0;
		    printf("message:%s\n",buf);
		    bzero(buf,BUF_SIZE+1);
		}else{
		    n=read(usr_fd,buf,BUF_SIZE);
		    if(n<0&&errno==EAGAIN){
			continue;
		    }
		    buf[n]=0;
		    n=send(sockfd,buf,n,0);
		    bzero(buf,BUF_SIZE+1);
		}
	    }
	}
    }
}






    












