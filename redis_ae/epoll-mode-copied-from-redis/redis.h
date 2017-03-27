#ifndef _REDIS_H_INCLUDED_H_
#define _REDIS_H_INCLUDED_H_

#include "ae.h"
#include "adlist.h"
#include "anet.h"

#define REDIS_OK		0
#define REDIS_ERR		-1

#define	REDIS_BINDADDR_MAX	16
#define REDIS_MAX_CLIENTS	10000


#define REDIS_SERVERPORT	6379
#define REDIS_TCP_BACKLOG	511

#define REDIS_IP_STR_LEN	46

#define REDIS_REPLY_CHUNK_BYTES	(16*1024)

struct redisServer{
    pid_t		 pid;
    aeEventLoop		*el;
    /*Networking*/
    int 		 port;
    int		 	 tcp_backlog;
    int			 tcpkeepalive;
    char		*bindaddr[REDIS_BINDADDR_MAX];
    int			 bindaddr_count;
    int			 ipfd[REDIS_BINDADDR_MAX];
    int			 ipfd_count;
    char		 neterr[ANET_ERR_LEN];
    /*limits*/
    unsigned int	 maxclients;
    uint64_t		 next_client_id;

    int			 stat_numconnections;

    list		*clients;
/*    list		*clients_to_close;
*/
};
    
typedef struct redisClient{
    uint64_t		 id;  /* Client incremental unique ID.*/
    int			 fd;
    int			 flags;

    /*Response buffer*/

    int			 bufpos;
    char		 buf[REDIS_REPLY_CHUNK_BYTES];

}redisClient;


extern struct redisServer	server;


#endif
