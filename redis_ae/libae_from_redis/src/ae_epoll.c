#include <sys/epoll.h>

typedef struct aeApiData{
    int epfd;
    struct epoll_event*events;

}aeApiData;



static int aeApiCreate(aeEventLoop*eventLoop){
    aeApiData*Data=malloc(sizeof(aeApiData));

    if(!Data) return -1;
    Data->events=malloc(sizeof(struct epoll_event)*eventLoop->setsize);
    if(!Data->events){
	free(Data);
	return -1;
    }
    Data->epfd=epoll_create(1024);
    if(Data->epfd<0){
	free(Data->events);
	free(Data);
	return -1;
    }

    eventLoop->apidata=Data;
    return 0;
}
