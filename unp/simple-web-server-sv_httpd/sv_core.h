#ifndef _SV_CORE_H_INCLUDED_
#define _SV_CORE_H_INCLUDED_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <syslog.h>
#include <signal.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <sv_config.h>
#include <sv_conn.h>




void sv_core_httpd();



#endif
