#ifndef __PRINT_H_
#define __PRINT_H_


#include "../tcpflow.h"
void print_packet_orig(u_char*user,const struct pcap_pkthdr*h,const u_char *packet);
void ip_packet_print(u_char*user,const struct pcap_pkthdr*h,const u_char *packet);
//void icmp_packet_print(u_char*user,const struct pcap_pkthdr*h,
//	const u_char *packet);
//void udp_packet_print(u_char*user,const struct pcap_pkthdr*h,
//	const u_char *packet);
void tcp_packet_print(u_char*user,const struct pcap_pkthdr*h,const u_char *packet);
void print_payload(const u_char *data,int size_payload);
void print_hex_ascii_line(const u_char *payload,int len,int offset);

#endif
