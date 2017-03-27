#include "summary.h"
#include <time.h>
extern int packet,bytes;
static struct pkt_type protocl_types[]={
    {"arp",0,0,0,0},
    {"ip",0,0,0,0},
    {"icmp",0,0,0},
    {"udp",0,0,0},
    {"dns",0,0,0},
    {"tcp",0,0,0},
    {"http",0,0,0},
    {"http-",0,0,0},
    {"ssl",0,0,0},
    {"ssl-",0,0,0},
};
static int first_hour=0,first_min=0,first_sec=0;

void first_packet_time()
{
    time_t timep;
    struct tm*first_p;
    time(&timep);
    first_p=gmtime(&timep);

    first_hour=first_p->tm_hour;
    first_min=first_p->tm_min;
    first_sec=first_p->tm_sec;
}
void write_summary_file_info(FILE*fp)
{
    char last_packet[100],first_packet[100];
    int info_len;
    time_t timep;
    struct tm*last_p;
    int elapsed_hour,elapsed_min,elapsed_sec;
    float elapsed;
    char str_info[1024];

    time(&timep);
    last_p=gmtime(&timep);

    sprintf(first_packet,"fist packet:%02d-%02d-%02d %02d:%02d:%02d\n",1900+last_p->tm_year,
	    last_p->tm_mon,last_p->tm_mday,first_hour,first_min,first_sec);
    sprintf(last_packet,"last packet:%02d-%02d-%02d %02d:%02d:%02d\n",1900+last_p->tm_year,
	    last_p->tm_mon,last_p->tm_mday,last_p->tm_hour,last_p->tm_min,last_p->tm_sec);

    info_len=strlen(first_packet);
    first_packet[info_len]=0;
    fwrite(first_packet,info_len,1,fp);

    info_len=strlen(last_packet);
    last_packet[info_len]=0;
    fwrite(last_packet,info_len,1,fp);
    

    elapsed_hour=last_p->tm_hour-first_hour;
    elapsed_min=last_p->tm_min-first_min;
    elapsed_sec=last_p->tm_sec-first_sec;
    if(elapsed_sec<0){
	elapsed_min--;
	elapsed_sec+=60;
    }
    elapsed=elapsed_hour*3600+elapsed_min*60+elapsed_sec;

    bzero(str_info,sizeof(str_info));
    sprintf(str_info,"elapsed:\t%02d:%02d:%02d\n"
	             "packet:\t%d\n"
		     "length:\t%dbytes\n"
		     "avg.packet/sec:\t%.4f(packet/s)\n"
		     "avg.packet size:%.4f(bytes/packets)\n"
		     "avg.bytes/sec:\t%.4f(bytes/s)\n"
		     "avg.MBit/sec:\t%.4f(MBit/s)\n",
		  elapsed_hour,elapsed_min,elapsed_sec,
		  packet,bytes,
		  ((float)packet)/elapsed,((float)bytes)/packet,
		  ((float)bytes)/elapsed,(bytes/1024)/elapsed);
    info_len=strlen(str_info);
    str_info[info_len]=0;
    fwrite(str_info,info_len,1,fp);

    bzero(str_info,sizeof(str_info));
    memcpy(str_info,"--------------------------\n",30);
    info_len=strlen(str_info);
    fwrite(str_info,info_len,1,fp);
}
void write_summary_file(FILE*fp)
{
    char str_info[1024];
    int info_len;
    int i,number,byte;
    for(i=0;i<PROTOCL_NUM;i++){
	number=protocl_types[i].packet_number;
	byte=protocl_types[i].packet_bytes;
	protocl_types[i].number_percent=((float)number)/packet*100;
	protocl_types[i].byte_percent=((float)byte)/bytes*100;
    }

    bzero(str_info,sizeof(str_info));
    sprintf(str_info,"type\t\t\t\tpackets\t\t\t\t%Packets\t\t\t\t"
	    	     "Bytes\t\t\t\t%Bytes\n");
    info_len=strlen(str_info);
    str_info[info_len]=0;
    fwrite(str_info,info_len,1,fp);
    for(i=0;i<PROTOCL_NUM;i++)
    {
	bzero(str_info,sizeof(str_info));
	if(i==2||i==3||i==5){
		sprintf(str_info," %s\t\t\t\t%d\t\t\t\t%.2f%\t\t\t\t\t"
			 "%d\t\t\t\t%.2f%\n",
			 protocl_types[i].name,protocl_types[i].packet_number,
			 protocl_types[i].number_percent,
			 protocl_types[i].packet_bytes,protocl_types[i].byte_percent);
	}
	else if(i==4||i==6||i==7||i==8||i==9){
		sprintf(str_info,"  %s\t\t\t\t%d\t\t\t\t%.2f%\t\t\t\t\t"
			 "%d\t\t\t\t%.2f%\n",
			 protocl_types[i].name,protocl_types[i].packet_number,
			 protocl_types[i].number_percent,
			 protocl_types[i].packet_bytes,protocl_types[i].byte_percent);
	}
	else{
		sprintf(str_info,"%s\t\t\t\t%d\t\t\t\t%.2f%\t\t\t\t\t"
			 "%d\t\t\t\t%.2f%\n",
			 protocl_types[i].name,protocl_types[i].packet_number,
			 protocl_types[i].number_percent,
			 protocl_types[i].packet_bytes,protocl_types[i].byte_percent);
	}

	info_len=strlen(str_info);
	str_info[info_len]=0;
	fwrite(str_info,info_len,1,fp);
    }

}
void read_summary_file()
{
    FILE *fp;
    char temp[100];
    if((fp=fopen("summary_file","r"))==NULL){
	printf("fopen read\n");
	exit(0);
    }
    while(fscanf(fp,"%s",temp)!=EOF){
    }
    fclose(fp);
}
void update_summary_file()
{

    FILE *fp=NULL;
    read_summary_file();

    if((fp=fopen("summary_file","w"))==NULL){
	printf("fopen write");
	exit(0);
    }
    write_summary_file_info(fp);
    write_summary_file(fp);
    fclose(fp);
		
}
void update_pkt_type(int type,int len)
{
    if(type<PROTO_ARP_||type>PROTO_SSL_)
	return;
    protocl_types[type].packet_number++;
    protocl_types[type].packet_bytes+=len;
}









