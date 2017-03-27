#include <sv_config.h>
#include <sv_core.h>

sv_conn_t* 	conn_list[MAX_CONN];
int		num_conn;

enum {SV_AWAITING_REQUST,SV_SENDING_HEADER,SV_SENDING_FILE};

sv_type_t sv_types[]={
    {	4,".jpg",HTTP_HEADER "image/jpeg" EOL,0},
    {	5,".jpeg",HTTP_HEADER "image/jpeg" EOL,0},
    {	4,".gif",HTTP_HEADER "image/gif" EOL,0},
    {	4,".txt",HTTP_HEADER "plain/txt" EOL,0},
    {	4,".htm",HTTP_HEADER "plain/html" EOL,0},
    {	5,".html",HTTP_HEADER "plain/html" EOL,0},
    {	0,0,0,0}
};


static sv_conn_t* sv_add_conn(int fd);
static sv_conn_t* sv_del_conn(sv_conn_t*c);
static int	  sv_add_fd(int fd,fd_set*set,int maxfd);


static int	sv_readreq_conn(sv_conn_t*c);
static int	sv_getname_conn(char*b,char*f);
static int	sv_type_case(char*t,char*s,size_t len);

static int	sv_sendhead_conn(sv_conn_t*c);
static int	sv_sendfile_conn(sv_conn_t*c);

void sv_init_conn()
{
    int		i;
    for(i=0;i<MAX_CONN;i++){
	conn_list[i]=NULL;
    }
    num_conn=0;
}


int sv_insert_conn(int clientfd)
{
    int			i;
    sv_conn_t		*c;
    for(i=0;i<MAX_CONN;i++){
	if(conn_list[i]==NULL){
	    c=sv_add_conn(clientfd);
	    if(c==NULL){
		return -1;
	    }
	    conn_list[i]=c;
	    break;
	}
    }
    if(i>=MAX_CONN){
	syslog(LOG_ERR,"too many conns...");
	return -1;
    }
    return 1;
}

int sv_collect_fd(fd_set*rfs,fd_set*wfs)
{
    int				 i,maxfd=-1;
    sv_conn_t			*c;	
    for(i=0;i<MAX_CONN;i++){
	if(conn_list[i]){
	    c=conn_list[i];

	    switch(c->state) {
		case SV_AWAITING_REQUST:
		    maxfd=sv_add_fd(c->sd,rfs,maxfd);
		    break;
		case SV_SENDING_HEADER:
		    maxfd=sv_add_fd(c->sd,wfs,maxfd);
		    break;
		case SV_SENDING_FILE:
		    maxfd=sv_add_fd(c->sd,wfs,maxfd);
		    break;
	    }
	}
    }
    return maxfd;
}

void sv_handler_conn(fd_set*rfs,fd_set*wfs)
{
    int				 i,rv;
    sv_conn_t			*c;
    for(i=0;i<MAX_CONN;i++){
	if(conn_list[i]){
	    c=conn_list[i];

	    switch(c->state){
		case  SV_AWAITING_REQUST:
		    if(FD_ISSET(c->sd,rfs)){
			rv=sv_readreq_conn(c);
			if(rv<0){
			    conn_list[i]=sv_del_conn(c);
			    continue;
			}
		    }
		    break;
		case SV_SENDING_HEADER:
		    if(FD_ISSET(c->sd,wfs)){
			rv=sv_sendhead_conn(c);
			if(rv<0){
			    conn_list[i]=sv_del_conn(c);
			    continue;
			}
		    }
		    break;
		case SV_SENDING_FILE:
		    if(FD_ISSET(c->sd,wfs)){
			rv=sv_sendfile_conn(c);
			if(rv!=0){
			    conn_list[i]=sv_del_conn(c);
			    continue;
			}
		    }
		    break;
	    }
	}
    }
}

