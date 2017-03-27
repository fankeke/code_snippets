#include <sv_config.h>
#include <sv_core.h>

static void sigcatch(int sig)
{
    syslog(LOG_ERR,"signal pipe happed");
}
int main(int argc,char**argv)
{

    int			pid;
    signal(SIGPIPE,sigcatch);
    pid=fork();
    if(pid==0){
	setsid();
	sv_core_httpd();
    }
    return 0;
}
