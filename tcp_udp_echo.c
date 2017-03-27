/*
  using tcp and udp socket bind the same one port 
  just echo message to client
  */

#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdarg.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE	10

#define EVENT_ET  1
#define EVENT_LT  0

#define BUF_SIZE  1024
#define ERR_BUF	  128

static void seterrbuf(char*buf,const char*format,...)
{
    va_list ap;
    if(!buf) return;
    va_start(ap,format);
    vsnprintf(buf,ERR_BUF,format,ap);
    va_end(ap);
}

int setnonblocking(int fd)
{
    int flags;
    if((flags=fcntl(fd,F_GETFL,0))<0){
	perror("fcntl_get");
	return -1;
    }
    flags|=O_NONBLOCK;
    if((fcntl(fd,F_SETFL,flags))<0){
	perror("fcntl_set");
	return -1;
    }
    return 0;
}

int create_tcp_sock(struct sockaddr*server,char*errbuf)
{
    socklen_t addrlen=sizeof(struct sockaddr_in);
    int sockfd;
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
	seterrbuf(errbuf,"%s\n",strerror(errno));
	return -1;
    }
    if((setnonblocking(sockfd))<0){
	seterrbuf(errbuf,"%s\n",strerror(errno));
	return -1;
    }
    int on=1;
    if((setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0){
	seterrbuf(errbuf,"%s\n",strerror(errno));
	return -1;
    }

    if((bind(sockfd,server,addrlen))<0){
	seterrbuf(errbuf,"bind:%s\n",strerror(errno));
	return -1;
    }

    if((listen(sockfd,5))<0){
	seterrbuf(errbuf,"%s\n",strerror(errno));
	return -1;
    }

    return sockfd;
}
int create_udp_sock(struct sockaddr*server,char*errbuf)
{
    int sockfd;
    if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0){
	seterrbuf(errbuf,"%s\n",strerror(errno));
	return -1;
    }
    int on=1;
    if((setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0){
	seterrbuf(errbuf,"%s\n",strerror(errno));
	return -1;
    }
    if((setnonblocking(sockfd))<0){
	seterrbuf(errbuf,"%s\n",strerror(errno));
	return -1;
    }

    socklen_t addrlen=sizeof(struct sockaddr_in);
    if((bind(sockfd,server,addrlen))<0){
	seterrbuf(errbuf,"bind:%s\n",strerror(errno));
	return -1;
    }
    return sockfd;
}
int epoll_add_fd(int epoll_fd,int fd,char*errbuf)
{
    struct epoll_event ev;
    ev.data.u64=0;
    ev.data.fd=fd;
    ev.events=EPOLLIN|EPOLLET;
    if((epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&ev))<0){
	seterrbuf(errbuf,"%s\n",strerror(errno));
	return -1;
    }

    return 0;
}
void handle_tcp_client(int listenfd,int epoll_fd,char*errbuf)
{
    struct sockaddr_in client_addr;
    socklen_t addrlen=sizeof(client_addr);
    int connfd=accept(listenfd,(struct sockaddr*)&client_addr,&addrlen);
    if(connfd<0){
	seterrbuf(errbuf,"accpet:%s\n",strerror(errno));
	return;
    }
    if((epoll_add_fd(epoll_fd,connfd,errbuf))<0){
	fprintf(stderr,"epoll_add_fd:%s\n",errbuf);
	return;
    }
    fprintf(stdout,"accept tcp client:%s:%d\n",
	    inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
}

int main(int argc,char**argv)
{

    if(argc<=2){
	fprintf(stderr,"usage:%s ip,port",argv[0]);
	return 0;
    }
    const char*ip=argv[1];
    int port=atoi(argv[2]);
    char err_buf[ERR_BUF];

    struct sockaddr_in server;
    server.sin_family=AF_INET;
    server.sin_port=htons(port);
    inet_pton(AF_INET,ip,&server.sin_addr);

    int tcp_sock=create_tcp_sock((struct sockaddr*)&server,err_buf);
    int udp_sock=create_udp_sock((struct sockaddr*)&server,err_buf);

    if(tcp_sock<0||udp_sock<0){
	fprintf(stderr,"%s\n",err_buf);
	return 0;
    }
    printf("listen port:%d on tcp and upd \n",port);

    int epoll_fd;
    if((epoll_fd=epoll_create(10))<0){
	perror("epoll_create");
	return 0;
    }

    if((epoll_add_fd(epoll_fd,tcp_sock,err_buf))<0||
	    (epoll_add_fd(epoll_fd,udp_sock,err_buf)<0)){
	fprintf(stderr,"%s\n",err_buf);
	return 0;
    }

    struct epoll_event events[MAX_EVENT_NUMBER];

    while (1){
	int numevents=epoll_wait(epoll_fd,events,MAX_EVENT_NUMBER,-1);
	if(numevents<0){
	    fprintf(stderr,"epoll_wait:%s\n",strerror(errno));
	    continue;
	}
	int i;
	for(i=0;i<numevents;i++){
	    int sockfd=events[i].data.fd;
	    if(sockfd==tcp_sock){
		handle_tcp_client(sockfd,epoll_fd,err_buf);
	    }else if (sockfd==udp_sock){
		char message[BUF_SIZE];
		struct sockaddr_in client_addr;
		socklen_t addrlen=sizeof(client_addr);
		bzero(message,BUF_SIZE);
		int ret=recvfrom(sockfd,message,BUF_SIZE-1,0,(struct sockaddr*)&client_addr,&addrlen);
		if(ret>0){
		    printf("udp peer:%s:%d\t message:%s\n",
			    inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port),message);
		}
	    }else{
		char message[BUF_SIZE];
		while(1){
		    bzero(message,BUF_SIZE);
		    int ret=recv(sockfd,message,BUF_SIZE-1,0);
		    if(ret<0){
			if(errno==EAGAIN||errno==EWOULDBLOCK){
			    break;
			}else{
			    fprintf(stderr,"recv:%s\n",strerror(errno));
			    break;
			}
		    }else if(ret==0){
			close(sockfd);
			break;
		    }else{
			printf("tcp message:%s\n",message);
			send(sockfd,message,strlen(message),0);
		    }
		}
	    }
	}
    }
}













