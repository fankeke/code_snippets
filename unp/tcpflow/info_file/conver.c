#include "conver.h"

static struct tcp_conver*tcp_con_head=NULL;
static struct timeval tv_main;
void read_tcp_conver_file()
{
    FILE *fp;
    char temp[100];
    if((fp=fopen("tcp_conver","r"))==NULL){
	printf("fopen read tcp_conver\n");
	exit(0);
    }
    while(fscanf(fp,"%s",temp)!=EOF){
    }
    fclose(fp);

}
void get_tv_main()
{
    gettimeofday(&tv_main,NULL);
}
void update_tcp_conver_file()
{
    FILE*fp=NULL;
    read_tcp_conver_file();
    if((fp=fopen("tcp_conver","w"))==NULL){
	printf("fopen write tcp_conver");
	exit(0);
    }
    write_tcp_conver_file(fp);
    fclose(fp);
}

void tcp_conver_handler(u_char *ptemp,int ihl,int n_chars)
{
    struct tcp_conver*ptcp_con=tcp_con_head;
    struct iphdr*pip=(struct iphdr*)(ptemp);
    struct tcphdr*ptcp=(struct tcphdr*)(ptemp+ihl);
    struct timeval tv;
    while(ptcp_con){
	if(ptcp_con->addr_a.s_addr==pip->saddr&&
		ptcp_con->addr_b.s_addr==pip->daddr&&
		ptcp_con->port_a==ptcp->th_sport&&
		ptcp_con->port_b==ptcp->th_dport){
	    gettimeofday(&tv,NULL);
	    ptcp_con->val_e.tv_sec=tv.tv_sec-tv_main.tv_sec;
	    ptcp_con->val_e.tv_usec=tv.tv_usec-tv_main.tv_usec;

	    ptcp_con->packets_to++;
	    ptcp_con->bytes_to+=n_chars;
	    ptcp_con->packets++;
	    ptcp_con->bytes+=n_chars;
	    goto done;
	}else if(ptcp_con->addr_a.s_addr==pip->daddr&&
            ptcp_con->addr_b.s_addr==pip->saddr&&
	    ptcp_con->port_a==ptcp->th_dport&&
	    ptcp_con->port_b==ptcp->th_sport){

	    gettimeofday(&tv,NULL);
	    ptcp_con->val_e.tv_sec=tv.tv_sec-tv_main.tv_sec;
	    ptcp_con->val_e.tv_usec=tv.tv_usec-tv_main.tv_usec;

	    ptcp_con->packets_from++;
	    ptcp_con->bytes_from+=n_chars;
	    ptcp_con->packets++;
	    ptcp_con->bytes+=n_chars;
	    goto done;
	}
	else 
	    ptcp_con=ptcp_con->next;
    }
    if(ptcp_con==NULL){
	gettimeofday(&tv,NULL);
	ptcp_con=malloc(sizeof(struct tcp_conver));
	bzero(ptcp_con,sizeof(struct tcp_conver));

	ptcp_con->val_s.tv_sec=tv.tv_sec-tv_main.tv_sec;
	ptcp_con->val_s.tv_usec=tv.tv_usec-tv_main.tv_usec;

	ptcp_con->addr_a.s_addr=(pip->saddr);
	ptcp_con->addr_b.s_addr=(pip->daddr);

	ptcp_con->port_a=(ptcp->th_sport);
	ptcp_con->port_b=(ptcp->th_dport);

	ptcp_con->packets=1;
	ptcp_con->bytes=n_chars;
	ptcp_con->bytes_to=n_chars;
	ptcp_con->packets_to=1;
	ptcp_con->bytes_from=0;
	ptcp_con->packets_from=0;
	
	ptcp_con->next=tcp_con_head;
        tcp_con_head=ptcp_con;	
    }
done:
    return;
}

void write_tcp_conver_file(FILE*fp)
{
    struct tcp_conver*ptcp_con=tcp_con_head;
    char str_info[1024]={0};
    sprintf(str_info,"tcp conversation\n"
	    	     "Address A\tport A\tAddress B\tport B\t"
	    	     "packets\tbytes\t"
		     "pkt A->B byte A->B pkt B->A byte B->A\t"
		     "start_time\tduration\t"
	    	     "bps A->B\tbps A<-B\n");
    int info_len=strlen(str_info);
    str_info[info_len]=0;
    fwrite(str_info,info_len,1,fp);

    char *a1;
    char a2[20],a3[20];
    int porta,portb;
    while(ptcp_con){
	bzero(str_info,sizeof(str_info));
	a1=inet_ntoa(ptcp_con->addr_a);
	strcpy(a2,a1);
	a1=inet_ntoa(ptcp_con->addr_b);
	strcpy(a3,a1);
	porta=ntohs(ptcp_con->port_a);
	portb=ntohs(ptcp_con->port_b);
	ptcp_con->start_time=((float)ptcp_con->val_s.tv_sec+
		(float)ptcp_con->val_s.tv_usec/1000000);
	ptcp_con->duration=((float)ptcp_con->val_e.tv_sec+
		(float)ptcp_con->val_e.tv_usec/1000000-
		(float)ptcp_con->val_s.tv_sec-
		(float)ptcp_con->val_s.tv_usec/1000000);
        ptcp_con->bps_from=((float)ptcp_con->bytes_from*8/ptcp_con->duration);
	ptcp_con->bps_to=((float)ptcp_con->bytes_to*8/ptcp_con->duration);
		
	sprintf(str_info,"%s\t%d\t%s\t%d\t%d\t%d\t%d\t%d\t\t%d\t%d\t"
	    		 "%.6f\t%.6f\t%.2f\t%.2f\n",
		a2,porta,
		a3,portb,
		ptcp_con->packets,ptcp_con->bytes,
		ptcp_con->packets_to,ptcp_con->bytes_to,
		ptcp_con->packets_from,ptcp_con->bytes_from,
		ptcp_con->start_time,ptcp_con->duration,
		ptcp_con->bps_to,ptcp_con->bps_from);
	info_len=strlen(str_info);
	str_info[info_len]=0;
	fwrite(str_info,info_len,1,fp);
	ptcp_con=ptcp_con->next;
    }

}

