#include "sds.h"
#include "utils.h"

static void sdsOomAbort()
{
    fprintf(stderr,"SDS:out of memory\n");
    abort();
}

size_t sdsavail(sds s)
{
    struct sdshdr*sh=(void*)(s-(sizeof(struct sdshdr)));
    return sh->free;
}

static sds sdsMakeRoomFor(sds s,size_t addlen)
{
    struct sdshdr*sh,*newsh;
    size_t free=sdsavail(s);
    size_t len,newlen;

    if(free>=addlen) return s;
    len=sdslen(s);
    sh=(void*)(s-(sizeof(struct sdshdr)));
    newlen=(len+addlen)*2;
    newsh=realloc(sh,sizeof(struct sdshdr)+newlen+1);
    if(newsh==NULL) sdsOomAbort();
    newsh->free=newlen-len;
    return newsh->buf;
}

size_t sdslen(sds s)
{
    struct sdshdr*sh=(void*)(s-(sizeof(struct sdshdr)));
    return sh->len;
}

sds sdsnewlen(const void*init,size_t initlen)
{
    struct sdshdr *sh;
    sh=malloc(sizeof(*sh)+initlen+1);
    if(sh==NULL) sdsOomAbort();
    sh->len=initlen;
    sh->free=0;
    if(initlen){
	if(init)memcpy(sh->buf,init,initlen);
	else    memset(sh->buf,0,initlen);
    }
    sh->buf[initlen]='\0';
    return (char*)sh->buf;
}


sds sdsnew(const char*init)
{
   size_t initlen=(init==NULL?0:strlen(init));
   return sdsnewlen(init,initlen);
}

void sdsfree(sds s)
{
    if(s==NULL) return ;
    free(s-(sizeof(struct sdshdr)));
}



sds sdsempty()
{
    return sdsnewlen("",0);
}

sds sdscatlen(sds s,void*t,size_t len)
{
    struct sdshdr*sh;
    size_t curlen=sdslen(s);
    s=sdsMakeRoomFor(s,len);
    if(s==NULL) return NULL;
    sh=(void*)(s-(sizeof(struct sdshdr)));
    memcpy(s+curlen,t,len);
    sh->len=curlen+len;
    sh->free=sh->free-len;
    s[curlen+len]='\0';
    return s;
}

sds sdscat(sds s,char*t)
{
    return sdscatlen(s,t,strlen(t));
}


//试图把fmt格式的内容加入到s的后面
sds sdscatprintf(sds s,const char*fmt,...)
{
    va_list	ap;
    char	*buf,*t;
    size_t	buflen=32;

    while(1){
	buf=malloc(buflen);
	if(buf==NULL) sdsOomAbort();
	buf[buflen-2]='\0';
	va_start(ap,fmt);
	vsnprintf(buf,buflen,fmt,ap);
	va_end(ap);
	if(buf[buflen-2]!='\0'){//如果被覆盖了，显然需要重新分配更大的buf
	    free(buf);
	    buflen*=2;
	    continue;
	}
	break;
    }
    //首先通过buf作为中间存放fmt格式的内容
    t=sdscat(s,buf);
    free(buf);
    return t;
}



//把s中头和尾出现的cset中的字符都去掉
sds sdstrim(sds s,const char*cset)
{
    struct sdshdr*sh=(void*)(s-(sizeof(struct sdshdr)));
    char *start,*end,*sp,*ep;
    size_t	len;

    sp=start=s;
    ep=end=s+sdslen(s)-1;

    while(sp<=end&&strchr(cset,*sp)) sp++;
    while(ep>start&&strchr(cset,*ep)) ep--;

    len=(sp>ep)?0:((ep-sp)+1);

    if(sh->buf!=sp) memmove(sh->buf,sp,len);

    sh->buf[len]='\0';
    sh->free=sh->free+(sh->len-len);
    sh->len=len;

    return s;
}