static int sv_readreq_conn(sv_conn_t*c)
{
    int			bc,rv;
    char		buf[MAX_REQ_LENGTH],fname[MAX_NAME_LENGTH];
    size_t		name_len;
    sv_type_t		*ct;
    while((bc=read(c->sd,buf,sizeof(buf)))<0){
	    if(errno!=EINTR){
		if(errno==EWOULDBLOCK){
		    return 0;
		}
		syslog(LOG_ERR,"read():%s",err());
		return -1;
	    }
    }
    rv=sv_getname_conn(buf,fname);
    if(rv<0){
	return -1;
    }
    name_len=strlen(fname);

    for(ct=sv_types;ct->ext_len;ct++){
	if(sv_type_case(fname+(name_len-ct->ext_len),
		    ct->ext,ct->ext_len)==1)
	    break;
    }
    if(ct->ext_len==0){
	return -1;
    }
    c->head=ct->http_header;
    c->fd=open(fname,O_RDONLY);
    c->hlen=ct->header_len;
    if(c->hlen==0){
	c->hlen=ct->header_len=strlen(ct->http_header);
    }
    if(c->fd<0){
	syslog(LOG_ERR,"open():%s",err());
	return -1;
    }
    c->state=SV_SENDING_HEADER;

    return 1;
}


static int sv_getname_conn(char*buf,char*fname)
{
    off_t		off=4;
    size_t		len=0;
    struct stat		st;
    strtok(buf,"\r\n");

    if(strlen(buf)<5){
	return -1;
    }

    off+=strspn(buf+off,"\t\v");
    off+=strspn(buf+off,"/");

    len=strcspn(buf+off," ");

    if(len>=MAX_NAME_LENGTH){
	return -1;
    }
    strncpy(fname,buf+off,len);
    fname[len]=0;

    if(strstr(fname,"..")){
	return -1;
    }
    if(stat(fname,&st)<0){
	return -1;
    }
    if(!S_ISREG(st.st_mode)){
	return -1;
    }
    return 1;

}
static int
sv_type_case(char*t,char*s,size_t len)
{
    while(len--){
	if(*s=='\0'){
	    if(*t=='\0')
		return 1;
	    return -1;
	}else if(*s!=*t){
	    if(tolower(*s)!=tolower(*t))
		return -1;
	}
	t++;s++;
    }
    return 1;
}
static int
sv_sendhead_conn(sv_conn_t*c)
{
    int			bc;
    while((bc=write(c->sd,c->head+c->hoff,c->hlen-c->hoff))<0){
	if(errno!=EINTR){
	    if(errno==EWOULDBLOCK){
		return 0;
	    }
	    syslog(LOG_ERR,"write():%s",err());
	    return -1;
	}
    }
    c->hoff+=bc;
    if(c->hoff==c->hlen){
	c->state=SV_SENDING_FILE;
    }

    return 1;
}
static int
sv_sendfile_conn(sv_conn_t*c)
{
    int			bc,bs;
    char		buf[BLOCK_SIZE];
    lseek(c->fd,c->foff,SEEK_SET);
    while((bc=read(c->fd,buf,BLOCK_SIZE))<0){
	if(errno!=EINTR){
	    syslog(LOG_ERR,"read():%s",err());
	    return -1;
	}
    }
    if(bc==0){
	syslog(LOG_INFO,"HTTP/1.0 200 OK");
	return 1;
    }
    c->foff+=bc;

    while((bs=write(c->sd,buf,bc))<0){
	if(errno!=EINTR){
	    if(errno==EWOULDBLOCK){
		return 0;
	    }
    	    syslog(LOG_ERR,"write():%s",err());
	    return -1;
	}
    }
    return 0;

}
    



static int
sv_add_fd(int fd,fd_set*set,int maxfd)
{
    if(maxfd<fd){
	maxfd=fd;
    }
    FD_SET(fd,set);
    return maxfd;
}
static sv_conn_t*
sv_add_conn(int clientfd)
{
    sv_conn_t			*c;
    c=malloc(sizeof(sv_conn_t));

    c->state=SV_AWAITING_REQUST;
    c->sd=clientfd;
    c->fd=-1;
    c->hoff=c->foff=0;
    c->head=NULL;

    return c;
}
static sv_conn_t*
sv_del_conn(sv_conn_t*c)
{
    if(c->state==SV_SENDING_HEADER||
	    c->state==SV_SENDING_FILE){
	close(c->fd);
    }
    close(c->sd);
    free(c);
    return NULL;
}












