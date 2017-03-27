#ifndef _SV_CONFIG_H_INCLUDED_
#define _SV_CONFIG_H_INCLUDED_

#include <sv_core.h>

#define err()	strerror(errno)
#define WEB_DIR		"/tmp/web"

#define SERVER_ADDR		0
#define SERVER_PORT		8080
#define LISTEN_Q		5

#define MAX_REQ_LENGTH		1024
#define MAX_NAME_LENGTH		1024

#define BLOCK_SIZE		16384


#define MAX_CONN		1024
#endif
