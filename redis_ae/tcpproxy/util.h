#ifndef _UTIL_H_
#define _UTIL_H_

//#define BUFFER_CHUNK_SIZE	1024*1024*2
#define BUFFER_CHUNK_SIZE	16

#define LogFatal(s...) do {\
    LogInternal(kFatal,s);\
    LogPrint(kFatal,"\n");\
    exit(EXIT_FAILURE);\
}while(0)

#define LogInfo(s...) do {\
    LogInternal(kInfo,s);\
    LogPrint(kInfo,"\n");\
}while(0)

#define LogError(s...) do {\
    LogInternal(kError,s);\
    LogPrint(kError,"\n");\
}while(0)

#define LogDebug(s...) do {\
    LogInternal(kDebug,s);\
    LogPrint(kDebug,"\n");\
}while(0)

typedef struct BufferListNode{
    char data[BUFFER_CHUNK_SIZE];
    int size; //how many byte this node has stored
    struct BufferListNode*next;
}BufferListNode;

typedef struct BufferList{
    BufferListNode*head;
    BufferListNode*tail;
    int read_pos;             
    BufferListNode*write_node;//last node that have data right now
}BufferList;


BufferList*AllocBufferList(int n);
void FreeBufferList(BufferList*blist);

char*BufferListGetSpace(BufferList*blist,int*len);
void BufferListPush(BufferList*blist,int len);

char*BufferListGetData(BufferList*blist,int*len);
void BufferListPop(BufferList*blist,int len);

typedef enum LogLevel{
    kNone=0,
    kFatal,
    kCritical,
    kError,
    kWarning,
    kInfo,
    kDebug,
}LogLevel;


void InitLogger(LogLevel level,const char*filename);
void LogInternal(LogLevel level,const char*fmt,...);
void LogPrint(LogLevel level,const char*fmt,...);

#endif
