#ifndef _CONVER_H_
#define _CONVER_H_

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../tcpflow.h"

struct tcp_conver{
    struct tcp_conver*next;
    struct in_addr addr_a;
    struct in_addr addr_b;
    int port_a,port_b;
    int packets,bytes;
    int packets_to,packets_from;
    int bytes_to,bytes_from;

    struct timeval val_s,val_e;
    float  start_time;
    float duration;
    float bps_to,bps_from;
};

void update_tcp_conver_file();
void read_tcp_conver_file();
void write_tcp_conver_file(FILE*fp);
void tcp_conver_handler(u_char*ptemp,int ihl,int n_chars);
void get_tv_main();//for tcp_conver_handler to count time;
#endif
