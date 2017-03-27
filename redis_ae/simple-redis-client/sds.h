#ifndef _SDS_H_INCLUDE_H_
#define _SDS_H_INCLUDE_H_

#include "utils.h"

typedef char* sds;
struct sdshdr{
    long len,free;
    char buf[0];
};

size_t sdslen(sds s);
size_t sdsavail(sds s);

sds sdsnewlen(const void*init,size_t initlen);
sds sdsnew(const char*init);
void sdsfree(sds s);

sds sdsempty();

sds sdscat(sds s,char*t);
sds sdscatlen(sds s,void*t,size_t len);
sds sdscatprintf(sds s,const char*fmt,...);

sds sdstrim(sds s,const char*cset);

#endif
