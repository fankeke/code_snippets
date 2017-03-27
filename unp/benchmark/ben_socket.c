#include <ben_config.h>
#include <ben_socket.h>

int ben_socket(const char*host,int port)
{
    int			sock;
    unsigned long	inaddr;
    struct sockaddr_in	sin;
    struct hostent	*h;

    bzero(&sin,sizeof(sin));
    sin.sin_family=AF_INET;
    sin.sin_port=htons(port);

    inaddr=inet_addr(host);
    if(inaddr!=INADDR_NONE){
	memcpy(&sin.sin_addr,&inaddr,sizeof(inaddr));
    }else{
	h=gethostbyname(host);
	if(h==NULL){
	    return -1;
	}
	memcpy(&sin.sin_addr,h->h_addr_list[0],h->h_length);
    }

    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0){
	return sock;
    }
    if(connect(sock,(struct sockaddr*)&sin,sizeof(sin))<0){
	return -1;
    }
    return sock;
}











