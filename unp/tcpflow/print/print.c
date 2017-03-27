#include "print.h"
static int  packet_len,ip_len,tcp_len,udp_len,icmp_len;

void print_packet_orig(u_char*args,const struct pcap_pkthdr*header,
	const u_char*packet)
{
       static int count = 1;                   /* packet counter */
	/* declare pointers to packet headers */
        const struct ethhdr*eth;
	struct pcap_pkthdr hdr=*header;
	int i=0;
	u_char *ptr=NULL;//print for mac addr;
	packet_len=hdr.len;
	eth = (struct ethhdr*)(packet);
	
	printf("\nPacket number %d:\n", count);
	count++;
	printf("Grabbed packet of length %d\n",packet_len);
	printf("Recevided at:%s",ctime((const time_t*)&hdr.ts));

	ptr=(u_char*)eth->h_dest;
	i=ETH_ALEN;
	printf("Destination address:");
	do{
	    printf("%s%x",(i==ETH_ALEN)?"":":",*ptr++);
	}while(--i>0);
	printf("\n");

	ptr=(u_char*)eth->h_source;
	i=ETH_ALEN;
	printf("source address:");
	do{
	    printf("%s%x",(i==ETH_ALEN)?"":":",*ptr++);
	}while(--i>0);
	printf("\n");


	/* define ethernet header */
	switch(ntohs(eth->h_proto)){
	    case ETH_P_ARP:
		printf("arp packet\n");
		break;
	    case ETH_P_IP:
		printf("ip packet \n");
		ip_packet_print(args,header,packet);
		break;
	    default:
		printf("other packet\n");
		break;
	}
}
void ip_packet_print(u_char *user,const struct pcap_pkthdr*header,
	const u_char *packet)
{


	const struct iphdr *ip;              /* The IP header */
	u_char *new_packet;

	/* define/compute ip header offset */
	ip = (struct iphdr*)(packet + ETH_HLEN);
	ip_len = ip->ihl*4;
	if (ip_len < 20) {
		printf("   * Invalid IP header length: %u bytes\n", ip_len);
		return;
	}
	/* print source and destination IP addresses */
	printf("       From: %s\n", inet_ntoa(ip->saddr));
	printf("         To: %s\n", inet_ntoa(ip->daddr));
	
	/* determine protocol */	

	switch(ip->protocol){
	    case IPPROTO_TCP:
		printf("  protocol:TCP\n");
		tcp_packet_print(user,header,packet);
		break;
	    case IPPROTO_UDP:
		printf("  Protocol:UDP:\n");
		//udp_packet_print(user,header,packet);
		break;
	    case IPPROTO_ICMP:
		printf("  Procotol:icmp:\n");
		//icmp_packet_print(user,header,packet);
		break;
	    default:
		printf("  Protcol:unknon\n");
		break;
	}
}

void tcp_packet_print(u_char *user,const struct pcap_pkthdr*header,
	const u_char *packet)
{
    struct tcphdr *tcpptr;
    int size_payload;
    char *new_packet;
    tcpptr=(struct tcphdr*)(packet+ETH_HLEN+ip_len);
    tcp_len=tcpptr->th_off*4;
    
    if(tcp_len<20){
       fprintf(stderr,"Invalid TCP header length:%u bytes\n",tcp_len);
       return;
    }
    printf("  Src port:%d\n",ntohs(tcpptr->th_sport));
    printf("  Dst prot:%d\n",ntohs(tcpptr->th_dport));

    new_packet=(u_char*)(packet+ETH_HLEN+ip_len+tcp_len);
    size_payload=packet_len-ETH_HLEN-ip_len-tcp_len;

    printf("The content of length is %d\n",size_payload);
    print_payload(new_packet,size_payload); 

}

void print_payload(const u_char *payload,int len)
{
    int len_rem=len;
    int line_width=16;
    int line_len;
    int offset=0;
    const u_char*ch=payload;

    if(len<=0)
	return;
    if(len<=line_width){
	print_hex_ascii_line(ch,len,offset);
	return;
    }
    for(;;){
	line_len=line_width%len_rem;
	print_hex_ascii_line(ch,line_len,offset);
	len_rem=len_rem-line_len;
	ch=ch+line_len;
	offset=offset+line_width;
	if(len_rem<=line_width){
	    print_hex_ascii_line(ch,len_rem,offset);
	    break;
	}
    }
    return;
}
void print_hex_ascii_line(const u_char *payload,int len,int offset)
{
    int i;
    int gap;
    const u_char *ch;

    printf("%05d   ",offset);
    ch=payload;
    for(i=0;i<len;i++){
	printf("%02x ",*ch);
	ch++;
	if(i==7)
	    printf(" ");
    }
    if(len<8)
	printf(" ");
    if(len<16){
	gap=16-len;
	for(i=0;i<gap;i++){
	    printf("  ");
	}
    }
    printf("  ");
    ch=payload;
    for(i=0;i<len;i++){
	if(isprint(*ch))
	    printf("%c",*ch);
	else
	    printf(".");
	ch++;
    }
    printf("\n");
    return ;
}
