#ifndef __SUMMARY_H
#define __SUMMARY_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PROTOCL_NUM 10 
struct pkt_type
{
    char name[10];
    int packet_number;
    float number_percent;
    int packet_bytes;
    float byte_percent;
};
#define PROTO_ARP_  0
#define PROTO_IP_   1
#define PROTO_ICMP_ 2
#define PROTO_UDP_  3
#define PROTO_DNS_  4
#define PROTO_TCP_  5
#define PROTO_HTTP_ 6
#define PROTO_HTTP_ack 7
#define PROTO_SSL_  8
#define PROTO_SSL_ack  9

void write_summary_file(FILE*fp);
void write_summary_file_info(FILE*fp);
void read_sumamary_file();

void update_pkt_type(int type,int length);
void update_summary_file();

void first_packet_time();
#endif
