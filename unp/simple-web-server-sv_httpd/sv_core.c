#include <sv_config.h>
#include <sv_core.h>

static int sv_open_listen();
static int sv_create_conn(int fd);

void sv_core_httpd()
{
    int			listenfd,clientfd,maxfd;
    int			rv;
    fd_set		rfs,wfs;


    if(chdir(WEB_DIR)){
	syslog(LOG_ERR,"chdir():%s",err());
	return;
    }

    listenfd=sv_open_listen();
    if(listenfd<0){
	return;
    }

    sv_init_conn();
    syslog(LOG_ERR,"waiting for connctions..");

    for(;;){
	FD_ZERO(&rfs);
	FD_ZERO(&wfs);
	FD_SET(listenfd,&rfs);

	maxfd=sv_collect_fd(&rfs,&wfs);

	if(maxfd<listenfd){
	    maxfd=listenfd;
	}

	rv=select(maxfd+1,&rfs,&wfs,NULL,NULL);
	if(rv<0){
	    syslog(LOG_ERR,"select():%s",err());
	    return;
	}
	
	if(FD_ISSET(listenfd,&rfs)){
	    clientfd=sv_create_conn(listenfd);
	    if(clientfd<0){
		continue;
	    }
	    rv=sv_insert_conn(clientfd);
	    if(rv<0){
		break;
	    }
	}
	sv_handler_conn(&rfs,&wfs);
    }
    syslog(LOG_ERR,"terminate by last error....");
    return;
}






static int
sv_open_listen()
{
    int			listenfd;
    struct sockaddr_in	sin;
    struct hostent	*h;


    listenfd=socket(AF_INET,SOCK_STREAM,0);
    if(listenfd<0){
	syslog(LOG_ERR,"socket():%s",err());
	return -1;
    }

    bzero(&sin,sizeof(sin));

    if(SERVER_ADDR){
	h=gethostbyname(SERVER_ADDR);
	if(h==NULL){
	    syslog(LOG_ERR,"gethostbyname():%s",err());
	    return -1;
	}
	memcpy(&sin.sin_addr,h->h_addr_list[0],h->h_length);
    }else{
	sin.sin_addr.s_addr=INADDR_ANY;
    }

    sin.sin_family=AF_INET;
    sin.sin_port=htons(SERVER_PORT);

    if(bind(listenfd,(struct sockaddr*)&sin,sizeof(sin))<0){
	    syslog(LOG_ERR,"bind():%s",err());
	    return -1;
    }
    if(listen(listenfd,LISTEN_Q)<0){
	    syslog(LOG_ERR,"listen():%s",err());
	    return -1;
    }

    return listenfd;
}













static int
sv_create_conn(int listenfd)
{
    int			clientfd;
    int			rv;

    while((clientfd=accept(listenfd,NULL,NULL))<0){
	if(errno!=EINTR){
	    syslog(LOG_ERR,"accept():%s",err());
	    return -1;
	}
    }
    rv=fcntl(clientfd,F_SETFL,O_NONBLOCK);
    if(rv<0){
	syslog(LOG_ERR,"fcntl():%s",err());
    	return -1;
    }
    return clientfd;
}



