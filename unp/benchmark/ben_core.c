#include <ben_config.h>
#include <ben_core.h>

#define REQUEST_SIZE	2048

int	benchtime=30;
int	proxyport=80;
int	clients=1;

char	host[MAXHOSTNAMELEN];
char	request[REQUEST_SIZE];


int	succeed=0,failed=0,bytes=0;
int	ben_timeout=0;

static const struct option long_options[]={
    {	"time",required_argument,NULL,'t'},
    {	"help",no_argument,NULL,'h'},
    {	"clients",required_argument,NULL,'c'},
    {	NULL,0,NULL,0}
};

static void usage(void)
{
    fprintf(stderr,
	    "benmark [option]...URL\n"
	    "-t|--time<sec>	run benmark for<sec> seconds.default 30.\n"
	    "-c|--clients<n>	run n http clients at once.default one.\n"
	    "-?|-h|--help	This information .\n"
	   );
}

static void ben_alarm_handler(int sig)
{
    ben_timeout=1;
}
static void ben_build_request(const char*s);
static int bench_main();
static void bench_core(const char*host,const int port,const char*req);



int main(int argc,char**argv)
{
    int				opt=0;
    int				options_index=0;

    if(argc==1){
	usage();
	return 2;
    }

    while((opt=getopt_long(argc,argv,"t:c:h",
		    long_options,&options_index))!=EOF) {
	    switch(opt){
		case 't':benchtime=atoi(optarg);break;
		case 'h':usage();return 2;break;
		case 'c':clients=atoi(optarg);break;
	    }
    }

    if(optind==argc){
	fprintf(stderr,"benmark:Missing URL.\n");
	usage();
	return 2;
    }

    if(clients==0) clients=1;
    if(benchtime==0) benchtime=60;
    fprintf(stderr,"benchmark-Simple web Benchmark\n");

    ben_build_request(argv[optind]);
    printf("host:%s port:%d,request:%s\n",host,proxyport,request);

    printf("\n Benchmarking.:GET %s,using HTTP/1.0\n",argv[optind]);
    printf("clients %d,running %d sec\n",clients,benchtime);

    return bench_main();

}


static void
ben_build_request(const char *url)
{
    char			port[10];
    int				i;
    bzero(host,MAXHOSTNAMELEN);
    bzero(request,REQUEST_SIZE);

    strcpy(request,"GET");;
    strcat(request," ");

    if(0!=strncasecmp("http://",url,7)){
	fprintf(stderr,"\n%s:is not a valid URL.\n",url);
	exit(2);
    }
    if(strlen(url)>1500){
	fprintf(stderr,"\n%s:is too long URL.\n",url);
	exit(2);
    }
    i=strstr(url,"://")-url+3;
    if(strchr(url+i,'/')==NULL){
	fprintf(stderr,"\n Invalid URL -hostname don't ends with '/'.\n");
	exit(2);
    }

    if(index(url+i,':')!=NULL&&
	    index(url+i,':')<index(url+i,'/')){
	strncpy(host,url+i,strchr(url+i,':')-url-i);
	bzero(port,10);
	strncpy(port,index(url+i,':')+1,strchr(url+i,'/')-index(url+i,':')-1);
	proxyport=atoi(port);
	if(proxyport==0)
	    proxyport=80;
    }else{
	strncpy(host,url+i,strcspn(url+i,"/"));
    }

    strcat(request+strlen(request),url+i+strcspn(url+i,"/"));
    strcat(request," HTTP/1.0\r\n");
    strcat(request,"User-Agent: Benchmark\r\n");
    strcat(request,"Host: ");
    strcat(request,host);
    strcat(request,"\r\n");
    strcat(request,"\r\n");
}

    


static int
bench_main()
{
    int				i,j,k;
    int				ben_pipe[2];
    pid_t			pid=0;
    FILE			*f;
    i=ben_socket(host,proxyport);//test connncting..
    if(i<0){
	fprintf(stderr,"\nConnect to server failed.Aborting benchmark.\n");
	return 1;
    }
    close(i);

    if(pipe(ben_pipe)){
	perror("pipe failed");
	return 3;
    }

    for(i=0;i<clients;i++){
	pid=fork();
	if(pid<=(pid_t)0){
	    sleep(1);
	    break;
	}
    }
    if(pid<(pid_t)0){
	fprintf(stderr,"problems forking worker no.%d\n",i);
	perror("fork failed");
	return 3;
    }
    if(pid==(pid_t)0){
	bench_core(host,proxyport,request);//contiuing sending request 

	//timeout,finished request..
	f=fdopen(ben_pipe[1],"w");
	if(f==NULL){
	    perror("open pipe for writing failed.");
	    return 3;
	}
	fprintf(f,"%d %d %d\n",succeed,failed,bytes);
	fclose(f);
	return 0;//this process work done
    }else{
	f=fdopen(ben_pipe[0],"r");
	if(f==NULL){
	    perror("open pipe for reading faild.");
	    return 3;
	}
	setvbuf(f,NULL,_IONBF,0);
	succeed=0;
	failed=0;
	bytes=0;

	while(1){
	    pid=fscanf(f,"%d %d %d",&i,&j,&k);
	    if(pid<3){
		fprintf(stderr,"some of our children died.\n");
		break;
	    }
	    succeed+=i;
	    failed+=j;
	    bytes+=k;

	    if(--clients==0)
		break;
	}
	fclose(f);
	printf("\n Speed=%d request/min,%d bytes/sec.\n Request static:\n%d succeed,%d failed.\n",
		(int)((succeed+failed)/(benchtime/60.0f)),
		(int)(bytes/(float)benchtime),
		succeed,
		failed);
    }//end else

    return i;
}

static void 
bench_core(const char*host,const int port,const char*req)
{
    int			rlen;
    char		buf[1500];
    int			i,s;
    struct sigaction 	sa;

    sa.sa_handler=ben_alarm_handler;
    sa.sa_flags=0;

    if(sigaction(SIGALRM,&sa,NULL))
	exit(3);

    alarm(benchtime);

    rlen=strlen(req);

nexttry:
    while(1){
	if(ben_timeout){
	    if(failed){
		failed--;
	    }
	    return;
	}

	s=ben_socket(host,port);
	if(s<0){
	    failed++;
	    continue;
	}

	if(rlen!=write(s,req,rlen)){
	    failed++;
	    close(s);
	    continue;
	}

	//reciev response
	while(1){
	    if(ben_timeout)
		break;
	    i=read(s,buf,1500);
	    if(i<0){
		failed++;
		close(s);
		goto nexttry;
	    }else{
		if(i==0)
		    break;
		else
		    bytes+=i;
	    }
	}
	if(close(s)){
	    failed++;
	    continue;
	}
	succeed++;
    }
}













