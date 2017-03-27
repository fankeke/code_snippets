#ifndef _SV_CONN_H_INCLUDED_
#define _SV_CONN_H_INCLUDED_


#include <sv_config.h>
#include <sv_core.h>

#define HTTP_HEADER 	"HTTP/1.0 200 OK\nContent-type: "
#define EOL		"\r\n\r\n"



typedef struct sv_conn_s sv_conn_t;
typedef struct sv_type_s sv_type_t;

struct sv_conn_s {
    int		 fd,sd;
    char	*head;
    off_t	 foff;
    size_t	 hlen,hoff;
    int		 state;
};

struct sv_type_s {
    size_t	 ext_len;
    char	*ext;
    char	*http_header;
    size_t	 header_len;
};
    

int sv_collect_fd(fd_set*r,fd_set*w);

void sv_init_conn();
int sv_insert_conn(int fd);
void sv_handler_conn(fd_set*r,fd_set*w);


extern sv_conn_t*	conn_list[];
extern int		num_conn;

#endif
