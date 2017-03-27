#include <stdio.h>
#include <math.h>
#include <sys/time.h>

int main(int argc,char**argv)
{
    struct timeval tv1,tv2;
    long i,sum,sec,usec;

    gettimeofday(&tv1,NULL);
    for(i=0,sum=0;i<10000;i++) sum+=sqrt(sqrt(i));
    gettimeofday(&tv2,NULL);

    sec=tv2.tv_sec-tv1.tv_sec;
    usec=tv2.tv_usec-tv1.tv_usec;

    if(usec<0){usec+=1000000;sec--;}

    printf("%ld %ld\n",sec,usec);
}

