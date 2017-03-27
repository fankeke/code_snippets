
typedef struct bufferlistnode_s bufferlistnode;
struct bufferlistnode_s{
    char data[BUFFER_CHUNK_SIZE];
    int size;
    bufferlistnode*next;
};

typedef struct {
    bufferlistnode*head,*tail;
    int read_pos;
    bufferlistnode*write_node;
}bufferlist;

bufferlist*tc_create_blist(int n)
{
    if(n<1) return  NULL;

    bufferlist*blist=malloc(sizeof(*blist));
    bufferlistnode *last,*node=malloc(sizeof(*node));


    node->size=0;
    node->next=NULL;

    blist->head=blist->write_node=node;
    last=node;

    int i;
    for(i=1;i<n;i++){
	node=malloc(sizeof(*node));
	node->size=0;
	node->next=NULL;
	last->next=node;
	last=node;
    }
    blist->tail=last;
    blist->read_pos=0;
    return blist;
}

char*bufferlistgetspace(bufferlist*blist,int*len)
{
    if(blist->write_node==blist->tail&&
	    blist->write_node->size==BUFFER_CHUNK_SIZE){
	*len=0;
	return NULL;
    }
    *len=BUFFER_CHUNK_SIZE-blist->write_node->size;
    return blist->write_node->data+blist->write_node->size;
}

void bufferlistreducecspace(bufferlist*blist,int len)
{
    blist->write_node->size+=len;
    if(blist->write_node->size==BUFFER_CHUNK_SIZE&&
	    blist->write_node!=blist->tail)
	blist->write_node=blist->write_node->next;
}





typedef struct client{
    int fd;
    int flags;
    struct client*remote;
    bufferlist*blist;
    void(*on_local_error)(struct client*c);
    void(*on_peer_error)(struct client*c);
}client;


client*tc_create_remote(int c)
{
    client*r=malloc(sizeof(*r));
    r->flags=0;
    int fd=anetTcpNonBlockConnect(error_msg,peer_addr,peer_port);
    if(r==NULL||fd==-1) return NULL;

    anetNonBlock(NULL,fd);
    anetTcpNoDelay(NULL,fd);

    r->fd=fd;
    r->blist=tc_create_blist(3);
    r->remote=c;
    r->on_local_error=tc_remote_donw;
    r->on_peer_error=NULL;

    if(aeCreateFileEvent(el,r->fd,AE_READABLE,tc_readincome,r)==AE_ERR){
	tc_free_client(r);
	return NULL;
    }

    return r;

}

client*tc_create_client(int fd)
{
    client*c=malloc(sizeof(client));
    c->flags=0;
    if(!c) return NULL;
    anetNonBlock(NULL,fd);
    anetTcpNoDelay(NULL,fd);

    c->fd=fd;
    c->blist=tc_create_blist(3);
    c->remote=tc_create_remote(c);
    c->on_local_error=tc_free_client;
    c->on_peer_error=tc_close_after_sent;

    if(c->remote==NULL){
	close(fd);
	free(c);
	return NULL;
    }

    return c;
}

void tc_AccepTcpHandler(aeEventLoop*el,int fd,void*privdata,int mask)
{
    int cport,cfd;
    char cip[128];

    cfd=anetTcpAccept(error_msg,fd,cip,&cport);

    client*c=tc_create_client(cfd);

    if(c==NULL||aeCreateFileEvent(el,cfd,AE_READABLE,tc_readincome,c)
	    ==AE_ERR){
	error();
	tc_free_client(c);
    }
}

void tc_readincome(aeEventLoop*el,int fd,void*privdata,int mask)
{
    client*c=(client*)privdata;
    client*r=c->remote;

    char*buf;
    int len,nread=0;

    //direct read to peer bufffer,r->blist
    while(1){
	buf=bufferlistgetspace(r->blist,&len);
	if(buf==NULL) break;       //just leave data in kernel LT,mode
	nread=recv(fd,buf,len,0);
	if(nread==-1){		   //devour kernel reading buffer..back
	    if(errno==EAGAIN){
		nread=0;
	    }else{
		goto ERROR;
	    }
	}else if (nread==0){	//connection closed by peer
	    goto ERROR;
	}

	if(nread){
	    bufferlistreducecspace(r->blist,nread);
	    setwriteevent(r);
	}else{                // errno ==EAGAIN ,so nread ==0 ,so break
	    break;
	}
    }
    return;
ERROR:
    c->on_local_error(c);
}











