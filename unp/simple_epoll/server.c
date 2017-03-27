#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define oops(msg)	{perror(msg);exit(0);}
#define	BUF_SIZE	10
#define EPOLL_SIZE	1000

void handler_client(int fd,uint32_t revents);
int setnonblocking(s)
{
    int nb=1;
    return ioctl(s,FIONBIO,&nb);
}

int main(int argc,char**argv)
{
    int			listen_fd,conn_fd,event_fd,epfd;
    int			events,i;
    uint32_t		revents;
    struct epoll_event	event_list[EPOLL_SIZE],ee;
    struct sockaddr_in	serv_addr,cli_addr;
    socklen_t		cli_len;


    if(argc<2){
	oops("usage:port");
    }

    listen_fd=socket(AF_INET,SOCK_STREAM,0);
    if(listen_fd<0){
	oops("socket()");
    }
    epfd=epoll_create(EPOLL_SIZE);
    if(epfd<0){
	oops("epoll_create");
    }

    ee.data.fd=listen_fd;
    ee.events=EPOLLIN;
    if(epoll_ctl(epfd,EPOLL_CTL_ADD,listen_fd,&ee)<0){
	oops("epoll_ctl");
    }

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr=inet_addr("192.168.1.101");

    if(bind(listen_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
	oops("bind");
    }

    listen(listen_fd,5);

    for(;;){
	events=epoll_wait(epfd,event_list,EPOLL_SIZE,-1);
	if(events<0){
	    oops("epoll_wait");
	}
	for(i=0;i<events;i++){
	    event_fd=event_list[i].data.fd;
	    revents=event_list[i].events;

	    if(event_fd==listen_fd){
		cli_len=sizeof(cli_addr);
		conn_fd=accept(listen_fd,(struct sockaddr*)&cli_addr,&cli_len);
		if(conn_fd<0){
		    oops("accept");
		}
		printf("client %s:%d connected\n",
			inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));

		if(setnonblocking(conn_fd)<0){
		    oops("setnonblocking");
		}
		ee.data.fd=conn_fd;
		ee.events=EPOLLIN|EPOLLOUT|EPOLLET;
		if(epoll_ctl(epfd,EPOLL_CTL_ADD,conn_fd,&ee)<0){
		    oops("epoll_ctl");
		}
	    }else{
		handler_client(event_fd,revents);
	    }
	}
    }
}

void 
handler_client(int fd,uint32_t revents)
{
    int			n;
    char		buf[BUF_SIZE+1];

    if(revents&EPOLLIN){
	for(;;){
	    n=read(fd,buf,BUF_SIZE,0);
	    if(n<0&&errno!=EAGAIN){
		oops("read wrong");
	    }else if(n==0){
		printf("client closed\n");
		close(fd);
		return;
	    }
	    if(n==BUF_SIZE){
	       	buf[n]=0;
		printf("message:%s\n",buf);
		n=send(fd,buf,strlen(buf),0);
		bzero(buf,BUF_SIZE+1);
	    }else if(n<BUF_SIZE){
		buf[n]=0;
		printf("message:%s\n",buf);
		n=send(fd,buf,strlen(buf),0);
		bzero(buf,BUF_SIZE+1);
		return;
	    }
	}
    }
}




    












