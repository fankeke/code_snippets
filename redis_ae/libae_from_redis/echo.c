#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "./src/anet.h"
#include "./src/ae.h"

char anet_error[ANET_ERR_LEN];


int main()
{
    int server_fd=anetTcpServer(anet_error,8000,"0.0.0.0",0);
    if(server_fd<0){
	printf("%s\n",anet_error);
	exit(0);
    }

    aeEventLoop*loop=aeCreateEventLoop(1024);
    if(!loop){
	printf("www\n");
	exit(0);
    }
    if((aeCreateFileEvent(loop,server_fd,AE_READABLE,acceptHandler,NULL))<0){
	printf("aeCreatefileEvent");
	exit(0);
    }
    aeMain(loop);

}
