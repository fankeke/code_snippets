//in_addr_t is long type


typedef struct {
    in_addr_t		addr;
    in_addr_t		mask;
}mycidr_t;


in_addr_t ngx_inet_addr(u_char*text,int len)
{
    in_addr_t		addr,octer;
    u_char*p,c;
    addr=octer=0;
    int n=0;
    for(p=text;p<text+len;p++){
	c=*p;
	if(c>='0'&&c<='9'){
	    octer=octer*10+c-'0';
	    if(octer>255){
		return -1;
	    }
	    continue
	}else if (c=='.'){
	    addr=(addr<<8)+octer;
	    octer=0;
	    n++;
	    continue;
	}else{
	    return -1;
	}
    }
    if(n==3){
	addr=(addr<<8)+octer;
	return htonl(addr);
    }
    return -1;
}



in_addr_t myinet_addr(u_char*text,int len)
{
    in_addr_t			addr,octer;
    int				n=0;
    u_char			*p,c;

    addr=octer=0;

    for(p=text;p<text+len;p++){
	c=*p;
	if(c>='0'&&c<='9'){
	    octer=octer*10+c-'0';
	    if(octer>255)
		return -1;
	    continue;
	}else if(c=='.'){
	    addr=(addr<<8)+octer;
	    octer=0;
	    n++;
	    continue;
	}else
	    return -1;
    }
    if(n==3){ //only this case can we return correct
	addr=(addr<<8)+octer;
	return htonl(addr);
    }
    return -1;
}

u_char* my_findslash(u_char*p,u_char*last,u_char e)
{
    while(p<last){
	if(*p==e)
	    return p;
	p++;
    }
    return NULL;
}

int my_pto_cidr(u_char*addr,int len,mycidr_t*cidr)
{
    u_char			*p,*last,*mask;
    int				 shift;

    p=addr;
    last=p+len;
    mask=my_findslash(p,last,'/');

    len=(mask?mask:last)-p;

    cidr->addr=myinet_addr(p,len);

    if(cidr->addr==-1)
	return -1;
    if(mask==NULL){
	cidr->mask=0xffffffff;
	return 0;
    }

    mask++;
    shift=atoi(mask,last-mask);
    if(shift<0||shift>32)
	return -1;
    cidr->mask=htonl((0xffffffff<<(32-shift)));

    return 0;
}








